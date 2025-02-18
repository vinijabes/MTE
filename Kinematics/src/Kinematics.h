#pragma once

//For use by Kinematics Applications
#include "Kinematics/Core/Application.h"
#include "Kinematics/Core/Log.h"
#include "Kinematics/Core/Layer.h"
#include "Kinematics/Core/Timer.h"

#include "Kinematics/Core/KeyCodes.h"
#include "Kinematics/Core/Timestep.h"

//Renderer
#include "Kinematics/Renderer/Renderer.h"
#include "Kinematics/Renderer/Renderer2D.h"
#include "Kinematics/Renderer/RenderCommand.h"

#include "Kinematics/Renderer/Buffer.h"
#include "Kinematics/Renderer/Shader.h"
#include "Kinematics/Renderer/Texture.h"
#include "Kinematics/Renderer/VertexArray.h"
#include "Kinematics/Renderer/Text.h"

#include "Kinematics/Renderer/OrthographicCamera.h"
#include "Kinematics/Renderer/OrthographicCameraController.h"
#include "Kinematics/Renderer/PerspectiveCamera.h"

//Framework
#include "Kinematics/Framework/Framework.h"
#include "Kinematics/Framework/Events/Event.h"
#include "Kinematics/Framework/Managers/TaskManager.h"
#include "Kinematics/Framework/Managers/FactoryManager.h"
#include "Kinematics/Framework/Managers/StateManager.h"
#include "Kinematics/Framework/Managers/ResourceManager.h"
#include "Kinematics/Framework/Managers/FontManager.h"

#include "Kinematics/Scripting/LuaScript.h"
#include "Kinematics/Scripting/Callback.h"

#include "Kinematics/Framework/Interface/WindowSubSystemInterface.h"
#include "Kinematics/Framework/Interface/NetworkSubSystemInterface.h"

#include "Kinematics/UI/UI.h"