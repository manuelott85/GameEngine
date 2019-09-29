#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer);
	}

	~Sandbox()
	{

	}
};

// This function is in the namespace Hazel and not in the HAZEL_API and it is only decleared not defined
// This allows you do define this function in a client application using this namespace
Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}