#pragma once
#ifndef MENU_HPP
#define MENU_HPP

#include <Windows.h>
#include <gl/GL.h>

#include "../../ext/fonts/roboto.hpp"
#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_opengl2.h"
#include "globals.hpp"
#include "hooks.hpp"


typedef int(__cdecl t_SDL_ShowCursor)(int);
t_SDL_ShowCursor* o_SDL_ShowCursor = (t_SDL_ShowCursor*)GetProcAddress(GetModuleHandle(L"SDL.dll"), "SDL_ShowCursor");

namespace menu
{
	inline const char* name;
	inline bool ready;
	inline int keybind;
	inline ImFont* font;
	inline bool open;
	inline bool make = false;

	void load()
	{
		menu::name = "venom | ac internal";
		menu::ready = false;
		menu::keybind = VK_INSERT;
		menu::make = true;
	}

	void unload()
	{
		make = false;
		ready = false;
		open = false;
	
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplWin32_Shutdown();
		o_SDL_ShowCursor(0);
	}

	void setup()
	{
		globals::gl::menuContext = wglCreateContext(globals::gl::hdc);

		wglMakeCurrent(globals::gl::hdc, globals::gl::menuContext);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, globals::gl::viewPort[2], globals::gl::viewPort[3], 0, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0, 0, 0, 0);

		ImGui::CreateContext();

		ImGui::GetIO().Fonts->AddFontDefault();
		menu::font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)roboto, sizeof(roboto), 14);
		ImGui::GetIO().Fonts->Build();

		ImGui_ImplWin32_Init(globals::gl::hwnd);
		ImGui_ImplOpenGL2_Init();

		menu::ready = true;
	}

	void draw()
	{
		if (menu::open)
		{
			o_SDL_ShowCursor(1);
		
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 400,400 });
			ImGui::Begin(name);
			ImGui::PopStyleVar();

			ImGui::PushID("Menu");

			if (ImGui::BeginTabBar("Tabs"))
			{
				ImGui::PushID("Visual");
				if (ImGui::BeginTabItem("Visual"))
				{
					ImGui::Checkbox("2D Box ESP", &globals::features::esp::enabled);
					if (globals::features::esp::enabled)
					{
						ImGui::Separator();
						
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::Checkbox("Team", &globals::features::esp::team);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::Checkbox("Enemies", &globals::features::esp::enemy);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::Checkbox("Distance", &globals::features::esp::distance);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::Checkbox("Name", &globals::features::esp::name);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::ColorEdit4("Color Outline", globals::features::esp::colorOutline);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::ColorEdit4("Color Box Team", globals::features::esp::colorBoxTeam);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::ColorEdit4("Color Box Enemies", globals::features::esp::colorBoxEnemy);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::ColorEdit4("Color Text", globals::features::esp::colorText);
					}

					ImGui::Separator();

					ImGui::EndTabItem();
				}
				
				ImGui::PopID();

				ImGui::PushID("Combat");
				if (ImGui::BeginTabItem("Combat"))
				{
					ImGui::Checkbox("Draw FOV", &globals::features::aimbot::drawFov);
					
					if (globals::features::aimbot::drawFov)
					{
						ImGui::Separator();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::SliderInt("FOV Amount", &globals::features::aimbot::fov, globals::features::aimbot::MIN_FOV, globals::features::aimbot::MAX_FOV);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::ColorEdit4("FOV Color", globals::features::aimbot::colorFov);
					}
					ImGui::Separator();

					ImGui::Checkbox("Aimbot", &globals::features::aimbot::enabled);
					if (globals::features::aimbot::enabled)
					{
						ImGui::Separator();

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::Checkbox("Smoothing", &globals::features::aimbot::smoothing);

						if (globals::features::aimbot::smoothing)
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
							ImGui::SliderFloat("Amount", &globals::features::aimbot::smoothingAmount, 0.10f, 0.99f, "%.2f");
						}

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::Checkbox("Wallcheck", &globals::features::aimbot::wallCheck);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::ColorEdit4("Color Target Circle", globals::features::aimbot::colorTarget);
					}
					ImGui::Separator();

					ImGui::PushID("Magic Bullet");
					ImGui::Checkbox("Magic Bullet", &globals::features::magicBullet::enabled);
					if (globals::features::magicBullet::enabled)
					{
						ImGui::Separator();

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
						ImGui::Checkbox("Wallcheck", &globals::features::magicBullet::wallCheck);
					}
					ImGui::Separator();
					ImGui::PopID();

					ImGui::EndTabItem();
				}
				ImGui::PopID();


				ImGui::PushID("Exploits");

				if (ImGui::BeginTabItem("Exploits"))
				{
					ImGui::Checkbox("Freeze Health", &globals::features::exploits::freezeHealth);
					ImGui::Checkbox("Freeze Armor", &globals::features::exploits::freezeArmor);
					ImGui::Checkbox("Freeze Ammo", &globals::features::exploits::freezeAmmo);
					ImGui::Checkbox("No Recoil", &globals::features::exploits::noRecoil);
					ImGui::Checkbox("Rapid Fire", &globals::features::exploits::rapidFire);
				}

				ImGui::PopID();

				

				ImGui::EndTabBar();
			}

			ImGui::PopID();


			ImGui::End();
		}
		else {
			o_SDL_ShowCursor(0);
		}
	}
}
#endif