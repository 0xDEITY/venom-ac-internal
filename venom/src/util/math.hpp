#pragma once
#ifndef MATH_HPP
#define MATH_HPP

#include <math.h>
#include "../../ext/imgui/imgui.h"

namespace math
{
	constexpr float pi = 3.14159265359;

	template<typename T>
	T lerp(const T& start, const T& end, float t)
	{
		return start + (end - start) * t;
	}

	template<typename T>
	T lerpYaw(const T& start, const T& end, float t)
	{
		T result;

		if (end - start > T(180))
		{
			T wrappedEnd = end - T(360);
			result = start + (wrappedEnd - start) * t;
		}
		else if (start - end > T(180))
		{
			T wrappedStart = start - T(360);
			result = wrappedStart + (end - wrappedStart) * t;
		}
		else
		{
			result = start + (end - start) * t;
		}

		return result;
	}

	struct vec3 {
		float x, y, z;

		vec3()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		vec3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		};

		float dist(vec3 other)
		{
			return sqrt(
				pow(other.x - this->x, 2) +
				pow(other.y - this->y, 2) +
				pow(other.z - this->z, 2)
			);
		}
	};

	ImColor ImColor4f(float col[4])
	{
		return ImColor(col[0], col[1], col[2], col[3]);
	}

	struct vec4 {
		float x, y, z, w;

		vec4()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 0.0f;
		}

		vec4(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		};
	};

	vec3 w2s(vec3 pos, int viewport[4], float matrix[16])
	{
		vec4 clipCoords
		{
			pos.x* matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12],
			pos.x* matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13],
			pos.x* matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14],
			pos.x* matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15]
		};
	
		if (clipCoords.w < 0.1f)
		{
			return {NAN, NAN, NAN};
		}

		vec3 ndc
		{
			clipCoords.x / clipCoords.w,
			clipCoords.y / clipCoords.w,
			clipCoords.z / clipCoords.w
		};


		return vec3{ (viewport[2] / 2 * ndc.x) + (ndc.x + viewport[2] / 2) , -(viewport[3] / 2 * ndc.y) + (ndc.y + viewport[3] / 2) , 0};
	}

	template<typename T>
	T clamp(T val, T min, T max)
	{
		return (val < min) ? min : (val > max) ? max : val;
	}
};

#endif