#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "Core.h"
#include "Layer.h"

class CORE_API Application
{
public:
	// Defined in the DLL so every module shares the same instance
	static Application& Singleton();

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

#pragma warning(push)
#pragma warning(disable: 4251) // std types in an exported class, safe while all modules share a CRT
	std::vector<std::unique_ptr<Layer>> m_layerStack;
#pragma warning(pop)
};