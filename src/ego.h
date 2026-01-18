#pragma once

#include "raymath.h"
#include "config.h"
#include "utils.h"

namespace ego
{
	class Ego 
	{
	private:
		class EgoPos
		{
		private:
			class MeterCoord
			{
			private:
				float& value_m;
				float& value_px;
			public:
				MeterCoord(float& value_m, float& value_px) : value_m(value_m), value_px(value_px) {}
				MeterCoord& operator=(const float value)
				{
					value_m = value;
					value_px = value * config::general::convert::mToPx;
					return *this;
				}
				MeterCoord& operator+=(const float value)
				{
					value_m += value;
					value_px += value * config::general::convert::mToPx;
					return *this;
				}
				MeterCoord& operator-=(const float value) { return (*this) += (-value); }
				operator float() const { return value_m; }
			};
			class PixelCoord
			{
			private:
				float& value_m;
				float& value_px;
			public:
				PixelCoord(float& value_m, float& value_px) : value_m(value_m), value_px(value_px) {}
				PixelCoord& operator=(const float value)
				{
					value_px = value;
					value_m = value * config::general::convert::pxToM;
					return *this;
				}
				PixelCoord& operator+=(const float value)
				{
					value_px += value;
					value_m += value * config::general::convert::pxToM;
					return *this;
				}
				PixelCoord& operator-=(const float value) { return (*this) += (-value); }
				operator float() const { return value_px; }
			};

		private:
			float _x_m{ 0.0 };
			float _y_m{ 0.0 };
			float _x_px{ 0.0 };
			float _y_px{ 0.0 };

		public:
			MeterCoord x_m{ _x_m, _x_px };
			MeterCoord y_m{ _y_m, _y_px };
			PixelCoord x_px{ _x_m, _x_px };
			PixelCoord y_px{ _y_m, _y_px };

			EgoPos() {}
			Vector2 asVector2M() const { return Vector2{ x_m, y_m }; }
			Vector2 asVector2Px() const { return Vector2{ x_px, y_px }; }
			Vector2 asVector2PxInvertY() const { return Vector2{ x_px, -y_px }; }
		};

	public:
		EgoPos pos{};
		float vel_mps{ 0.0 };
		float rot_rad{ 0.0 };
		float wheelRot_rad{ 0.0 };
		
		EgoPos icr{};
		float r_m = config::general::values::inf_m;
		float angularVel_radps = 0.0;

		Ego() { icr.y_m = config::general::values::inf_m; }

		types::EgoContour computeRotatedEgoContourPx(const types::EgoContour& egoContour_m) const
		{
			types::EgoContour contour_px;
			Vector2 egoPos_px{ pos.asVector2Px() };

			for (size_t i = 0; i < contour_px.size(); i++)
			{
				Vector2& currPt_px{ contour_px[i] };

				// 1. Convert point from m to px
				currPt_px = utils::convertPosMtoPosPx(egoContour_m[i]);

				// 2. Rotate point (its still around origin)
				currPt_px = Vector2Rotate(currPt_px, rot_rad);

				// 3. Translate point to ego location
				currPt_px = Vector2Add(currPt_px, egoPos_px);
			}
			return contour_px;
		}

		types::WheelContour computeRotatedWheelContourPx(const types::WheelContour& wheelContour_m, bool steeringWheel = false) const
		{
			types::WheelContour wheelContour_px;

			Vector2 egoPos_px{ pos.asVector2Px() };
			Vector2 wheelCenter_px{ utils::convertPosMtoPosPx(utils::getRectCenter(wheelContour_m)) };

			for (size_t i = 0; i < wheelContour_px.size(); i++)
			{
				Vector2& currPt_px{ wheelContour_px[i] };

				// 1. Convert point from m to px
				currPt_px = utils::convertPosMtoPosPx(wheelContour_m[i]);

				if (steeringWheel)
				{
					// 2. If front wheel, rotate point (around its center)
					currPt_px = Vector2Subtract(currPt_px, wheelCenter_px);
					currPt_px = Vector2Rotate(currPt_px, wheelRot_rad);
					currPt_px = Vector2Add(currPt_px, wheelCenter_px);
				}

				// 3. Rotate point (along with ego)
				currPt_px = Vector2Rotate(currPt_px, rot_rad);

				// 4. Translate point to ego location
				currPt_px = Vector2Add(currPt_px, egoPos_px);
			}
			return wheelContour_px;
		}
	
		void updatePosition()
		{
			// 1. Wheel angle -> icr, r
			r_m = config::ego::geom::wheelbase_m / tanf(wheelRot_rad);
			icr.x_m = pos.x_m + r_m * cosf(rot_rad + 90 * config::general::convert::degToRad);
			icr.y_m = pos.y_m + r_m * sinf(rot_rad + 90 * config::general::convert::degToRad);

			// 2. icr, r, vel -> angularVel (also yaw)
			angularVel_radps = vel_mps / r_m;

			// 3. rot, angularVel -> new rot
			rot_rad += angularVel_radps * GetFrameTime();

			// 4. pos, icr, angularVel -> new pos
			pos.x_m += vel_mps * GetFrameTime() * cosf(rot_rad);
			pos.y_m += vel_mps * GetFrameTime() * sinf(rot_rad);

		}
	};
}
