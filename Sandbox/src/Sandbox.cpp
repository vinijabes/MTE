// Sandbox.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include <Kinematics.h>
#include <Kinematics/Core/EntryPoint.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <functional>

class ExampleTask : public Kinematics::TaskInterface
{
public:
	ExampleTask(unsigned int time)
		:m_Time(time)
	{

	}

	~ExampleTask()
	{

	}

	void OnAccepted() override
	{
		KINEMATICS_TRACE("TASK {} ACCEPTED", this->GetType());
	}

	void OnCompleted() override
	{
		KINEMATICS_TRACE("EXECUTING TASK {} ON THREAD {}", this->GetType(), std::this_thread::get_id());
	}

	void Run() override
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(m_Time));
		KINEMATICS_TRACE("SLEEPING TIME {}", m_Time);

	}
private:
	unsigned int m_Time;
};

class FunctionTask : public Kinematics::TaskInterface
{
public:
	FunctionTask(std::function<void()> cb)
	{
		m_Callback = cb;
	}

	void Run() override
	{
		m_Callback();
	}

private:
	std::function<void()> m_Callback;
};

class Sandbox : public Kinematics::Application
{
public:
	Sandbox()
		: Application()
	{
	}

	~Sandbox()
	{

	}

	void Run() override
	{
		srand(time(NULL));
		Kinematics::Framework framework;

		framework.Initialize();

		Kinematics::TaskManager::GetInstance()->AddTask(new FunctionTask([=] {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			Kinematics::TaskManager::GetInstance()->AddTask(new FunctionTask([=] { this->Stop(); }));
		}));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));
		Kinematics::TaskManager::GetInstance()->AddTask(new ExampleTask(rand() % 10000 + 2000));


		while (this->m_Running)
		{
			framework.Update();
		}

		framework.Shutdown();
	}

	void Stop() override
	{
		m_Running = false;
	}

private:
	std::atomic<bool> m_Running = true;
};

Sandbox::Application* Kinematics::CreateApplication()
{
	return new Sandbox();
}