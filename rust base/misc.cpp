#include "misc.hpp"


namespace misc
{
	void AdminEsp()
	{
		if (variables::local_player->get_health() == 0) return;
		variables::local_player->add_flag(4);
	}
	void ReducedRecoil(float x)
	{
		variables::local_player->get_held_item()->recoilControl(x);
	}
	void AutomaticWeapon()
	{
		variables::local_player->get_held_item()->set_automatic(true);
	}
	void InstaEoka()
	{
		variables::local_player->get_held_item()->set_always_eoka(-1.0f);
	}
}