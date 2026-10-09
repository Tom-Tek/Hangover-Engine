#pragma once

class Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

public:
	virtual void OnAttach() { }
	virtual void OnDetach() { }
	virtual void OnUpdate(const float deltaTime) { }

};