// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <iostream>

class Sandbox : public Kinematics::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{

	}
};

Sandbox::Application* Kinematics::CreateApplication()
{
	return new Sandbox();
}