#include "pch.h"

#include "Application/Application.hpp"

int main( int argc, char* argv[] )
{
	Application app = { L"Lost In Galaxy", 1280u, 720u };
	return app.Run();
}
