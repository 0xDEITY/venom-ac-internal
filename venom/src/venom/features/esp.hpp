#pragma once
#ifndef ESP_HPP
#define ESP_HPP

#include "../../../ext/imgui/imgui.h"
#include "../features.hpp"
#include "../ac.hpp"
#include "../globals.hpp"
#include "../../util/math.hpp"

namespace esp {

	void draw()
	{
		if (!globals::features::esp::enabled) return;

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		uintptr_t entityList = *ac::world::getEntityList();
		int playerCount = *ac::world::getPlayerCount();

		for (int i = 0; i < playerCount; i++)
		{
			uintptr_t player = *ac::entity::get(entityList + i * 0x4);

			if (player == 0) continue;

			int playerTeam = *ac::entity::getTeam(player);
			int myTeam = *ac::entity::getTeam(*ac::entity::getLocalPlayer());

			char* playerName = ac::entity::getName(player);
			char* myName = ac::entity::getName(*ac::entity::getLocalPlayer());

			if (!strcmp(playerName, myName)) continue;
			if (!globals::features::esp::team && playerTeam == myTeam) continue;
			if (!globals::features::esp::enemy && playerTeam != myTeam) continue;

			math::vec3 playerPos = *ac::entity::getPosFeet(player);
			math::vec3 myPos = *ac::entity::getPosFeet(*ac::entity::getLocalPlayer());

			math::vec3 visCheck = math::w2s(playerPos, globals::gl::viewPort, globals::gl::viewMatrix);
			if (visCheck.x == NULL) continue;

			math::vec3 points3d[8];

			for (int j = 0; j < 8; j++)
			{
				points3d[j] = playerPos;
			}

			float playerHeight = *ac::entity::getHeight(player);

			points3d[0].x -= globals::sizes::PLAYER_WIDTH / 2;
			points3d[0].y -= globals::sizes::PLAYER_WIDTH / 2;

			points3d[1].x -= globals::sizes::PLAYER_WIDTH / 2;
			points3d[1].y += globals::sizes::PLAYER_WIDTH / 2;

			points3d[2].x += globals::sizes::PLAYER_WIDTH / 2;
			points3d[2].y += globals::sizes::PLAYER_WIDTH / 2;

			points3d[3].x += globals::sizes::PLAYER_WIDTH / 2;
			points3d[3].y -= globals::sizes::PLAYER_WIDTH / 2;

			points3d[4].x -= globals::sizes::PLAYER_WIDTH / 2;
			points3d[4].y -= globals::sizes::PLAYER_WIDTH / 2;
			points3d[4].z += playerHeight + .5f;

			points3d[5].x -= globals::sizes::PLAYER_WIDTH / 2;
			points3d[5].y += globals::sizes::PLAYER_WIDTH / 2;
			points3d[5].z += playerHeight + .5f;

			points3d[6].x += globals::sizes::PLAYER_WIDTH / 2;
			points3d[6].y += globals::sizes::PLAYER_WIDTH / 2;
			points3d[6].z += playerHeight + .5f;

			points3d[7].x += globals::sizes::PLAYER_WIDTH / 2;
			points3d[7].y -= globals::sizes::PLAYER_WIDTH / 2;
			points3d[7].z += playerHeight + .5f;

			for (int j = 0; j < 8; j++)
			{
				points3d[j] = math::w2s(points3d[j], globals::gl::viewPort, globals::gl::viewMatrix);
			}

			float minX = points3d[0].x;
			float maxX = points3d[0].x;
			float minY = points3d[0].y;
			float maxY = points3d[0].y;

			bool skip = false;

			for (int j = 1; j < 8; j++)
			{
				const math::vec3 point = points3d[j];

				if (point.x == NAN)
				{
					skip = true;
					break;
				};

				if (minX > point.x) minX = point.x;
				if (maxX < point.x) maxX = point.x;
				if (minY > point.y) minY = point.y;
				if (maxY < point.y) maxY = point.y;
			}

			if (skip) continue;

			if (minX > -1000 && maxX < globals::gl::viewPort[2] + 1000 && minY > -1000 && maxY < globals::gl::viewPort[3] + 1000)
			{
				int health = math::clamp<int>(*ac::entity::getHealth(player), 0, globals::sizes::MAX_HEALTH);

				if (health == 0) continue;

				float healthPercentage = (float)health / globals::sizes::MAX_HEALTH;
				float healthWidth = math::clamp<float>(minX + (healthPercentage * (maxX - minX)), 0, minX + (healthPercentage * (maxX - minX)));

				int armor = math::clamp<int>(*ac::entity::getArmor(player), 0, globals::sizes::MAX_HEALTH);
				float armorPercentage = (float)armor / globals::sizes::MAX_HEALTH;
				float armorWidth = math::clamp<float>(minX + (armorPercentage * (maxX - minX)), 0, minX + (armorPercentage * (maxX - minX)));

				float distance = myPos.dist(playerPos);
				float distFactor = distance / 100;
				float fontSize = math::clamp<float>(globals::sizes::ESP_MIN_FONT_SIZE + (globals::sizes::ESP_MAX_FONT_SIZE - globals::sizes::ESP_MIN_FONT_SIZE) * (1 - distFactor), globals::sizes::ESP_MIN_FONT_SIZE, globals::sizes::ESP_MAX_FONT_SIZE);

				if (globals::features::esp::name)
				{
					// name
					drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX - 0.5, minY - 0.5 - fontSize - 1), math::ImColor4f(globals::features::esp::colorOutline), playerName);
					drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX - 0.5, minY + 0.5 - fontSize - 1), math::ImColor4f(globals::features::esp::colorOutline), playerName);
					drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX + 0.5, minY - 0.5 - fontSize - 1), math::ImColor4f(globals::features::esp::colorOutline), playerName);
					drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX + 0.5, minY + 0.5 - fontSize - 1), math::ImColor4f(globals::features::esp::colorOutline), playerName);
					drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX, minY - fontSize - 1), math::ImColor4f(globals::features::esp::colorText), playerName);
				}

				// box outline
				drawList->AddRect(ImVec2(minX - .5, minY - .5), ImVec2(maxX + .5, maxY + .5), math::ImColor4f(globals::features::esp::colorOutline), 0, 0, 1.5);

				// box
				drawList->AddRect(ImVec2(minX, minY), ImVec2(maxX, maxY), myTeam != playerTeam ? math::ImColor4f(globals::features::esp::colorBoxEnemy) : math::ImColor4f(globals::features::esp::colorBoxTeam), 0, 0, .5);

				// distance
				char distanceText[12]; sprintf_s(distanceText, "%.1fm", distance);

				if (health > 0)
				{
					// health bar outline
					drawList->AddRect(ImVec2(minX - .5, maxY + 2 - .5), ImVec2(maxX + .5, maxY + 3 + .5), math::ImColor4f(globals::features::esp::colorOutline), 0, 0, 1.5);

					// health bar
					drawList->AddRectFilled(ImVec2(minX, maxY + 2), ImVec2(healthWidth, maxY + 3), ImColor(255, 0, 0), 0, 0);

					if (armor <= 0)
					{
						if (globals::features::esp::distance)
						{
							drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX - 0.5, maxY - 0.5 + 4), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
							drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX - 0.5, maxY + 0.5 + 4), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
							drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX + 0.5, maxY - 0.5 + 4), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
							drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX + 0.5, maxY + 0.5 + 4), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
							drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX, maxY + 4), math::ImColor4f(globals::features::esp::colorText), distanceText);
						}
					}
				}

				if (armor > 0)
				{
					// shield bar outline
					drawList->AddRect(ImVec2(minX - .5, maxY + 5 - .5), ImVec2(maxX + .5, maxY + 6 + .5), math::ImColor4f(globals::features::esp::colorOutline), 0, 0, 1.5);

					// shield bar
					drawList->AddRectFilled(ImVec2(minX, maxY + 5), ImVec2(armorWidth, maxY + 6), ImColor(0, 0, 255), 0, 0);

					if (globals::features::esp::distance)
					{
						drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX - 0.5, maxY - 0.5 + 8), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
						drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX - 0.5, maxY + 0.5 + 8), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
						drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX + 0.5, maxY - 0.5 + 8), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
						drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX + 0.5, maxY + 0.5 + 8), math::ImColor4f(globals::features::esp::colorOutline), distanceText);
						drawList->AddText(ImGui::GetFont(), fontSize, ImVec2(minX, maxY + 8), math::ImColor4f(globals::features::esp::colorText), distanceText);
					}
				}
			}
		}
	}

	void load()
	{
		features::addDrawing(draw);
	}
}


#endif