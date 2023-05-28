#pragma once
#ifndef AC_HPP
#define AC_HPP

#include "globals.hpp"
#include "../util/math.hpp"

struct traceresult {
	math::vec3 hit;
	bool collided;
};

namespace ac {
	namespace entity
	{
		uintptr_t* get(uintptr_t pointer)
		{
			return (uintptr_t*)(pointer);
		}

		uintptr_t* getLocalPlayer()
		{
			return (uintptr_t*)(globals::venom::moduleBase + globals::offsets::localPlayer);
		}

		int* getAmmo(uintptr_t entity)
		{
			return (int*)(*(uintptr_t*)(*(uintptr_t*)(entity + globals::offsets::currentWeapon) + globals::offsets::ammo));
		}

		int* getArmor(uintptr_t entity)
		{
			return (int*)(entity + globals::offsets::armor);
		}

		char* getName(uintptr_t entity)
		{
			return (char*)(entity + globals::offsets::name);
		}

		math::vec3* getAngles(uintptr_t entity)
		{
			return (math::vec3*)(entity + globals::offsets::angles);
		}

		math::vec3* getPosFeet(uintptr_t entity)
		{
			return (math::vec3*)(entity + globals::offsets::posFeet);
		}

		float* getHeight(uintptr_t entity)
		{
			return (float*)(entity + globals::offsets::playerHeight);
		}

		math::vec3* getPosHead(uintptr_t entity)
		{
			return (math::vec3*)(entity + globals::offsets::posHead);
		}

		int* getHealth(uintptr_t entity)
		{
			return (int*)(entity + globals::offsets::health);
		}

		int* getTeam(uintptr_t entity)
		{
			return (int*)(entity + globals::offsets::team);
		}

		bool isVisible(uintptr_t entity)
		{
			uintptr_t traceline = (uintptr_t)(globals::venom::moduleBase + globals::offsets::traceline);

			traceresult result;
			result.collided = false;
			
			uintptr_t localPlayer = *getLocalPlayer();

			math::vec3 from = *getPosHead(localPlayer);
			math::vec3 to = *getPosHead(entity);

			__asm {
				push 0
				push 0
				push localPlayer
				push to.z
				push to.y
				push to.x
				push from.z
				push from.y
				push from.x
				lea eax, [result]
				call traceline
				add esp, 36
			}

			return !result.collided;
		}
	}

	namespace world {
		uintptr_t* getEntityList()
		{
			return (uintptr_t*)(globals::venom::moduleBase + globals::offsets::entityList);
		}

		int* getPlayerCount()
		{
			return (int*)(globals::venom::moduleBase + globals::offsets::playerCount);
		}
	}
}

#endif