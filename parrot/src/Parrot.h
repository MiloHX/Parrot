#pragma once

//---------- Core -----------

#include "parrot/core/Application.h"
#include "parrot/core/TimeStep.h"
#include "parrot/core/Layer.h"
#include "parrot/core/input.h"
#include "parrot/core/KeyCode.h"
#include "parrot/imgui/ImGuiLayer.h"
#include "parrot/ecs/Scene.h"
#include "parrot/ecs/Entity.h"
#include "parrot/ecs/Component.h"

//----------- Tool ------------
#include "parrot/core/Log.h"
#include "parrot/tool/Profiler.h"
#include "parrot/tool/ScopeTimer.h"

//----------- Renderer ------------

#include "parrot/renderer/Renderer.h"
#include "parrot/renderer/Renderer2D.h"
#include "parrot/renderer/RenderCommand.h"
#include "parrot/renderer/VertexArray.h"
#include "parrot/renderer/Shader.h"
#include "parrot/renderer/Buffer.h"
#include "parrot/renderer/FrameBuffer.h"
#include "parrot/renderer/Texture.h"
#include "parrot/renderer/SubTexture2D.h"
#include "parrot/renderer/OrthographicCamera.h"
#include "parrot/renderer/OrthographicCameraController.h"
