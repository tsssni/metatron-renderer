#pragma once
#include <metatron/render/light/light.hpp>
#include <metatron/geometry/material/texture/texture.hpp>
#include <metatron/core/image/image.hpp>

namespace metatron::light {
	struct Environment_Light final: Light {
		Environment_Light(std::unique_ptr<material::Texture<spectra::Stochastic_Spectrum>> env_map);

		auto operator()(
			math::Vector<f32, 3> const& wo,
			math::Vector<f32, 3> const& n,
			spectra::Stochastic_Spectrum const& L
		) const -> std::optional<Interaction>;
		auto sample(eval::Context const& ctx, math::Vector<f32, 2> const& u) const -> std::optional<Interaction>;

	private:
		std::unique_ptr<material::Texture<spectra::Stochastic_Spectrum>> env_map;
	};
}
