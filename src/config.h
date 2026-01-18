#pragma once

#include <array>
#include "raylib.h"
#include "types.h"

namespace config
{
	namespace env
	{
		namespace screen
		{
			constexpr unsigned int width_px = 1600;
			constexpr unsigned int height_px = 1200;
		}
		constexpr unsigned int fps = 60;
	}

	namespace general
	{
		namespace convert
		{
			constexpr unsigned int mToPx = 50;
			constexpr float pxToM = 1.0 / mToPx;
			constexpr float degToRad = PI / 180.0;
		}
		namespace values
		{
			constexpr float inf_m = 1000.0;
			constexpr float inf_deg = 360.0;
		}
	}

	namespace bg
	{
		constexpr Color color = { 40, 40, 40, 255 };
		namespace gridLines
		{
			constexpr Color color = { 100, 100, 100, 255 };
			constexpr float renderDistance_m = 100.0;
			constexpr float interval_m = 1.0;
			constexpr float thickness_px = 1.0;
		}
	}

	namespace ego
	{
		constexpr float maxSpeed_mps = 10.0;
		constexpr float accel_mps2 = 3.0;
		constexpr float braking_mps2 = 5.0;
		constexpr float wheelTurnSpeed_degps = 30.0;

		namespace geom
		{
			constexpr float rearOverhang_m = 1.0;
			constexpr float wheelbase_m = 2.7;
			constexpr float frontOverhang_m = 0.9;
			constexpr float totalLength_m = 4.6;
			constexpr float bodyWidth_m = 1.8;

			constexpr float trackWidth_m = 1.55;
			constexpr float wheelDiameter_m = 0.64;
			constexpr float tireWidth_m = 0.215;

			constexpr float maxWheelRotation_deg = 30.0;

			constexpr types::EgoContour contour_m = { { { wheelbase_m + frontOverhang_m, bodyWidth_m / 2 },
                                                        { wheelbase_m + frontOverhang_m, -(bodyWidth_m / 2) },
                                                        { -rearOverhang_m, -(bodyWidth_m / 2)},
                                                        { -rearOverhang_m,  bodyWidth_m / 2 } } };

			constexpr types::WheelContour wheelContourFL_m = { { { wheelbase_m + wheelDiameter_m / 2, trackWidth_m / 2 + tireWidth_m / 2 },
																 { wheelbase_m + wheelDiameter_m / 2, trackWidth_m / 2 - tireWidth_m / 2 },
																 { wheelbase_m - wheelDiameter_m / 2, trackWidth_m / 2 - tireWidth_m / 2 },
																 { wheelbase_m - wheelDiameter_m / 2, trackWidth_m / 2 + tireWidth_m / 2 } } };

			constexpr types::WheelContour wheelContourFR_m = { { { wheelbase_m + wheelDiameter_m / 2, -(trackWidth_m / 2) + tireWidth_m / 2 },
																 { wheelbase_m + wheelDiameter_m / 2, -(trackWidth_m / 2) - tireWidth_m / 2 },
																 { wheelbase_m - wheelDiameter_m / 2, -(trackWidth_m / 2) - tireWidth_m / 2 },
																 { wheelbase_m - wheelDiameter_m / 2, -(trackWidth_m / 2) + tireWidth_m / 2 } } };

			constexpr types::WheelContour wheelContourRL_m = { { {   wheelDiameter_m / 2,  trackWidth_m / 2 + tireWidth_m / 2 },
																 {   wheelDiameter_m / 2,  trackWidth_m / 2 - tireWidth_m / 2 },
																 { -(wheelDiameter_m / 2), trackWidth_m / 2 - tireWidth_m / 2 },
																 { -(wheelDiameter_m / 2), trackWidth_m / 2 + tireWidth_m / 2 } } };

			constexpr types::WheelContour wheelContourRR_m = { { {   wheelDiameter_m / 2,  -(trackWidth_m / 2) + tireWidth_m / 2 },
																 {   wheelDiameter_m / 2,  -(trackWidth_m / 2) - tireWidth_m / 2 },
																 { -(wheelDiameter_m / 2), -(trackWidth_m / 2) - tireWidth_m / 2 },
																 { -(wheelDiameter_m / 2), -(trackWidth_m / 2) + tireWidth_m / 2 } } };
		}
		namespace draw
		{
			constexpr Color color = GREEN;
			constexpr float lineThickness = 3.0;
		}
	}
}
