#pragma once
#include <metatron/geometry/intr/interaction.hpp>
#include <metatron/core/math/bounding-box.hpp>
#include <optional>

namespace metatron::shape {
	struct Shape {
		virtual ~Shape() {}
		auto virtual bounding_box(usize idx = 0uz) const -> math::Bounding_Box = 0;
		auto virtual sample(
			intr::Context const& ctx,
			math::Vector<f32, 2> const& u,
			usize idx = 0uz
		) const -> std::optional<intr::Interaction> = 0;
		auto virtual intersect(
			math::Ray const& r,
			usize idx = 0uz
		) const -> std::optional<intr::Interaction> = 0;
	};
}
