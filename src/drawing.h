#pragma once

#include <cmath>
#include <array>
#include "raylib.h"
#include "config.h"
#include "utils.h"
#include "types.h"
#include "ego.h"

namespace drawing
{
	namespace detail
	{
		namespace cartesian
		{
			Vector2 invertY(const Vector2 pt) { return { pt.x, -pt.y }; }

			void drawLine(const Vector2& start, const Vector2& stop, const float thick, const Color& color)
			{
				DrawLineEx(invertY(start), invertY(stop), thick, color);
			}

			template <size_t size>
			void drawPolygon(const std::array<Vector2, size>& pts, const float thick, const Color& color)
			{
				for (size_t i = 0; i < pts.size() - 1; i++)
				{
					const Vector2& pt1{ pts[i] };
					const Vector2& pt2{ pts[i + 1] };
					drawLine(pt1, pt2, thick, color);
				}
				const Vector2& pt1{ pts[pts.size() - 1] };
				const Vector2& pt2{ pts[0] };
				drawLine(pt1, pt2, thick, color);
			}

			void drawCircleFilled(const Vector2& center, const float radius, const Color& color)
			{
				DrawCircleV(invertY(center), radius, color);
			}

			void drawCircle(const Vector2& center, const float radius, const int thick, const Color& color)
			{
				const int start_r = radius - thick / 2;
				for (int i = 0; i < thick; i++)
				{
					DrawCircleLinesV(invertY(center), start_r + i, color);
				}
			}
		}

		namespace text
		{
			Vector2 measureText(const Font& font, const char* buffer, const float fontSize, const float charSpacing, const float lineSpacing)
			{
				const float textWidth = MeasureTextEx(font, buffer, fontSize, charSpacing).x;

				const float lineHeight = MeasureTextEx(font, "a", fontSize, charSpacing).y;
				const int numLines = std::count(buffer, buffer + strlen(buffer), '\n') + 1;

				const float textHeight = lineHeight * numLines + lineSpacing * (numLines - 1);

				return { textWidth, textHeight };
			}

			void drawText(const Font& font, const char* buffer, const Vector2 pos, const float fontSize, const float charSpacing, const float lineSpacing, const Color& color)
			{
				Vector2 textSize = measureText(font, buffer, fontSize, charSpacing, lineSpacing);

				const size_t bufferSize = strlen(buffer);
				const float lineHeight = MeasureTextEx(font, "a", fontSize, charSpacing).y;

				Vector2 currPos = pos;
				char currLine[128];
				size_t currLineStart = 0;
				for (size_t i = 0; i < bufferSize; i++)
				{
					if (buffer[i] == '\n')  // line end found
					{
						strncpy(currLine, buffer + currLineStart, i - currLineStart);
						currLine[i - currLineStart] = '\0';
						DrawTextEx(font, currLine, currPos, fontSize, charSpacing, color);
						
						currPos.y += lineHeight + lineSpacing;
						currLineStart = i + 1;
					}
				}
				strncpy(currLine, buffer + currLineStart, bufferSize - currLineStart + 1);
				DrawTextEx(font, currLine, currPos, fontSize, charSpacing, color);
			}
		}
	}

	namespace worldCoords
	{
		void drawBackgroundScreen()
		{
			constexpr float minX_m = -config::bg::gridLines::renderDistance_m;
			constexpr float maxX_m = +config::bg::gridLines::renderDistance_m;
			constexpr float minY_m = -config::bg::gridLines::renderDistance_m;
			constexpr float maxY_m = +config::bg::gridLines::renderDistance_m;

			constexpr int minX_px = minX_m * config::general::convert::mToPx;
			constexpr int maxX_px = maxX_m * config::general::convert::mToPx;
			constexpr int minY_px = minY_m * config::general::convert::mToPx;
			constexpr int maxY_px = maxY_m * config::general::convert::mToPx;

			constexpr int gridLinesInterval_px = config::bg::gridLines::interval_m * config::general::convert::mToPx;

			constexpr int numVericalGridLines = (maxX_px - minX_px) / gridLinesInterval_px + 1;
			constexpr int numHorizontalGridLines = (maxY_px - minY_px) / gridLinesInterval_px + 1;
			constexpr std::array<int, numVericalGridLines>    gridLineXs_px = utils::range<int, minX_px, maxX_px, gridLinesInterval_px>();
			constexpr std::array<int, numHorizontalGridLines> gridLineYs_px = utils::range<int, minY_px, maxY_px, gridLinesInterval_px>();

			ClearBackground(config::bg::color);

			for (const int& x_px : gridLineXs_px)
			{
				detail::cartesian::drawLine({ static_cast<float>(x_px), minY_px },
					{ static_cast<float>(x_px), maxY_px },
					config::bg::gridLines::thickness_px,
					config::bg::gridLines::color);
			}

			for (const int& y_px : gridLineYs_px)
			{
				detail::cartesian::drawLine({ minX_px, static_cast<float>(y_px) },
					{ maxX_px, static_cast<float>(y_px) },
					config::bg::gridLines::thickness_px,
					config::bg::gridLines::color);
			}
		}

		void drawEgo(const ego::Ego& ego)
		{
			types::EgoContour egoContour_px{ ego.computeRotatedEgoContourPx(config::ego::geom::contour_m) };
			detail::cartesian::drawPolygon(egoContour_px, config::ego::draw::lineThickness, config::ego::draw::color);

			types::WheelContour wheelContourFL_px{ ego.computeRotatedWheelContourPx(config::ego::geom::wheelContourFL_m, true) };
			detail::cartesian::drawPolygon(wheelContourFL_px, config::ego::draw::lineThickness, config::ego::draw::color);

			types::WheelContour wheelContourFR_px{ ego.computeRotatedWheelContourPx(config::ego::geom::wheelContourFR_m, true) };
			detail::cartesian::drawPolygon(wheelContourFR_px, config::ego::draw::lineThickness, config::ego::draw::color);

			types::WheelContour wheelContourRL_px{ ego.computeRotatedWheelContourPx(config::ego::geom::wheelContourRL_m) };
			detail::cartesian::drawPolygon(wheelContourRL_px, config::ego::draw::lineThickness, config::ego::draw::color);

			types::WheelContour wheelContourRR_px{ ego.computeRotatedWheelContourPx(config::ego::geom::wheelContourRR_m) };
			detail::cartesian::drawPolygon(wheelContourRR_px, config::ego::draw::lineThickness, config::ego::draw::color);

			detail::cartesian::drawCircleFilled(ego.pos.asVector2Px(), 5.0, config::ego::draw::color);
		}

		void drawInfo(const ego::Ego& ego)
		{
			detail::cartesian::drawCircle(ego.icr.asVector2Px(), utils::convertMtoPx(ego.r_m), 1, DARKGREEN);
		}
	}

	namespace screenCoords
	{
		void drawInfo(const ego::Ego& ego)
		{
			// 1. fps:       60
			// 2. vel.:      5 m/s
			// 3. ang. vel.: 23 deg/s
			// 4. whl. ang.: 21 deg
			// 5. R:         14 m

			char buffer[256];
			snprintf(buffer, sizeof(buffer),
				"fps:      %u         \n"
				"vel.:     %.2f m/s   \n"
				"ang.vel.: %.1f deg/s \n"
				"whl.ang.: %.1f deg   \n"
				"R:        %.1f m     ",
				GetFPS(),
				ego.vel_mps,
				ego.angularVel_radps * (1 / config::general::convert::degToRad),
				ego.wheelRot_rad * (1 / config::general::convert::degToRad),
				ego.r_m);

			constexpr float fontSize = 20.0;
			constexpr float charSpacing = 0.0;
			constexpr float lineSpacing = 0.0;

			Font font = LoadFontEx("C:/Windows/Fonts/consola.ttf", fontSize, nullptr, 0);

			const float textHeight = detail::text::measureText(font, buffer, fontSize, charSpacing, lineSpacing).y;
			detail::text::drawText(font, buffer, { 10, GetScreenHeight() - textHeight - 10 }, fontSize, charSpacing, lineSpacing, GREEN);
		}
	}
}
