#pragma once
#include "../hinclude.h"
#include "../ongui.h"
#include "../baseplayer.h"
#include "../utils/lazy_importer.hpp"
namespace hooks
{
	__forceinline bool init()
	{
		ongui::performaceui_hook = *reinterpret_cast<uintptr_t*>(il2cpp::method(xorstr_("PerformanceUI"), xorstr_("Update"), 0, xorstr_("Facepunch")));
		il2cpp::hook(&ongui::hk_performance_ui_update, xorstr_("PerformanceUI"), xorstr_("Update"), 0, xorstr_("Facepunch"));
		il2cpp::hook(&ongui::ongui_hook, xorstr_("DevControls"), xorstr_("OnGUI"), 0, xorstr_(""));
		baseplayer::client_input_address = il2cpp::hook_virtual_function(xorstr_("BasePlayer"), xorstr_("ClientInput"), &baseplayer::client_input_hook, 1);

		return true;
	}
}