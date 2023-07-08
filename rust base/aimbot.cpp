#include "aimbot.h"
float length;
namespace aimbot {
	rust::BasePlayer* getBestPlayerByFov() {
		rust::BasePlayer* bestEnt = nullptr;
		int closest = INT_MAX;
		Vector2 curAngles = variables::local_player->get_input()->get_view_angles();
		for (int i = 0; i < variables::player_list.size(); i++) {
			Vector2 anglesTo = Math::CalcAngle(variables::local_player->get_bone_position(BoneList::head), variables::player_list[i]->get_bone_position(BoneList::head));
			int difx = anglesTo.x - curAngles.x;
			int dify = anglesTo.y - curAngles.y;
			difx = Math::abs(difx);
			dify = Math::abs(dify);
			Vector3 headPos = variables::player_list[i]->get_model()->get_position();
			Vector2 head2D;
			if (functions::WorldToScreen(headPos, head2D) == false) continue;
			 length = sqrt(pow((variables::screen_width / 2) - head2D.x, 2) + pow((variables::screen_height / 2) - head2D.y, 2));
			 if (headPos == Vector3{ 0, 0, 0 }) continue;
			int average = (difx + dify) / 2;
			if (average < closest && length <= 250.0f) {
				closest = average;
				bestEnt = variables::player_list[i];
				functions::get_mouse_pos();
			}
		}
		return bestEnt;
	}

	int getClosestBoneToCrosshair(rust::BasePlayer* player) {
		int bestBone = -1;
		int closest = INT_MAX;
		Vector2 curAngles = variables::local_player->get_input()->get_view_angles();
		for (int i = 0; i < rust::valid_bones.size(); i++) {
			Vector2 anglesTo = Math::CalcAngle(variables::local_player->get_bone_position(BoneList::head), player->get_bone_position(rust::valid_bones[i]));
			int difx = anglesTo.x - curAngles.x;
			int dify = anglesTo.y - curAngles.y;
			difx = Math::abs(difx);
			dify = Math::abs(dify);
			int average = (difx + dify) / 2;
			if (average < closest) {
				closest = average;
				bestBone = rust::valid_bones[i];
			}
		}
		return bestBone;
	}
	Vector3 sim_prediction(const Vector3& from, int bone, float initial_velocity, float gravity_modifier, float drag, float height)
	{
		if (variables::local_player->get_held_item()->is_melee())
		{
			return Vector3{ 0,0,0 };
		}

		rust::BasePlayer* who = getBestPlayerByFov();

		Vector3 aimpoint = who->get_bone_position(48);
		float distance = Math::Calc3D_Dist(from, aimpoint);
		auto held_weapon = variables::local_player->get_held_item();
		if (!held_weapon)
			return aimpoint;

		if (!held_weapon->is_gun())
			return aimpoint;

		auto weapon_data = held_weapon->get_weapon_data();

		float m_flBulletSpeed = initial_velocity;
		float bullet_gravity = gravity_modifier;
		float bullet_time = distance / m_flBulletSpeed;
		float m_fDrag = drag;

		const float m_flTimeStep = (0.015625f);
		float YTravelled{}, m_flYSpeed{}, m_flBulletTime{}, m_flDivider{};



		for (float distance_to_travel = 0.f; distance_to_travel < distance;)
		{
			float speed_modifier = 1.f - m_flTimeStep * m_fDrag;
			m_flBulletSpeed *= speed_modifier;

			if (m_flBulletSpeed <= 0.f || m_flBulletSpeed >= 10000.f || YTravelled >= 10000.f || YTravelled < 0.f)
				break;


			if (m_flBulletTime > 8.f)
				break;

			m_flYSpeed += ((9.81f) * bullet_gravity) * m_flTimeStep;
			m_flYSpeed *= speed_modifier;

			distance_to_travel += m_flBulletSpeed * m_flTimeStep;
			YTravelled += m_flYSpeed * m_flTimeStep;
			m_flBulletTime += m_flTimeStep;
		}

		Vector3 velocity = who->get_model()->get_velocity() * 0.75f;
		//if (velocity.y > 0.f)
		//    velocity.y /= 3.25f;

		aimpoint.y += YTravelled;
		aimpoint += velocity * m_flBulletTime;

		return aimpoint;
	}


	rust::BasePlayer* hotbar() {
		rust::BasePlayer* bestEnt = nullptr;
		float closest = 999999999;

		for (int i = 0; i < variables::player_list.size(); i++) {
			auto model = variables::player_list[i]->get_model();


			auto distancea = Math::Calc3D_Dist(variables::local_position, variables::player_list[i]->get_model()->get_position());

			if (variables::player_list[i] == variables::local_player)
				continue;
			if (variables::player_list[i]->get_life_state())
				continue;
			if (variables::player_list[i]->is_sleeping())
				continue;
			Vector3 headPos = variables::player_list[i]->get_model()->get_position();
			Vector2 head2D;
			if (headPos == Vector3{ 0, 0, 0 }) continue;
			if (functions::WorldToScreen(headPos, head2D) == false) continue;
			float length = sqrt(pow((variables::screen_width / 2) - head2D.x, 2) + pow((variables::screen_height / 2) - head2D.y, 2));

			float height = 50;

			if (length < closest && length <= 50) {
				closest = length;
				bestEnt = variables::player_list[i];
			}
		}
		return bestEnt;
	}



	void run(float Fov) {
		rust::BasePlayer* player = getBestPlayerByFov();
		Vector3 Final = sim_prediction(variables::local_player->get_bone_position(BoneList::eyeTranform), BoneList::neck, variables::local_player->get_held_item()->get_weapon_data().initial_velocity, variables::local_player->get_held_item()->get_weapon_data().gravity_modifier, variables::local_player->get_held_item()->get_weapon_data().drag, 0.0f);


		//  Vector3 sim_prediction(const Vector3 & from, rust::BasePlayer * who, unsigned int bone, float initial_velocity, float gravity_modifier, float drag)
		int bone;
		
		if (player->get_bone_position(BoneList::neck) == Vector3{ 0, 0, 0 })
			return;
		// rust::Attack


		variables::local_player->get_input()->set_view_angles(Math::CalcAngle(variables::local_player->get_bone_position(BoneList::eyeTranform), Final));
	
	}

	void PSilent(float Fov) {
		rust::BasePlayer* player = getBestPlayerByFov();
		Vector3 Final = sim_prediction(variables::local_player->get_bone_position(BoneList::eyeTranform), BoneList::neck, variables::local_player->get_held_item()->get_weapon_data().initial_velocity, variables::local_player->get_held_item()->get_weapon_data().gravity_modifier, variables::local_player->get_held_item()->get_weapon_data().drag, 0.0f);

		//  Vector3 sim_prediction(const Vector3 & from, rust::BasePlayer * who, unsigned int bone, float initial_velocity, float gravity_modifier, float drag)
		int bone;

		if (player->get_bone_position(BoneList::neck) == Vector3{ 0, 0, 0 })
			return;
		// rust::Attack
		std::string XVa = "[ X : " + std::to_string(float(Final.x)) + " ]";
		std::string YVa = "[ Y : " + std::to_string(float(Final.y)) + " ]";

		functions::label(Vector4(functions::get_screen_width() / 2, functions::get_screen_height() / 2, 300, 40), XVa.c_str(), Vector4(255, 255, 255, 255));
		functions::label(Vector4(functions::get_screen_width() / 2, functions::get_screen_height() / 2 + 35, 300, 40), YVa.c_str(), Vector4(255, 255, 255, 255));

		std::string MouseX = "[ X : " + std::to_string(float(functions::get_mouse_pos().x)) + " ]";
		std::string MouseY = "[ Y : " + std::to_string(float((functions::get_mouse_pos().y))) + " ]";

		functions::label(Vector4(functions::get_screen_width() / 2, functions::get_screen_height() / 2 + 45, 300, 40), MouseX.c_str(), Vector4(255, 255, 255, 255));
		functions::label(Vector4(functions::get_screen_width() / 2, functions::get_screen_height() / 2 + 60, 300, 40), MouseY.c_str(), Vector4(255, 255, 255, 255));
		Vector3 Mouse = functions::get_mouse_pos();

		if (functions::get_key(KeyCode::Mouse0))
		{
		

		}

	}
}