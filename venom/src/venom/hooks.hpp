#pragma once
#ifndef HOOKS_HPP
#define HOOKS_HPP

#include <Windows.h>
#include <gl/GL.h>
#include <iostream>
#include <windowsx.h>

#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_opengl2.h"
#include "../util/hook.hpp"
#include "menu.hpp"
#include "features/aimbot.hpp"
#include "ac.hpp"
#include "../util/math.hpp"
#include "globals.hpp"
#include "features.hpp"

typedef bool(__stdcall t_wglSwapBuffers)(HDC hdc);
t_wglSwapBuffers* o_wglSwapBuffers;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef LRESULT(CALLBACK t_WndProc) (HWND, UINT, WPARAM, LPARAM);
t_WndProc* o_WndProc;

typedef int(__cdecl t_ShootBullet)(float* from, float* to, int owner);
t_ShootBullet* o_ShootBullet;

namespace hooks
{
	inline hook<t_wglSwapBuffers*> m_wglSwapBuffers;
	inline hook<t_ShootBullet*> m_ShootBullet;

	LRESULT CALLBACK hk_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		if (msg == WM_KEYDOWN)
		{
			if (wParam == menu::keybind)
				menu::open = !menu::open;
		}

		if (menu::open && menu::ready)
		{
			//ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
			ImGuiIO& io = ImGui::GetIO();
			
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);

			switch (msg)
			{
			case WM_LBUTTONDOWN:
				io.MouseDown[1] = !io.MouseDown[0];
				return 0;
			case WM_RBUTTONDOWN:
				io.MouseDown[1] = !io.MouseDown[1];
				return 0;
			case WM_MBUTTONDOWN:
				io.MouseDown[2] = !io.MouseDown[2];
				return 0;
			case WM_MOUSEWHEEL:
				return 0;
			case WM_MOUSEMOVE:
				io.MousePos.x = (signed short)(lParam);
				io.MousePos.y = (signed short)(lParam >> 16);
				return 0;
			}
		}
		
	
		return CallWindowProc(o_WndProc, hwnd, msg, wParam, lParam);
	}

	void init_WndProc()
	{
		o_WndProc = (t_WndProc*)SetWindowLongPtr(globals::gl::hwnd, GWLP_WNDPROC, (LONG_PTR)hk_WndProc);
	}

	int __cdecl hk_ShootBullet(float* from, float* to, int owner)
	{
		if (globals::features::magicBullet::enabled)
		{
			uintptr_t closestPlayer = aimbot::getClosestToCursor(globals::features::magicBullet::wallCheck);

			if (closestPlayer != 0)
			{
				math::vec3 pos = *ac::entity::getPosHead(closestPlayer);

				from[0] = pos.x;
				from[1] = pos.y;
				from[2] = pos.z;
			}
		}

		return o_ShootBullet(from, to, owner);
	}

	bool __stdcall hk_wglSwapBuffers(HDC hdc)
	{
		globals::gl::hdc = hdc;
		globals::gl::hwnd = WindowFromDC(hdc);
		globals::gl::gameContext = wglGetCurrentContext();
		glGetIntegerv(GL_VIEWPORT, globals::gl::viewPort);
		memcpy(globals::gl::viewMatrix, (uintptr_t*)(globals::venom::moduleBase + globals::offsets::viewMatrix), sizeof(globals::gl::viewMatrix));

		if (!menu::ready && menu::make)
		{
			hooks::init_WndProc();
			menu::setup();
		}

		if (menu::ready)
		{
			wglMakeCurrent(globals::gl::hdc, globals::gl::menuContext);

			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			if (menu::font)
			{
				ImGui::PushFont(menu::font);

				ImGui::SetNextWindowPos(ImVec2(0, 0));
				ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
				ImGui::Begin("Overlay", nullptr,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoScrollbar |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoInputs |
					ImGuiWindowFlags_NoFocusOnAppearing |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoBackground);

				features::drawAll();
				menu::draw();

				ImDrawList* drawList = ImGui::GetWindowDrawList();
				drawList->AddText(ImVec2(10, 200), ImColor(255, 0, 0), globals::debugText);

				ImGui::PopFont();
			}

			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

			wglMakeCurrent(globals::gl::hdc, globals::gl::gameContext);
		}

		return o_wglSwapBuffers(hdc);
	}

	void init_wglSwapBuffers()
	{
		o_wglSwapBuffers = (t_wglSwapBuffers*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
		m_wglSwapBuffers.make(o_wglSwapBuffers, hk_wglSwapBuffers, 5);
		m_wglSwapBuffers.activate();

		o_ShootBullet = (t_ShootBullet*)(globals::venom::moduleBase + globals::offsets::shootBullet);
		m_ShootBullet.make(o_ShootBullet, hk_ShootBullet, 6);
		m_ShootBullet.activate();
	}

	void load()
	{
		init_wglSwapBuffers();
	}
	
	void unload()
	{
		SetWindowLongPtr(globals::gl::hwnd, GWLP_WNDPROC, (LONG_PTR)o_WndProc);
		
		m_wglSwapBuffers.restore();
		m_ShootBullet.restore();
	}
}

#endif