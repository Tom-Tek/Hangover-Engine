#pragma once

#include <iostream>

#include "Layer.h"

class EditorLayer : public Layer
{
public:
	virtual void OnUpdate(float deltaTime) override
	{
		std::printf("Tick!\n");
	}
};