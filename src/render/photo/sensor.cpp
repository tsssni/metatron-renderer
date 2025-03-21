#include <metatron/render/photo/sensor.hpp>

namespace metatron::photo {
	Sensor::Sensor(
		std::unique_ptr<spectra::Spectrum> r,
		std::unique_ptr<spectra::Spectrum> g,
		std::unique_ptr<spectra::Spectrum> b
	): r(std::move(r)), g(std::move(g)), b(std::move(b)) {}

	auto Sensor::operator()(spectra::Stochastic_Spectrum const& spectrum) -> math::Vector<f32, 3> {
		// TODO: just test rgb
		return math::Vector<f32, 3>{spectrum.value};
		// auto r = spectrum(*(this->r));
		// auto g = spectrum(*(this->g));
		// auto b = spectrum(*(this->b));
		// return {r, g, b};
	}
}
