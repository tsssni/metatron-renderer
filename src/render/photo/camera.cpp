#include <metatron/render/photo/camera.hpp>

namespace metatron::photo {
	Camera::Camera(
		std::unique_ptr<Film> film,
		std::unique_ptr<Lens> lens
	) : film(std::move(film)), lens(std::move(lens)) {}

	auto Camera::sample(
		math::Vector<usize, 2> pixel,
		usize idx,
		math::Sampler& sampler
	) -> Sample {
		sampler.start(pixel, idx);
		auto sample = sampler.generate_pixel_2d();
		auto pixel_position = math::Vector<f32, 2>{pixel} + sample;
		auto fixel = film->sample(pixel_position);
		auto ray = lens->sample({fixel.position[0], fixel.position[1], 0.f}, {0.f, 0.f});
		return {ray, fixel};
	}

	auto Camera::to_path(std::string_view path) -> void {
		film->to_path(path);
	}
}
