#pragma once

#include "Core.h"

class CORE_API Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

public:
	virtual void OnAttach() { }
	virtual void OnDetach() { }
	virtual void OnUpdate(const float deltaTime) { }

};