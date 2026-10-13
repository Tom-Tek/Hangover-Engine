#include "core/Application.h"

#include <chrono>
#include <iostream>

Application& Application::Singleton()
{
	static Application instance;
	return instance;
}

void Application::Run()
{
	m_running = true;

	// Attach layers pushed before Run(), so a Quit() from OnAttach ends the loop before the first tick.
	// Layers pushed from inside OnAttach attach themselves, so only the existing ones are walked.
	const size_t pendingCount = m_layerStack.size();
	for (size_t i = 0; i < pendingCount; ++i)
		m_layerStack[i]->OnAttach();

	auto lastTickStamp = std::chrono::steady_clock::now();
	while (m_running)
	{
		auto now = std::chrono::steady_clock::now();
		const float deltaTime = std::chrono::duration<float>(now - lastTickStamp).count();
		lastTickStamp = now;

		PollEvents(deltaTime);
		PollLayers(deltaTime);
	}

	Shutdown();
}

void Application::Quit()
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

void Application::Shutdown()
{
	// Destroy layers while main() is still running, in reverse order
	// so later layers go before the ones they may depend on
	while (!m_layerStack.empty())
	{
		m_layerStack.back()->OnDetach();
		m_layerStack.pop_back();
	}
}
