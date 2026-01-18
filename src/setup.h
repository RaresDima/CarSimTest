#pragma once

#include "raylib.h"
#include "utils.h"
#include "ego.h"

namespace setup
{
	Camera2D setupCamera(const ego::Ego& ego)
	{
		Camera2D camera = { 0 };
		camera.target = ego.pos.asVector2Px();
		camera.offset = utils::getScreenCenter();
		camera.rotation = 0;
		camera.zoom = 1.0;
		return camera;
	}
}
