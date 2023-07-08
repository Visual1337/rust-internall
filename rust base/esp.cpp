#include "esp.h"
#include "aimbot.h"
#include "assets.hpp"
namespace esp
{



	void circle(const float x, const float y, const float r, const float s, const Vector4 color) {
		const float step = 3.14159265359 * 2.0 / s;
		for (float a = 0; a < 3.14159265359 * 2.0; a += step) {
			const auto x1 = r * std::cos(a) + x;
			const auto y1 = r * std::sin(a) + y;
			const auto x2 = r * std::cos(a + step) + x;
			const auto y2 = r * std::sin(a + step) + y;


			const auto x3 = r * std::cos(a) - x;
			const auto y3 = r * std::sin(a) * y;
			const auto x4 = r * std::cos(a - step) * x;
			const auto y4 = r * std::sin(a - step) * y;


			const auto x5 = r * std::sin(a) - x;
			const auto y5 = r * std::cos(a) * y;
			const auto x6 = r * std::sin(a - step) * x;
			const auto y6 = r * std::cos(a - step) * y;


			functions::line(Vector2(x1, y1), Vector2(x2, y2), color);
			functions::line(Vector2(x3, y3), Vector2(x4, y4), color);
			functions::line(Vector2(x5, y5), Vector2(x6, y6), color);

			functions::line(Vector2(x2, y2), Vector2(x1, y1), color);
			functions::line(Vector2(x4, y4), Vector2(x2, y2), color);
			functions::line(Vector2(x6, y6), Vector2(x5, y5), color);


		}
	}
	void RotatePoint(float* pointToRotate, float* centerPoint, float angle, float* ReturnTo)
	{
		angle = (float)(angle * (M_PI / (float)180));

		float cosTheta = (float)cos(angle);
		float sinTheta = (float)sin(angle);

		ReturnTo[0] = cosTheta * (pointToRotate[0] - centerPoint[0]) - sinTheta * (pointToRotate[1] - centerPoint[1]);
		ReturnTo[1] = sinTheta * (pointToRotate[0] - centerPoint[0]) + cosTheta * (pointToRotate[1] - centerPoint[1]);

		ReturnTo[0] += centerPoint[0];
		ReturnTo[1] += centerPoint[1];
	}


	void RenderSkeleton(rust::BasePlayer* Player, BoneList Bone1, BoneList Bone2)
	{
		Vector2 Screen1, Screen2;
		rust::Transform* BoneStarta = Player->get_bone_transform(Bone1);
		rust::Transform* BoneEnda = Player->get_bone_transform(Bone1);
		Vector3 BoneStart = BoneStarta->get_position();
		Vector3 BoneEnd = BoneEnda->get_position();
		functions::WorldToScreen(BoneStart, Screen1);
		functions::WorldToScreen(BoneEnd, Screen2);

		functions::line({ Screen2.x,Screen2.y }, { Screen1.x, Screen1.y }, { 255,0,155,255 });

	}


	void radar()
	{
		//if (options::radar)
		{
			//if (options::radar)
			{
				float x = 200, y = 200;
				float resx = 200, resy = 200;
				float screenPos[2];
				float RotatedPoint[2];
				float centerx = x + (resx / 2), centery = 300;
				float centerWindowPos[2] = { centerx, centery };

				functions::fill_box({ x, y, resx, resy }, { 0.2f, 0.2f, 0.2f, 0.2f });

				for (int i = 0; i < variables::player_list.size(); i++)
				{
					rust::BasePlayer* player = variables::player_list.at(i);

					// if (player->is_knocked())
					  //   color = { 1,0,0,1 };
					 //if (player->get_model()->is_npc())
					   //  color = { 0.9, 0, 1, 1 };
					 //if (player->is_same_team(variables::local_player))
					   //  color = { 0, 1, 0, 1 };
					 //if (player->is_sleeping())
					   //      
					static int cases = 0;
					static float r = 1.00f, g = 0.00f, b = 1.00f;
					switch (cases)
					{
					case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
					case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
					case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
					case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
					default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
					}
					Vector4 color = { 1,0,0,1 };

					screenPos[0] = variables::local_position.x - player->get_bone_position(BoneList::spine1).x;
					screenPos[1] = variables::local_position.z - player->get_bone_position(BoneList::spine1).z;
					screenPos[0] *= 1;
					screenPos[1] *= 1;
					screenPos[0] *= -1;
					screenPos[0] += centerx;
					screenPos[1] += centery;

					RotatePoint(screenPos, centerWindowPos, 360.0f - variables::local_player->get_input()->get_view_angles().y, RotatedPoint);

					if (RotatedPoint[0] < x) continue;
					if (RotatedPoint[1] < y) continue;
					if (RotatedPoint[0] > x + resx) continue;
					if (RotatedPoint[1] > y + resy) continue;

					functions::fill_box({ RotatedPoint[0] - 1, RotatedPoint[1] - 1, 4, 4 }, { 0,0,0,1 });
					functions::fill_box({ RotatedPoint[0], RotatedPoint[1], 2, 2 }, color);
				}
				functions::fill_box({ centerx, centery, 2, 2 }, { 1,0.5,0,1 });
			}
		}
	}
	uintptr_t shader;

	void do_chams(rust::BasePlayer* player)
	{
		// variables::local_player->get_modelstate()->add_flag(16);
		 // do_handchams();
		if (!variables::asset_bundle)
			variables::asset_bundle = functions::load_bundle_file("C:\\assets.saph");
		apple:
		if (!variables::asset_bundle)
		{
			MessageBox(NULL, "Fuck", "Fuck", MB_OK);
			variables::asset_bundle = functions::load_bundle_file("C:\assets.saph");
			goto apple;
			return;
		}


		auto multiMesh = player->get_model()->get_multimesh();
		auto ourmesh = variables::local_player->get_model()->get_multimesh();

		if (!multiMesh)
			return;

		auto renderers = functions::get2renderers(multiMesh);
		auto ourrenders = functions::get2renderers(ourmesh);
		if (!renderers)
			return;

		for (int i = 0; i < renderers->get_size(); i++)
		{
			auto renderer = renderers->get_value(i);

			if (!renderer)
				return;

			const auto material = functions::get_material(renderer);
			if (!material)
				return;


			if (!shader)
				shader = functions::load_asset(variables::asset_bundle, "flatshader.shader", il2cpp::type_object("UnityEngine", "Shader")); // there are two flatshader.shader and shader.shader
			if (functions::get_shader(material) == shader)
				return;

			functions::set_shader(material, shader);
			static int cases = 0;
			static float r = 1.00f, g = 0.00f, b = 1.00f;
			switch (cases)
			{
			case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
			case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
			case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
			case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
			default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
			}


			functions::set_color(material, "ColorBehind", { 255.0f, 0,0, 0.2 });
			functions::set_color(material, "ColorVisible", { r, g, b, 0.2 });
		}
	}

	template <typename T>
	void write(uintptr_t address, T data) {
		if (address > 0x40000 && (address + sizeof(T)) < 0x7FFFFFFF0000)
			*(T*)(address) = data;
	}
	void fov(float fov)
	{

		uintptr_t Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>();

		auto klass2 = *reinterpret_cast<uintptr_t*>(Vbase + 0x2FF2728); // convar_admin_c*
		//auto Op = *reinterpret_cast<uintptr_t*>(Vbase + 49981568); // convar_admin_c*

		auto static_fields2 = *reinterpret_cast<uintptr_t*>(klass2 + 0xb8); 
		//auto fos = *reinterpret_cast<uintptr_t*>(Op + 0xb8);
	//	write<float>(fos + 0X18, 130.0f);
		write<float>(static_fields2 + 0x0, 12.0f);
	}


	void NoSway()
	{
		variables::local_player->get_held_item()->set_aim_cone(-1.0f);
		variables::local_player->get_held_item()->set_aim_cone_curve_scale(-1.0f);
		variables::local_player->get_held_item()->set_aim_cone_penalty_max(-1.0f);
		variables::local_player->get_held_item()->set_aim_cone_penalty_per_shot(-1.0f);
		variables::local_player->get_held_item()->set_aim_sway(-1.0f);
		variables::local_player->get_held_item()->set_aim_sway_speed(-1.0f);
	//	variables::local_player->get_held_item()->CrazyGun();
		//variables::local_player->get_held_item()->set_automatic(true);
	}

	float flyhack_forgiveness_interia = 10.0f;
	float flyhack_forgiveness = 1.5f;
	float flyhackDistanceVertical = 110.f;
	float flyhack_extrusion = 1.85f;
	float flyhackDistanceHorizontal = 110.f;
	float flyhackPauseTime{};


	float VFlyhack = 0.0f;
	float HFlyhack = 0.0f;
	float VMaxFlyhack = 0.0f;
	float HMaxFlyhack = 0.0f;

	void FlyHack()
	{
		variables::local_player->get_movement()->flying();
	}
	void flyhackbar()
	{
		rust::BasePlayer* plly = variables::local_player;
		float flyhack_forgiveness_interia = 10.0f;
		float flyhack_forgiveness = 1.5f;
		float flyhack_extrusion = 1.85f;
		bool flag = false;
		bool isInAir = false;
		Vector3 vector = (plly->get_lastSentTick()->get_position() + plly->get_transform()->get_position()) * 0.5f;
		flyhackPauseTime = functions::mathf_max(0.f, flyhackPauseTime - functions::get_deltaTime());

		if (plly) {
			float radius = plly->get_radius();
			float height = plly->get_height(false);

			Vector3 vector2 = vector + Vector3(0.0f, radius - flyhack_extrusion, 0.0f);
			Vector3 vector3 = vector + Vector3(0.0f, height - radius, 0.0f);
			float radius2 = radius - 0.05f;

			isInAir = plly->get_movement()->flying();

			if (isInAir)
			{
				Vector3 vector4 = (plly->get_lastSentTick()->get_position() - plly->get_transform()->get_position());

				float num2 = functions::mathf_abs(vector4.y);
				float num3 = sqrt((vector4.x * vector4.x) + (vector4.y * vector4.y) + (vector4.z + vector4.z));

				if (vector4.y >= 0.0f)
				{
					flyhackDistanceVertical += vector4.y;
					flag = true;
				}
				if (num2 < num3)
				{
					flyhackDistanceHorizontal += num3;
					flag = true;
				}
				if (flag)
				{
					float num5 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
					float num6 = (plly->get_jumpheight() + num5);
					if (flyhackDistanceVertical > num6) {
						//std::cout << OBFUSCATE_STR("Got Flyhacked") << std::endl;
					}
					float num7 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
					float num8 = (5.f + num7);
					if (flyhackDistanceHorizontal > num8) {
						//std::cout << OBFUSCATE_STR("Got Flyhacked") << std::endl;
					}
				}
			}




			else {
				flyhackDistanceHorizontal = 0.f;
				flyhackDistanceVertical = 0.f;
			}
			float num5 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
			float num6 = ((plly->get_jumpheight() + num5) * 3);
			//num6 = 13.0f;
			VMaxFlyhack = num6;//(num6 * 2) - 2;
			if (flyhackDistanceVertical <= (num6)) {
				VFlyhack = flyhackDistanceVertical;
			}
			if (VFlyhack >= VMaxFlyhack)
				VFlyhack = VMaxFlyhack;
			float num7 = functions::mathf_max((flyhackPauseTime > 0.0f) ? flyhack_forgiveness_interia : flyhack_forgiveness, 0.0f);
			float num8 = ((5.f + num7) * 3);
			//num8 = 26.0f;
			HMaxFlyhack = num8;//(num8 * 2) - 2; //- (VFlyhack * 2);//(num6 * 2);
			if (flyhackDistanceHorizontal <= (num8)) {
				HFlyhack = flyhackDistanceHorizontal;// +(VFlyhack * 2);
			}
			if (HFlyhack >= HMaxFlyhack)
				HFlyhack = HMaxFlyhack;

		}
		else {
			flyhackDistanceHorizontal = 0.f;
			flyhackDistanceVertical = 0.f;
		}
		if (!isInAir)
		{
			flyhackDistanceHorizontal = 0.f;
			flyhackDistanceVertical = 0.f;
		}


		float v, h;
		v = VFlyhack / (VMaxFlyhack - 1.0f);
		h = HFlyhack / (HMaxFlyhack - 1.0f);

		static int cases = 0;
		static float r = 1.00f, g = 0.00f, b = 1.00f;
		switch (cases)
		{
		case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
		case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
		case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
		case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
		default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
		}
		functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 870, 250, 10), Vector4(0, 0, 0, 0.9));
		functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 850, 250, 10), Vector4(0, 0, 0, 0.9));
		functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 870, 250 * (v >= 1.0f ? 1.0f : v), 10), Vector4(r, g, b, 1));
		functions::fill_box(Vector4((functions::get_screen_width() / 2) - 125, (functions::get_screen_height() / 2) - 850, 250 * (h >= 1.0f ? 1.0f : h), 10), Vector4(r, g, b, 1));




	}


	void aimbot(rust::BasePlayer* player)
	{
		//for (int i = 0; i < variables::player_list.size(); i++) {
		  //  auto player = variables::player_list[i];

			//auto Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>(); //GetModuleHandleA(xorstr_("GameAssembly.dll"));

			//	functions::set_draw_color(Vector4(1, 1, 1, 1));

			//bool is_npc = false;
			//	if (model->is_npc())
			//	return;
			//	else
			//	is_npc = true;
		int espas;

		if (player == variables::local_player)
			return;

		//variables::local_player->get_movement()->fun();
		//variables::local_player->get_movement()->spiderman();

		auto bounds = get_bounds(player, 2.f);
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto half = (bounds.right - bounds.left) / 2;
		Vector2 HeadPos;
		Vector2 NeckPos;
		Vector2 PelvisPos;
		Vector2 LegLPos;
		Vector2 LegRPos;

		Vector3 Neck = player->get_bone_position(BoneList::neck);
		Vector3 Chest = player->get_bone_position(BoneList::BoobCensor);
		Vector3 Penis = player->get_bone_position(BoneList::penis);
		Vector3 Lm = player->get_bone_position(BoneList::l_middle1);
		Vector3 Lmm = player->get_bone_position(BoneList::l_middle2);
		Vector3 Lmmm = player->get_bone_position(BoneList::l_middle3);

		//aimbot::getClosestBoneToCrosshair(player);
		//aimbot::getBestPlayerByFov();
		//aimbot::run();
		//rust::BaseProjectile a = variables::local_player->get_held_item();
		circle(functions::get_screen_width() / 2, functions::get_screen_height() / 2, 150.0f, 85.0f, { 0.0f, 255.0f, 350.0f, 255.0f });
		if (bounds.onscreen) {
			auto a = aimbot::getBestPlayerByFov();
			//   aimbot::draw();

			 //  functions::line(Vector2(functions::get_screen_width() / 2, functions::get_screen_height() / 2 ), Vector2( a->get_bone_position(BoneList::neck).x, a->get_bone_position(BoneList::neck).y), Vector4(1, 1, 1, 1));

			if (functions::get_key(KeyCode::Mouse1)) {
				aimbot::run(0.0f);
				// esp::render_hotbar(aimbot::getBestPlayerByFov());
			}
			else {
				espas = 0;
			}


		}
	}
	void PSlient(rust::BasePlayer* player)
	{
		//for (int i = 0; i < variables::player_list.size(); i++) {
		  //  auto player = variables::player_list[i];

			//auto Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>(); //GetModuleHandleA(xorstr_("GameAssembly.dll"));

			//	functions::set_draw_color(Vector4(1, 1, 1, 1));

			//bool is_npc = false;
			//	if (model->is_npc())
			//	return;
			//	else
			//	is_npc = true;
		int espas;

		if (player == variables::local_player)
			return;

		//variables::local_player->get_movement()->fun();
		//variables::local_player->get_movement()->spiderman();

		auto bounds = get_bounds(player, 2.f);
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto half = (bounds.right - bounds.left) / 2;
		Vector2 HeadPos;
		Vector2 NeckPos;
		Vector2 PelvisPos;
		Vector2 LegLPos;
		Vector2 LegRPos;

		Vector3 Neck = player->get_bone_position(BoneList::neck);
		Vector3 Chest = player->get_bone_position(BoneList::BoobCensor);
		Vector3 Penis = player->get_bone_position(BoneList::penis);
		Vector3 Lm = player->get_bone_position(BoneList::l_middle1);
		Vector3 Lmm = player->get_bone_position(BoneList::l_middle2);
		Vector3 Lmmm = player->get_bone_position(BoneList::l_middle3);

		//aimbot::getClosestBoneToCrosshair(player);
		//aimbot::getBestPlayerByFov();
		//aimbot::run();
		//rust::BaseProjectile a = variables::local_player->get_held_item();
		circle(functions::get_screen_width() / 2, functions::get_screen_height() / 2, 150.0f, 250.0f, { 0.0f, 255.0f, 350.0f, 255.0f });
		if (bounds.onscreen) {
			auto a = aimbot::getBestPlayerByFov();
			//   aimbot::draw();

			 //  functions::line(Vector2(functions::get_screen_width() / 2, functions::get_screen_height() / 2 ), Vector2( a->get_bone_position(BoneList::neck).x, a->get_bone_position(BoneList::neck).y), Vector4(1, 1, 1, 1));

			if (functions::get_key(KeyCode::Mouse1)) {
				aimbot::PSilent(0.0f);
				// esp::render_hotbar(aimbot::getBestPlayerByFov());
			}
			else {
				espas = 0;
			}


		}
	}
	void adman()
	{
		variables::local_player->add_flag(4);
	}

	void render_hotbar(rust::BasePlayer* player)
	{


		auto playera = aimbot::hotbar();
		if (!playera)
			return;

		auto belt = aimbot::hotbar()->aget_hotbar_items();
		if (belt.empty()) return;

		if (!aimbot::hotbar()->get_held_item()->get_definition_id()) return;
		auto distancea = Math::Calc3D_Dist(variables::local_position, playera->get_model()->get_position());

		if (belt.empty())
			return;
		if (!belt.empty())
		{
			float info_y = 0;
			for (int i = 0; i < 6; i++)
			{

				
				functions::fill_box({ 1177 + info_y, 180, 75, 75 }, { 0.1, 0.1, 0.1, 0.0 });
				functions::outline_box(Vector2(1177 + info_y, 180), Vector2(75, 75), Vector4(0.0f, 0.0f, 0.0f, 0.0));

				if (i > belt.size() - 1)
				{
					info_y += 95;
					continue;
				}

				auto item = belt.at(i);
				if (!item)
					return;

				if (item != NULL)
				{
					auto weapon_name = functions::ws2s(item->get_name());

					if (!weapon_name.empty())
					{
						auto sprite_the_drink = item->get_sprite();
						if (!sprite_the_drink)
							continue;

						auto texture = functions::get_texture(sprite_the_drink);
						if (!texture)
							continue;


						functions::set_draw_color({ 1, 1, 1, 1 });
						functions::draw_texture({ 1177 + info_y, 180, 75, 75 }, texture);
						info_y += 90;
					}
				}

			}
			info_y = 0;
		}
	}

	void render_Health(rust::BasePlayer* player) {
		if (player == variables::local_player)
			return;

		auto bounds = get_bounds(player, 2.f);
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto half = (bounds.right - bounds.left) / 2;
		Vector2 HeadPos;
		Vector2 NeckPos;
		Vector2 PelvisPos;
		Vector2 LegLPos;
		Vector2 LegRPos;

		auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());


		if (bounds.onscreen) {

			auto health = Vector4(bounds.left - 5, bounds.top + (box_height - box_height * (player->get_health() / 100)), 3, box_height * (player->get_health() / 100));
			const auto health_bg = Vector4(bounds.left - 6, bounds.top, 5, box_height);

			auto health2 = Vector4(bounds.left, bounds.bottom, box_width * (player->get_health() / 100), 3);
			const auto health_bg2 = Vector4(bounds.left, bounds.bottom, box_width, 5);

			//functions::fill_box(health_bg, Vector4(0, 0, 0, 1));
			functions::fill_box(health_bg2, Vector4(0, 0, 0, 1));
			//functions::fill_box(health, Vector4(0.2, 0.8, 0, 1));
			functions::fill_box(health2, Vector4(0.2, 0.8, 0, 1));
		}
	}
	void TextBone(rust::BasePlayer* Player, BoneList Bone, std::string label, Vector4 Color)
	{
		Vector3 BonePos = Player->get_bone_position(Bone);
		Vector2 Pos;
		functions::WorldToScreen(BonePos, Pos);
		functions::label(Vector4(Pos.x, Pos.y, 150, 20), label, Color);
	}
	void render_box(rust::BasePlayer* player) {
		//for (int i = 0; i < variables::player_list.size(); i++) {
		  //  auto player = variables::player_list[i];

		if (player == variables::local_player)
			return;

		auto bounds = get_bounds(player, 2.f);
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto half = (bounds.right - bounds.left) / 2;
		Vector2 HeadPos;
		Vector2 NeckPos;
		Vector2 PelvisPos;
		Vector2 LegLPos;
		Vector2 LegRPos;

		auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());


		if (bounds.onscreen) {
			float resp, gesp, besp;
			resp = 0.0f;
			gesp = 1.0f;
			besp = 0.2f;

			if (player->get_model()->is_npc()) {
				resp = 0.5f;
				gesp = 0.2f;
				besp = 0.0f;
			}
			if (player->is_knocked()) {
				resp = 1.0f;
				gesp = 0.0f;
				besp = 0.0f;
			}

			if (player->is_sleeping()) {
				resp = 0.4f;
				gesp = 0.0f;
				besp = 1.0f;
			}
			Vector3 vis = player->get_model()->get_position();
			if (player->is_visible(vis)) {
				resp = 0.3f;
				gesp = 0.2f;
				besp = 0.9f;
			}

			functions::outline_box({
			  bounds.left - 1,
			  bounds.top - 1
				}, {
				  box_width + 2,
				  box_height + 2
				}, Vector4(0, 0, 0, 1), 1);
			functions::outline_box({
			  bounds.left,
			  bounds.top
				}, {
				  box_width,
				  box_height
				}, Vector4(1, 1, 1, 1), 1);


			//TextBone(player, BoneList::head, "Head", Vector4(255, 0, 0, 255));
			//TextBone(player, BoneList::spine1, "Spine", Vector4(255, 0, 0, 255));
			//TextBone(player, BoneList::r_hip, "R_Hip", Vector4(255, 0, 0, 255));
			//TextBone(player, BoneList::l_hip, "L_Hip", Vector4(255, 0, 0, 255));
			//TextBone(player, BoneList::r_forearm, "R_Arm", Vector4(255, 0, 0, 255));
			//TextBone(player, BoneList::l_forearm, "L_Arm", Vector4(255, 0, 0, 255));
			functions::fill_box({ bounds.left - 1, bounds.top - 1, box_width + 2, box_height + 2 }, Vector4(0.1, 0.1, 0.1, 0.3f));
			//	rust::TOD_NightParameters* Sky;
			//	Sky->set_ambient_multiplier(25.0f);
			//	Sky->set_light_intensity(0.0f);
				//rust::HitInfo* Block;
				//Vector3 World = Block->get_hit_position_world();
				//functions::ddraw_arrow(World, variables::local_position, 2.0f, Vector4(255, 0, 0, 0));

			float localplayerHealth = variables::local_player->get_health();
			functions::fill_box(Vector4(functions::get_screen_width() / 2, functions::get_screen_height() / 3.5, 200, 500), Vector4(0, 0, 0, 0));
			functions::fill_box(Vector4(functions::get_screen_width() / 2, functions::get_screen_height() / 3.5, 200, (5 * localplayerHealth)), Vector4(0, 0, 0, 0));

			//	functions::set_


			//(12.0f);
		}
	}
	void render_name(rust::BasePlayer* player) {
		//for (int i = 0; i < variables::player_list.size(); i++) {
		  //  auto player = variables::player_list[i];

		if (player == variables::local_player)
			return;

		auto bounds = get_bounds(player, 2.f);
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto half = (bounds.right - bounds.left) / 2;
		Vector2 HeadPos;
		Vector2 NeckPos;
		Vector2 PelvisPos;
		Vector2 LegLPos;
		Vector2 LegRPos;

		auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());


		if (bounds.onscreen) {
			float resp, gesp, besp;
			resp = 0.0f;
			gesp = 1.0f;
			besp = 0.2f;

			if (player->get_model()->is_npc()) {
				resp = 0.5f;
				gesp = 0.2f;
				besp = 0.0f;
			}
			if (player->is_knocked()) {
				resp = 1.0f;
				gesp = 0.0f;
				besp = 0.0f;
			}

			if (player->is_sleeping()) {
				resp = 0.4f;
				gesp = 0.0f;
				besp = 1.0f;
			}
			Vector3 vis = player->get_model()->get_position();
			if (player->is_visible(vis)) {
				resp = 0.3f;
				gesp = 0.2f;
				besp = 0.9f;
			}

			auto name = player->get_username();
			std::string wcs = functions::ws2s(name);
			functions::outline(Vector4{
			  bounds.left - 80.f, bounds.bottom + 5, 79.f + half * 2 + 80.f, 13
				}, wcs, Vector4(0, 0, 0, 1), true, 10);
			functions::label(Vector4{
			  bounds.left - 80.f, bounds.bottom + 5, 79.f + half * 2 + 80.f, 13
				}, wcs, {
				  1,
				  1,
				  1,
				  1
				}, true, 10);


		}

		//	functions::set_timescale(12.0f);

	}
	void render_distance(rust::BasePlayer* player) {
		// for (int i = 0; i < variables::player_list.size(); i++) {
		 //    auto player = variables::player_list[i];

			 //auto Vbase = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>(); //GetModuleHandleA(xorstr_("GameAssembly.dll"));

			 //	functions::set_draw_color(Vector4(1, 1, 1, 1));

			 //bool is_npc = false;
			 //	if (model->is_npc())
			 //	return;
			 //	else
			 //	is_npc = true;
		int espas;

		if (player == variables::local_player)
			return;

		auto bounds = get_bounds(player, 2.f);
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto half = (bounds.right - bounds.left) / 2;
		Vector2 HeadPos;
		Vector2 NeckPos;
		Vector2 PelvisPos;
		Vector2 LegLPos;
		Vector2 LegRPos;

		auto distancea = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());

		if (bounds.onscreen) {

			std::string playerdistance = "[ " + std::to_string(int(distancea)) + "m ]";

			functions::outline(Vector4{
			  bounds.left - 80.f, bounds.bottom + 25, 79.f + half * 2 + 80.f, 13
				}, playerdistance, Vector4(0, 0, 0, 0), true, 10);
			functions::label(Vector4{
			  bounds.left - 80.f, bounds.bottom + 25, 79.f + half * 2 + 80.f, 13
				}, playerdistance, {
				  1,
				  1,
				  1,
				  1
				}, true, 10);


		}


		// }
	}


}