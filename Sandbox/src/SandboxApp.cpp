#include <Hazel.h>

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{

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