#pragma once

extern Kinematics::Application* Kinematics::CreateApplication();

extern "C" {

	int main(int argc, char** argv)
	{
		Kinematics::Log::Init();

		KINEMATICS_PROFILE_BEGIN_SESSION("Startup", "KinematicsProfile-Startup.js");
		auto app = Kinematics::CreateApplication();
		KINEMATICS_PROFILE_END_SESSION();

		KINEMATICS_PROFILE_BEGIN_SESSION("Runtime", "KinematicsProfile-Runtime.js");
		app->Run();
		KINEMATICS_PROFILE_END_SESSION();

		KINEMATICS_PROFILE_BEGIN_SESSION("Shutdown", "KinematicsProfile-Shutdown.js");
		delete app;
		KINEMATICS_PROFILE_END_SESSION();
	}

}