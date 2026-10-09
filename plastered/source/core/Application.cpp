#include "Application.h"

#include <chrono>



void Application::Run()
{
	auto lastTickStamp = std::chrono::steady_clock::now();

	m_running = true;
	while (m_running)
	{
		auto now = std::chrono::steady_clock::now();
		const float deltaTime = std::chrono::duration<float>(now - lastTickStamp).count();
		lastTickStamp = now;

		PollEvents(deltaTime);
		PollLayers(deltaTime);
	}
}

void Application::Shutdown()
{
	m_running = false;
}

void Application::PollEvents(const float deltaTime)
{
	// Filler function for now
}

void Application::PollLayers(const float deltaTime)
{
	for (auto& layer : m_layerStack)
		layer->OnUpdate(deltaTime);
}
