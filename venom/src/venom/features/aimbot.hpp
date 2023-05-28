#pragma once
#ifndef AIMBOT_HPP
#define AIMBOT_HPP

#include "../../../ext/imgui/imgui.h"
#include "../features.hpp"
#include "../ac.hpp"
#include "../globals.hpp"
#include "../../util/math.hpp"

namespace aimbot {
	uintptr_t getClosest()
	{
		float closestDistance = FLT_MAX;
		uintptr_t closestPlayer = 0;
		uintptr_t players = *ac::world::getEntityList();
		uintptr_t localPlayer = *ac::entity::getLocalPlayer();

		int playerCount = *ac::world::getPlayerCount();

		for (int i = 0; i < playerCount; i++)
		{
			uintptr_t player = *(uintptr_t*)(players + i * 4);

			if (player == 0) continue;

			char* playerName = ac::entity::getName(player);
			char* myName = ac::entity::getName(localPlayer);

			if (!strcmp(playerName, myName)) continue;

			if (*ac::entity::getHealth(player) <= 0) continue;
			if (*ac::entity::getTeam(player) == *ac::entity::getTeam(localPlayer)) continue;

			math::vec3 playerPos = *ac::entity::getPosFeet(player);
			math::vec3 myPos = *ac::entity::getPosFeet(localPlayer);

			float distance = myPos.dist(playerPos);

			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestPlayer = player;
			}
		}

		return closestPlayer;
	}
	uintptr_t getClosestToCursor(bool wallCheck)
	{
		float closestDistance = globals::features::aimbot::fov;
		uintptr_t closestPlayer = 0;
		uintptr_t players = *ac::world::getEntityList();
		uintptr_t localPlayer = *ac::entity::getLocalPlayer();

		int playerCount = *ac::world::getPlayerCount();

		for (int i = 0; i < playerCount; i++)
		{
			uintptr_t player = *(uintptr_t*)(players + i * 4);

			if (player == 0) continue;

			char* playerName = ac::entity::getName(player);
			char* myName = ac::entity::getName(localPlayer);

			if (!strcmp(playerName, myName)) continue;

			if (*ac::entity::getHealth(player) <= 0) continue;
			if (*ac::entity::getTeam(player) == *ac::entity::getTeam(localPlayer)) continue;
			if (wallCheck && !ac::entity::isVisible(player)) continue;

			math::vec3 playerPos = *ac::entity::getPosFeet(player);
			playerPos.z += *ac::entity::getHeight(player) - 1;

			math::vec3 myPos = *ac::entity::getPosFeet(localPlayer);

			math::vec3 screen = math::w2s(playerPos, globals::gl::viewPort, globals::gl::viewMatrix);

			float distanceToCursor = sqrt(pow(screen.x - globals::gl::viewPort[2] / 2, 2) + pow(screen.y - globals::gl::viewPort[3] / 2, 2));

			if (distanceToCursor < closestDistance)
			{
				closestDistance = distanceToCursor;
				closestPlayer = player;
			}
		}

		return closestPlayer;
	}

	void run()
	{
		if (!globals::features::aimbot::enabled) return;

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (globals::features::aimbot::drawFov)
			drawList->AddCircle(ImVec2(globals::gl::viewPort[2] / 2, globals::gl::viewPort[3] / 2), globals::features::aimbot::fov, math::ImColor4f(globals::features::aimbot::colorFov), globals::features::aimbot::fov, 1);

		if (!GetAsyncKeyState(VK_RBUTTON) && 1) return;

		uintptr_t closestPlayer = getClosestToCursor(globals::features::aimbot::wallCheck);

		if (closestPlayer == 0) return;

		uintptr_t localPlayer = *ac::entity::getLocalPlayer();

		math::vec3 targetPos = *ac::entity::getPosHead(closestPlayer);
		targetPos.z += .1;
		math::vec3* angles = ac::entity::getAngles(localPlayer);
		math::vec3 origin = *ac::entity::getPosHead(localPlayer);

		math::vec3 delta = { targetPos.x - origin.x, targetPos.y - origin.y, targetPos.z - origin.z };
		float distance = sqrt(pow(delta.x, 2) + pow(delta.y, 2) + pow(delta.z, 2));

		float yaw = (atan2(delta.y, delta.x) * (180 / math::pi)) + 90;
		float pitch = atan2(delta.z, distance) * 180 / math::pi;

		math::vec3 screen = math::w2s(targetPos, globals::gl::viewPort, globals::gl::viewMatrix);
		drawList->AddCircle(ImVec2(screen.x, screen.y), 10, math::ImColor4f(globals::features::aimbot::colorTarget), 30, 1);

		if (globals::features::aimbot::smoothing)
		{
			float newYaw = math::lerpYaw<float>(angles->x, yaw, 1.0f - globals::features::aimbot::smoothingAmount);
			float newPitch = math::lerp<float>(angles->y, pitch, 1.0f - globals::features::aimbot::smoothingAmount);

			*angles = math::vec3(newYaw, newPitch, 0);
		}
		else {
			*angles = math::vec3(yaw, pitch, 0);
		}
	}

	void load()
	{
		features::addDrawing(run);
	}
}

#endif