#include <metatron/render/photo/camera.hpp>

namespace metatron::photo {
	Camera::Camera(
		std::unique_ptr<Film> film,
		std::unique_ptr<Lens> lens
	): film(std::move(film)), lens(std::move(lens)) {}

	auto Camera::sample(
		math::Vector<usize, 2> pixel,
		usize idx,
		math::Sampler& sampler
	) -> std::optional<Interaction> {
		sampler.start(pixel, idx);
		auto sample = sampler.generate_pixel_2d();
		auto pixel_position = sample + pixel;
		auto fixel = (*film)(pixel_position);

		auto lintr = lens->sample({fixel.position, 0.f}, {0.f, 0.f});
		if (!lintr) return {};

		auto& ray = lintr.value().r;
		return Interaction{ray, fixel};
	}

	auto Camera::to_path(std::string_view path) -> void {
		film->to_path(path);
	}
}
