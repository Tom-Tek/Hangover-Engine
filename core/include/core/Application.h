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

	template<typename TLayer, typename... TArgs>
	requires(std::is_base_of_v<Layer, TLayer>)
	void PushLayer(TArgs&&... args)
	{
		auto& layer = m_layerStack.emplace_back(std::make_unique<TLayer>(std::forward<TArgs>(args)...));
		if (m_running)
			layer->OnAttach();
	}

	template<typename TLayer>
	requires(std::is_base_of_v<Layer, TLayer>)
	TLayer* GetFirstLayer()
	{
		for (auto& layer : m_layerStack)
		{
			if (auto* found = dynamic_cast<TLayer*>(layer.get()))
				return found;
		}
		return nullptr;
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