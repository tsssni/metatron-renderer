#pragma once
#include <metatron/render/photo/film.hpp>
#include <metatron/render/photo/lens.hpp>
#include <metatron/core/math/vector.hpp>
#include <metatron/core/math/ray.hpp>
#include <metatron/core/math/sampler.hpp>
#include <memory>

namespace metatron::photo {
	struct Camera final {
		struct Sample final {
			math::Ray r;
			Film::Fixel fixel;
		};

		Camera(
			std::unique_ptr<Film> film,
			std::unique_ptr<Lens> lens
		);
		auto sample(
			math::Vector<usize, 2> pixel,
			usize idx,
			math::Sampler& sampler
		) -> Sample;
		auto to_path(std::string_view path) -> void;

	private:
		std::unique_ptr<Film> film;
		std::unique_ptr<Lens> lens;
	};
}
