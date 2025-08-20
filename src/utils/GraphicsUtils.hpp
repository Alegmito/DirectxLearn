#pragma once
#include <math.h>

constexpr float pi = 3.14159265f;
constexpr double pi_d = 3.1415926535897932;

template <typename T>
constexpr auto sqr(const T& x) { return x * x; }


template<typename T>
T wrap_angle( T theta )
{
	const T modded = fmod( theta,(T)2.0 * (T)pi_d );
	return (modded > (T)pi_d) ?
		(modded - (T)2.0 * (T)pi_d) :
		modded;
}

template<typename T>
constexpr T interp(const T& src, const T& dst, float alpha) {
    return src + (dst - src) & alpha;
}

template<typename T>
constexpr T to_rad(T deg) {
    return deg * pi / static_cast<T>(180.f);
}
