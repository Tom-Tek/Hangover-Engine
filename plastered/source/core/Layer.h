#pragma once

class Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

public:
	virtual void OnUpdate(float deltaTime) { }

};