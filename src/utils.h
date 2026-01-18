#pragma once

#include <vector>
#include <array>
#include "raymath.h"
#include "config.h"

namespace utils
{
	namespace detail
	{
		template <typename T, T start, T stop, T step = 1>
		struct RangeSize { static constexpr size_t value = (stop - start) / step + 1; };

		template <typename T, T start, T stop, T step = 1, size_t... Is>
		constexpr std::array<T, sizeof...(Is)> range_impl(std::index_sequence<Is...>)
		{
			return { { (start + step * static_cast<T>(Is))... } };
		}
	}

	template <typename T, T start, T stop, T step = 1>
	constexpr std::array<T, detail::RangeSize<T, start, stop, step>::value> range()
	{
		return detail::range_impl<T, start, stop, step>(std::make_index_sequence<detail::RangeSize<T, start, stop, step>::value>{});
	}

	Vector2 convertPosMtoPosPx(const Vector2& pos_m)
	{
		Vector2 pos_px{ Vector2Scale(pos_m, config::general::convert::mToPx) };
		return pos_px;
	}

	float convertMtoPx(float dist_m) { return dist_m * config::general::convert::mToPx; }

	Vector2 getScreenCenter()
	{
		return { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) };
	}

	Vector2 getRectCenter(const types::Rect& rect)
	{
		Vector2 result{ (rect[0].x + rect[2].x) / 2, (rect[0].y + rect[2].y) / 2 };
		return result;
	}
}
