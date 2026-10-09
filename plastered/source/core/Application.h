#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "Layer.h"

class Application
{
public:
	static Application& Singleton() 
	{
		static Application instance;
		return instance;
	}

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void Run();
	void Quit();

	template<typename T, typename... TArgs>
	requires(std::is_base_of_v<Layer, T>)
	void PushLayer(TArgs&&... args)
	{
		auto& layer = m_layerStack.emplace_back(std::make_unique<T>(std::forward<TArgs>(args)...));
		layer->OnAttach();
	}

private:
	Application() = default;
	~Application() = default;

	void PollEvents(const float deltaTime);
	void PollLayers(const float deltaTime);
	void Shutdown();

	bool m_running = false;

	std::vector<std::unique_ptr<Layer>> m_layerStack;
};