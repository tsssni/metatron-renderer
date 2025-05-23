#pragma once
#include <metatron/core/math/matrix.hpp>
#include <metatron/core/math/arithmetic.hpp>
#include <cmath>

namespace metatron::math {
	template<typename T, usize size>
	using Vector = Matrix<T, size>;

	template<usize n>
	auto inline guarded_div(Vector<f32, n> const& x, f32 y) -> Vector<f32, n> {
		return std::abs(y) < epsilon<f32> ? Vector<f32, n>{0.f} : x / y;
	}

	template<usize n>
	auto inline guarded_div(Vector<f32, n> const& x, Vector<f32, n> const& y) -> Vector<f32, n> {
		return foreach(x, [&y](f32 x, usize idx) -> f32 {
			return guarded_div(x, y[idx]);
		});
	}

	template<typename T, usize size>
	auto dot(Vector<T, size> const& x, Vector<T, size> const& y) -> T {
		auto result = T{};
		for (auto i = 0; i < size; i++) {
			result += x[i] * y[i];
		}
		return result;
	}

	template<typename T>
	auto cross(Vector<T, 3> const& x, Vector<T, 3> const& y) -> Vector<T, 3> {
		return {
			x[1] * y[2] - x[2] * y[1],
			x[2] * y[0] - x[0] * y[2],
			x[0] * y[1] - x[1] * y[0]
		};
	}

	template<typename T, usize size>
	requires std::floating_point<T>
	auto length(Vector<T, size> const& x) -> T {
		return sqrt(dot(x, x));
	}

	template<typename T, usize size>
	requires std::floating_point<T>
	auto normalize(Vector<T, size> const& x) -> Vector<T, size> {
		return guarded_div(x, length(x));
	}

	template<typename T>
	requires std::floating_point<T>
	auto reflect(Vector<T, 3> const& in, Vector<T, 3> const& n) -> Vector<T, 3> {
		return T{2.0} * n * dot(-in, n) + in; 
	}

	template<typename T>
	requires std::floating_point<T>
	auto refract(Vector<T, 3> const& in, Vector<T, 3> const& n, T const& eta) -> Vector<T, 3> {
		auto cos_theta_i = dot(in, n);
		auto cos_2_theta_t = T{1.0} - eta * eta * (T{1.0} - cos_theta_i * cos_theta_i); 
		if (cos_2_theta_t < 0.0) return Vector<T, 3>{T{0.0}};
		return eta * in - (eta * cos_theta_i + sqrt(cos_2_theta_t)) * n;
	}

	template<typename T, typename... Ts, usize n, usize tail = sizeof...(Ts)>
	requires (std::is_convertible_v<T, Ts> && ...)
	auto expand(Vector<T, n> const& x, Ts... v) -> Vector<T, n + tail> {
		return Vector<T, n + sizeof...(v)>{x, v...};
	}

	template<typename T, usize n, usize tail = 1uz>
	requires (n > tail)
	auto shrink(Vector<T, n> const& x) -> Vector<T, n - tail> {
		return Vector<T, n - tail>{x};
	}

	template<
		typename T,
		typename Func,
		typename Return_Type = decltype(std::declval<Func>()(std::declval<T>(), std::declval<usize>())),
		usize size
	>
	auto foreach(Vector<T, size> const& x, Func f) -> Vector<Return_Type, size> {
		auto r = Vector<Return_Type, size>{};
		for (auto i = 0uz; i < size; i++) {
			r[i] = f(x[i], i);
		}
		return r;
	}

	template<typename T, usize size>
	requires std::totally_ordered<T>
	auto min(Vector<T, size> const& x) -> T {
		auto y = x[0];
		for (auto i = 1uz; i < size; i++) {
			y = std::min(y, x[i]);
		}
		return y;
	}

	template<typename T, usize size>
	requires std::totally_ordered<T>
	auto max(Vector<T, size> const& x) -> T {
		auto y = x[0];
		for (auto i = 1uz; i < size; i++) {
			y = std::max(y, x[i]);
		}
		return y;
	}

	template<typename T, usize size>
	requires std::totally_ordered<T>
	auto clamp(Vector<T, size> const& x, Vector<T, size> const& l, Vector<T, size> const& r) -> Vector<T, size> {
		return foreach(x, [&](T const& v, usize i) -> T {
			return std::clamp(v, l[i], r[i]);
		});
	}

	template<typename T, usize size>
	requires std::floating_point<T>
	auto lerp(Vector<T, size> const& x, Vector<T, size> const& y, T const& alpha) -> Vector<T, size> {
		return (T{1.0} - alpha) * x + alpha * y;
	}

	template<typename T, usize size>
	requires std::floating_point<T>
	auto lerp(Vector<T, size> const& x, Vector<T, size> const& y, Vector<T, size> const& alpha) -> Vector<T, size> {
		return (T{1.0} - alpha) * x + alpha * y;
	}

	template<typename T, usize size>
	requires std::floating_point<T> || std::integral<T>
	auto abs(Vector<T, size> const& x) -> Vector<T, size> {
		return foreach(x, [](T const& v, usize) -> T {
			return std::abs(v);
		});
	}

	template<typename T, usize size>
	requires requires(T a, T b) { a + b; }
	auto sum(Vector<T, size> const& x) -> T {
		auto y = T{};
		foreach(x, [&y](T const& v, usize) -> T {
			y += v;
			return y;
		});
		return y;
	}

	template<typename T, usize size>
	requires requires(T a, T b) { a * b; }
	auto prod(Vector<T, size> const& x) -> T {
		auto y = T{1};
		foreach(x, [&y](T const& v, usize) -> T {
			y *= v;
			return y;
		});
		return y;
	}

	template<typename T, usize size>
	requires std::floating_point<T> || std::integral<T>
	auto mod(Vector<T, size> const& x, T const& m) -> Vector<T, size> {
		return foreach(x, [&](T const& v, usize i) -> T {
			if constexpr (std::floating_point<T>) {
				return std::fmod(v, m);
			} else {
				return v % m;
			}
		});
	}

	template<typename T, usize size>
	requires std::floating_point<T> || std::integral<T>
	auto mod(Vector<T, size> const& x, Vector<T, size> const& m) -> Vector<T, size> {
		return foreach(x, [&](T const& v, usize i) -> T {
			if constexpr (std::floating_point<T>) {
				return std::fmod(v, m[i]);
			} else {
				return v % m[i];
			}
		});
	}
}
