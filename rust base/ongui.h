#pragma once
#include "hinclude.h"
#include "esp.h"
#include "aimbot.h"
#include "misc.hpp"
namespace visuals {
	bool box;
	bool health;
	bool name;
	bool distance;
	bool chams;
	bool hotbar;
	bool radar;
}
namespace Misc {
	bool ReducedRecoil;
	float RecoilAmount;
	bool AutoMatic;
	bool Instaekoa;
	float fov;
	bool bar;
	bool fly;
	bool fovads;
	bool adman;
}
namespace Combat {
	bool MemoryAim;
	bool psilent;
	float Fov;
}

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
void entity_loop()
{
	variables::client_entities = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
	std::cout << variables::client_entities << std::endl;

	if (!variables::client_entities)
	{
		rust::LocalPlayer::ConsoleMessage(xorstr_("!client_entities"));
		return;
	}

	auto entity_realm = *reinterpret_cast<uintptr_t*>(variables::client_entities + 0x10);
	if (!entity_realm)
	{
		return;
	}

	auto buffer_list = *reinterpret_cast<uintptr_t*>(entity_realm + 0x28);
	if (!buffer_list)
	{
		return;
	}

	auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);

	auto object_list = *reinterpret_cast<uintptr_t*>(buffer_list + 0x18);
	if (!object_list)
	{
		return;
	}

	std::vector<rust::BasePlayer*> tempList;

	for (int idx = 0; idx < object_list_size; idx++)
	{
		auto current_object = *reinterpret_cast<uintptr_t*>(object_list + (0x20 + (idx * sizeof(uint64_t))));
		if (!current_object)
		{
			continue;
		}

		auto object_unk = *reinterpret_cast<uintptr_t*>(current_object);
		if (!object_unk)
		{
			continue;
		}

		auto classname_ptr = *reinterpret_cast<uintptr_t*>(object_unk + 0x10);
		if (!classname_ptr)
		{
			continue;
		}

		auto base_object = *reinterpret_cast<uintptr_t*>(current_object + 0x10);
		if (!base_object)
		{
			continue;
		}

		auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
		if (!object)
		{
			continue;
		}

		if (idx == 0)
		{
			auto local_player_object_class = *reinterpret_cast<uintptr_t*>(object + 0x30);

			if (!local_player_object_class)
			{
				continue;
			}

			auto local_entity = *reinterpret_cast<uintptr_t*>(local_player_object_class + 0x18);

			if (!local_entity)
			{
				continue;
			}

			variables::local_player = *reinterpret_cast<rust::BasePlayer**>(local_entity + 0x28);

			if (variables::local_player->get_life_state())
			{
				continue;
			}

			variables::local_position = variables::local_player->get_visual_position(local_player_object_class);
			continue;
		}

		functions::update_view_matrix();

		const auto tag = *reinterpret_cast<std::uint16_t*>(object + 0x54);
		if (tag == 6)
		{
			auto object_class = *reinterpret_cast<uintptr_t*>(object + 0x30);
			if (!object_class)
				return;

			auto entity = *reinterpret_cast<uintptr_t*>(object_class + 0x18);
			if (!entity)
				return;

			auto player = *reinterpret_cast<rust::BasePlayer**>(entity + 0x28);
			if (!reinterpret_cast<uintptr_t>(player))
				return;

			tempList.push_back(player);
			if (visuals::box)
			{
				esp::render_box(player);

			}
			if (visuals::chams)
			{
				esp::do_chams(player);
			}
			if (visuals::radar)
			{
				esp::radar();

			}
			if (visuals::distance)
			{
				esp::render_distance(player);
			}
			if (visuals::health)
			{
				esp::render_Health(player);
			}
			if (visuals::name) {
				esp::render_name(player);
			}
			if (visuals::hotbar)
			{
				esp::render_hotbar(player);
			}

			

		

			
			if (Combat::MemoryAim)
			{
				esp::aimbot(player);
			}
				
		
			//esp::do_chams(player);
		}
		else
		{
			std::hash<std::string_view> char_hash;
			const char* class_name = reinterpret_cast<const char*>(classname_ptr);
			auto class_crc = char_hash(class_name);

			auto object_name = xorstr_("OreResourceEntity");
			auto object_crc = char_hash(std::string_view(object_name, functions::my_strlen(object_name)));


			continue;
		}
	}
	variables::player_list = tempList;
	return;
}

namespace ongui
{
	bool left;
	bool right;

	std::vector<std::string> combo = { "one", "two" };
	int comboLeftSelected;
	int comboRightSelected;

	float sliderFloatLeft;
	float sliderFloatRight;

	int sliderIntLeft;
	int sliderIntRight;

	inline void ongui_hook(void* instance)
	{
		auto skin = functions::get_draw_skin();
		variables::draw_label = *reinterpret_cast<uintptr_t*>(skin + 0x38);
		functions::set_draw_alignment(variables::draw_label, 0x0);
		functions::set_draw_font_size(variables::draw_label, 12);
		functions::set_draw_color(Vector4(1, 1, 1, 1));

		entity_loop();
		if (functions::get_key(KeyCode::F1) &0x1)
		{
			visuals::box = true;
		}
		if (functions::get_key(KeyCode::F2) &0x1)
		{
			visuals::health = true;
		}
		if (functions::get_key(KeyCode::F3) & 0x1)
		{
			visuals::radar = true;
		}
		if (functions::get_key(KeyCode::F4) &0x1)
		{
			visuals::name = true;
		}
		if (functions::get_key(KeyCode::F5) &0x1)
		{
			visuals::distance = true;
		}
		if (functions::get_key(KeyCode::F6) &0x1)
		{
			visuals::hotbar = true;
		}
		if (functions::get_key(KeyCode::F7) &0x1)
		{
			Misc::Instaekoa = true;
		}
		if (functions::get_key(KeyCode::F8) & 0x1)
		{
			Misc::ReducedRecoil = true;
		}
		if (functions::get_key(KeyCode::F9) &0x1)
		{
			Misc::adman = true;
		}
		if (functions::get_key(KeyCode::F10) & 0x1)
		{
			visuals::chams = true;
		}
		if (functions::get_key(KeyCode::F1) && functions::get_key(KeyCode::LeftShift))
		{
			Combat::MemoryAim = true;
		}
	
		

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

			functions::outline(Vector4(10, 10, 150, 20), xorstr_("Lite [Public]"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 10, 150, 20), xorstr_("Lite [Public]"), Vector4(r, g, b, 1));
			functions::fill_box(Vector4(10, 30, 65, 5), Vector4(r, g, b, 1));

			functions::outline(Vector4(10, 50, 150, 20), xorstr_("[F1] Box"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 50, 150, 20), xorstr_("[F1] Box"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 80, 150, 20), xorstr_("[F2] Health Bar"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 80, 150, 20), xorstr_("[F2] Health Bar"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 110, 150, 20), xorstr_("[F3] Radar"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 110, 150, 20), xorstr_("[F3] Radar"), Vector4(1, 1, 1, 1));


			functions::outline(Vector4(10, 140, 150, 20), xorstr_("[F4] Name"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 140, 150, 20), xorstr_("[F4] Name"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 170, 150, 20), xorstr_("[F5] Distance"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 170, 150, 20), xorstr_("[F5] Distance"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 200, 150, 20), xorstr_("[F6] Draw Items"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 200, 150, 20), xorstr_("[F6] Draw Items"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 230, 150, 20), xorstr_("[F7] No Sway"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 230, 150, 20), xorstr_("[F7] No Sway"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 260, 150, 20), xorstr_("[F8] No Recoil"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 260, 150, 20), xorstr_("[F8] No Recoil"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 290, 150, 20), xorstr_("[F9] Day Time"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 290, 150, 20), xorstr_("[F9] Day Time"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 320, 150, 20), xorstr_("[F10] Player Chams"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 320, 150, 20), xorstr_("[F10] Player Chams"), Vector4(1, 1, 1, 1));

			functions::outline(Vector4(10, 350, 300, 40), xorstr_("[F1 + Shift] Aimbot With Prediction [Right Mouse Button]"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 350, 300, 40), xorstr_("[F1 + Shift] Aimbot With Prediction [Right Mouse Button]"), Vector4(1, 1, 1, 1));

		    functions::outline(Vector4(10, 350, 300, 40), xorstr_("[F2 + Shift] PSilent With Prediction [Right Mouse Button]"), Vector4(0, 0, 0, 1));
			functions::label(Vector4(10, 350, 300, 40), xorstr_("[F2 + Shift] PSilent With Prediction [Right Mouse Button]"), Vector4(1, 1, 1, 1));

		

		if (Misc::ReducedRecoil)
		{
			misc::ReducedRecoil(0.0f);
		}
		if (Misc::Instaekoa)
		{
			esp::NoSway();
		}
		if (Misc::AutoMatic)
		{
			misc::AutomaticWeapon();
		}
		if (Misc::Instaekoa)
		{
			misc::InstaEoka();
		}
		

		return;
	}

	static bool wakeup{ true };
	uintptr_t performaceui_hook;
	void hk_performance_ui_update(void* instance)
	{
		if (wakeup)
		{
			auto game_object = il2cpp::methods::object_new(il2cpp::init_class(xorstr_("GameObject"), xorstr_("UnityEngine")));
			functions::create(game_object, xorstr_(""));
			functions::add_component(game_object, il2cpp::type_object(xorstr_(""), xorstr_("DevControls")));
			functions::dont_destroy_on_load(game_object);
			wakeup = false;
		}

		auto static update_performance_ui = reinterpret_cast<void(*)(void* instance)>(performaceui_hook);
		SpoofReturn(update_performance_ui, instance);
	}
}