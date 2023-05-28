#pragma once
#ifndef VENOM_HPP
#define VENOM_HPP

#include "hooks.hpp"
#include "globals.hpp"
#include "features.hpp"

#include "features/esp.hpp"
#include "features/aimbot.hpp"
#include "features/exploits.hpp"


#include <Windows.h>

namespace venom
{
	inline HMODULE handle;

	void init(HMODULE handle)
	{
		venom::handle = handle;
		globals::venom::moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

		hooks::load();
		menu::load();

		esp::load();
		aimbot::load();
		exploits::load();
		
		while (true)
		{
			if (GetAsyncKeyState(VK_END) & 1)
			{
				menu::unload();
				hooks::unload();
				break;
			}
		}

		FreeLibraryAndExitThread(venom::handle, 0);
	}
}
#endif

