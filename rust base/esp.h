#pragma once
#include "includes.hpp"
#include "game/classes.hpp"

namespace esp
{
	void render_Health(rust::BasePlayer* player);
	void render_distance(rust::BasePlayer* player);
	void render_name(rust::BasePlayer* player);
	void render_box(rust::BasePlayer* player);
	void RenderSkeleton(rust::BasePlayer* Player, BoneList Bone1, BoneList Bone2);
	void do_chams(rust::BasePlayer* player);
	void radar();
	void render_hotbar(rust::BasePlayer* player);
	void aimbot(rust::BasePlayer* player);
	void fov(float fov);	
	void PSlient(rust::BasePlayer* player);
	void flyhackbar();
	void FlyHack();
	void adman();
	void NoSway();
}