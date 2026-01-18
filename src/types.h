#pragma once

#include <array>
#include "raylib.h"

namespace types
{
	using Rect = std::array<Vector2, 4>;
	using EgoContour = Rect;
	using WheelContour = Rect;
}
