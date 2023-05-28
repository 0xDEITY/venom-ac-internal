#pragma once
#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <Windows.h>

#include "../../ext/imgui/imgui.h"

namespace globals {
	inline char debugText[100];
	namespace venom {
		inline uintptr_t moduleBase;
	}

	namespace features {
		namespace magicBullet {
			inline bool enabled{ true };
			inline bool wallCheck{ false };
		}
		namespace esp {
			inline bool enabled{ true };
			inline bool team{ true };
			inline bool enemy{ true };
			inline bool distance{ true };
			inline bool name{ true };

			inline float colorOutline[4] { 0.f, 0.f, 0.f, 1.f};
			inline float colorBoxTeam[4]{ 0.f, 1.0f, 0.f, 1.f};
			inline float colorBoxEnemy[4]{ 1.0f, 0.f, 0.f, 1.f};
			inline float colorText[4]{ 1.f, 1.f, 1.f, 1.f };

		}

		namespace exploits {
			inline bool freezeHealth{ true };
			inline bool freezeAmmo{ true };
			inline bool freezeArmor{ true };

			inline bool rapidFire{ true };
			inline bool noRecoil{ true };
			inline bool rapidSet{ false };
			inline bool recoilSet{ false };
		}

		namespace aimbot {
			inline bool enabled{ true };
			inline int fov{ 45 };
			inline bool drawFov{ true };
			inline bool smoothing{ true };
			inline float smoothingAmount{ .5f };
			inline bool wallCheck{ true };

			inline float colorFov[4]{ 1.f, 1.f, 1.f, 1.f};
			inline float colorTarget[4]{ 1.f, 0.f, 0.f, 1.f};

			constexpr int MIN_FOV = 0;
			constexpr int MAX_FOV = 360;
		}
	}
	
	namespace sizes {
		constexpr float PLAYER_WIDTH = 2.5;
		constexpr float ESP_MAX_FONT_SIZE = 14;
		constexpr float ESP_MIN_FONT_SIZE = 6;
		constexpr int MAX_HEALTH = 100;
	}

	namespace gl {
		inline HDC hdc;
		inline HWND hwnd;
		inline HGLRC gameContext;
		inline HGLRC menuContext;

		inline int viewPort[4];
		inline float viewMatrix[16];
	}

	namespace offsets {
		constexpr uintptr_t currentWeapon = 0x374;
		constexpr uintptr_t ammo = 0x14;
		constexpr uintptr_t localPlayer = 0x10F4F4;
		constexpr uintptr_t posFeet = 0x34;
		constexpr uintptr_t posHead = 0x4;
		constexpr uintptr_t entityList = 0x10F4F8;
		constexpr uintptr_t playerCount = 0x10F500;
		constexpr uintptr_t health = 0xF8;
		constexpr uintptr_t team = 0x32C;
		constexpr uintptr_t viewMatrix = 0x101AE8;
		constexpr uintptr_t armor = 0xFC;
		constexpr uintptr_t angles = 0x40;
		constexpr uintptr_t name = 0x225;
		constexpr uintptr_t playerHeight = 0x5C;
		constexpr uintptr_t traceline = 0x8A310;
		constexpr uintptr_t shootBullet = 0x613B0;
		constexpr uintptr_t rapidFire = 0x637E4; // 89 0A
		constexpr uintptr_t noRecoil = 0x63786; // 50 8D 4C 24 1C 51 8B CE FF D2
	}
}
#endif