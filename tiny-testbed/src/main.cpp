#include "pch.hpp"
#include <tiny_engine.hpp>

using namespace tiny;

class Testbed : public TinyApplication
{
public:
	Testbed(const WindowProperties& properties)
		: TinyApplication(properties)
	{

	}
};

std::unique_ptr<tiny::TinyApplication> createApplication()
{
	return std::make_unique<Testbed>(WindowProperties{ "Testbed" });
}