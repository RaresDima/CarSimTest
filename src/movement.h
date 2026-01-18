#pragma once

#include <array>
#include "raymath.h"
#include "raylib.h"
#include "config.h"
#include "utils.h"
#include "ego.h"

namespace movement
{
	void handleEgoInput(ego::Ego& ego)
	{
		constexpr float maxWheelRotation_rad = config::ego::geom::maxWheelRotation_deg * config::general::convert::degToRad;

		if (IsKeyDown(KEY_UP))
		{ 
			ego.vel_mps = Clamp(ego.vel_mps + GetFrameTime() * config::ego::accel_mps2,
				                -config::ego::maxSpeed_mps, 
				                config::ego::maxSpeed_mps);
		}
		if (IsKeyDown(KEY_DOWN))  
		{ 
			ego.vel_mps = Clamp(ego.vel_mps - GetFrameTime() * config::ego::accel_mps2,
				                -config::ego::maxSpeed_mps, 
				                config::ego::maxSpeed_mps);
		}

		if (IsKeyDown(KEY_SPACE))
		{
			ego.vel_mps = copysign(Clamp(abs(ego.vel_mps) - GetFrameTime() * config::ego::braking_mps2, 
				                         0, 
				                         config::ego::maxSpeed_mps), 
				                   ego.vel_mps);
		}

		if (IsKeyDown(KEY_LEFT))  
		{ 
			ego.wheelRot_rad = Clamp(ego.wheelRot_rad + config::ego::wheelTurnSpeed_degps * GetFrameTime() * config::general::convert::degToRad, 
				                     -maxWheelRotation_rad, 
				                     maxWheelRotation_rad); 
		}
		if (IsKeyDown(KEY_RIGHT)) 
		{ 
			ego.wheelRot_rad = Clamp(ego.wheelRot_rad - config::ego::wheelTurnSpeed_degps * GetFrameTime() * config::general::convert::degToRad,
				                     -maxWheelRotation_rad,
				                     maxWheelRotation_rad);
		}
	}

	void handleCameraFollow(Camera2D& camera, const ego::Ego& ego, bool lerp = false)
	{
		if (lerp) 
		{ 
			camera.target = Vector2Lerp(camera.target, ego.pos.asVector2PxInvertY(), 0.1);
		}
		else
		{
			camera.target = ego.pos.asVector2PxInvertY();
		}
	}

	void handleEgoMovement(ego::Ego& ego) { ego.updatePosition(); }
}
