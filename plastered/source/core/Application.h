#pragma  once

#include <vector>
#include <iostream>
#include <type_traits>
#include <forward_list>

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
	void Shutdown();

	template<typename T, typename... TArgs>
	requires(std::is_base_of_v<Layer, T>)
	void PushLayer(TArgs&&... args)
	{
		m_layerStack.push_back(std::make_unique<T>(std::forward<TArgs>(args)...));
	}

private:
	Application() = default;
	~Application() = default;

	void PollEvents(const float deltaTime);
	void PollLayers(const float deltaTime);

	bool m_running = false;

	std::vector<std::unique_ptr<Layer>> m_layerStack;
};