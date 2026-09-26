#pragma  once

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

private:
	Application() = default;
	~Application() = default;
};