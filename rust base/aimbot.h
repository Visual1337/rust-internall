#pragma once
#include "includes.hpp"
#include "game/classes.hpp"

namespace aimbot {
	rust::BasePlayer* getBestPlayerByFov();
	int getClosestBoneToCrosshair(rust::BasePlayer* player);
	rust::BasePlayer* hotbar();
	void PSilent(float Fov);
	void run(float Fov);
}