#pragma once
#include <memory>
#include "core/application.hpp"

extern std::unique_ptr<tiny::TinyApplication> createApplication();

int main()
{
	std::unique_ptr<tiny::TinyApplication> application = createApplication();
	application->run();

	return 0;
}