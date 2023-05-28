#pragma once
#ifndef FEATURES_HPP
#define FEATURES_HPP

#include <vector>
#include <functional>

namespace features {
	inline std::vector<std::function<void(void)>> drawings;

	void addDrawing(std::function<void(void)> drawing)
	{
		features::drawings.push_back(drawing);
	}

	void drawAll()
	{
		for (std::function<void(void)> drawing : features::drawings)
		{
			drawing();
		}
	}
}

#endif