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

class EmptyTask : public Kinematics::TaskInterface
{
public:
	EmptyTask(int i) : m_Index(i) {}

	void Run() override
	{
		for (size_t i = 0; i < 100000; i++);
		//KINEMATICS_TRACE("RUNNING TASK {}", m_Index);
	};

	int m_Index;
};

class BackgroundReadFile : public Kinematics::BackgroundTask
{
	void Run() override
	{
		for (int i = 0; i < 500; i++);
	}

	void OnCancel() override
	{
		KINEMATICS_TRACE("BACKGROUND TASK CANCELLED");
	}

	void OnBackgroundCompleted() override
	{
		KINEMATICS_TRACE("BACKGROUND TASK COMPLETED");
	}
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

		std::function<void()>* cb = new std::function<void()>([=] { m_Running = false; });
		framework.Initialize();
		Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::WindowClose, [=](Kinematics::Event* e) {
			this->Stop();
			return false;
		});

		Kinematics::StateManager::GetInstance()->On(Kinematics::EventType::WindowResize, [=](Kinematics::Event* e) {
			Kinematics::WindowResizeEvent* we = (Kinematics::WindowResizeEvent*) e;
			KINEMATICS_TRACE("Window Resize: {} {}", we->GetWidth(), we->GetHeight());
			return false;
		});

		framework.GetSubSystem<Kinematics::WindowSubSystemInterface>()->SetWindowResizeCallback([=](Kinematics::Event& e) {
			//KINEMATICS_CORE_TRACE("Window Resized: {} {}", message.GetWidth(), message.GetHeight());
			});

		while (this->m_Running)
		{
			{
				//Timer t("Sandbox::Update", [] {});
				framework.Update();
			}
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