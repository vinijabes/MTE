#pragma once

#ifdef KINEMATICS_PLATFORM_WINDOWS
extern Kinematics::Application* Kinematics::CreateApplication();

int main(int argc, char** argv)
{
	Kinematics::Log::Init();
	KINEMATICS_CORE_WARN("Initialized Log!");
	int a = 5;

	auto app = Kinematics::CreateApplication();
	app->Run();
	delete app;
}

#endif