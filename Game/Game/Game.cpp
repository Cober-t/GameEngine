#include "Game.h"

// --------------------------------------------------------------------------------------

Game::Game() : Layer("Game application") 
{
	m_MousePosition = glm::vec2(0.0f);
	float screenWidth = EngineApp::GetWindow().GetWidth();
	float screenHeight = EngineApp::GetWindow().GetHeight();
	//m_DefaultCamera = CreateRef<GameCamera>(45.0f, screenWidth, screenHeight, 0.01f, 1000.0f, GlobalCamera::perspective);

#if 0
	// Create Framebuffer...
	m_Fbo = Framebuffer::Create(m_DefaultCamera->GetSettings().width, m_DefaultCamera->GetSettings().height);
#endif
}

// --------------------------------------------------------------------------------------

void Game::OnAttach() 
{
	m_shader = Shader::Create("PositionColor");
	//m_ActiveScene = Scene::Load("SceneDefault.lua");
	
	auto SDLGPUDevice = SDLGPURenderAPI::Get()->GetDevice();
	
	// Create the pipeline
	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.vertex_shader = m_shader->GetVertexShader();
	pipelineCreateInfo.fragment_shader = m_shader->GetFragmentShader();
	pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	// Only to test
	m_shader->ReleaseShaders();
	
	SDL_GPUVertexBufferDescription vertexBufferDesc {};
	vertexBufferDesc.slot = 0;
	vertexBufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
	vertexBufferDesc.instance_step_rate = 0;
	vertexBufferDesc.pitch = sizeof(PositionColorVertex);
	
	SDL_GPUVertexAttribute vertexAttributes [2];
	vertexAttributes[0].buffer_slot = 0;
	vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
	vertexAttributes[0].location = 0;
	vertexAttributes[0].offset = 0;
	vertexAttributes[1].buffer_slot = 0;
	vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
	vertexAttributes[1].location = 1;
	vertexAttributes[1].offset = sizeof(float) * 3;
	
	SDL_GPUVertexInputState vertexInputState {};
	vertexInputState.num_vertex_buffers = 1;
	vertexInputState.vertex_buffer_descriptions = &vertexBufferDesc;
	vertexInputState.num_vertex_attributes = 2;
	vertexInputState.vertex_attributes = vertexAttributes;
	pipelineCreateInfo.vertex_input_state = vertexInputState;

	SDL_GPUGraphicsPipelineTargetInfo pipelinteTargetInfo {};
	pipelinteTargetInfo.num_color_targets = 1;
	SDL_GPUColorTargetDescription colorTargetDes {};
	colorTargetDes.format = SDL_GetGPUSwapchainTextureFormat(SDLGPUDevice, EngineApp::GetWindow().GetRawWindow());
	pipelinteTargetInfo.color_target_descriptions = &colorTargetDes;

	pipelineCreateInfo.target_info = pipelinteTargetInfo;

	Pipeline = SDL_CreateGPUGraphicsPipeline(SDLGPUDevice, &pipelineCreateInfo);
	if (Pipeline == NULL)
	{
		SDL_Log("Failed to create pipeline!");
		return ;
	}


	// Create the vertex buffer
	SDL_GPUBufferCreateInfo bufferInfo;
	bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
	bufferInfo.size = sizeof(PositionColorVertex) * 3;
	VertexBuffer = SDL_CreateGPUBuffer( SDLGPUDevice, &bufferInfo);

	// To get data into the vertex buffer, we have to use a transfer buffer
	SDL_GPUTransferBufferCreateInfo transferBufferInfo;
	transferBufferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	transferBufferInfo.size = sizeof(PositionColorVertex) * 3;

	SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(
		SDLGPUDevice, &transferBufferInfo);

	PositionColorVertex* transferData = (PositionColorVertex*)SDL_MapGPUTransferBuffer(
		SDLGPUDevice,
		transferBuffer,
		false
	);

	transferData[0].Position = { -1.0f, -1.0f, 0.0f };
	transferData[1].Position = {  1.0f, -1.0f, 0.0f };
	transferData[2].Position = {  0.0f,  1.0f, 0.0f };
	transferData[0].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	transferData[1].Color = { 0.0f, 1.0f, 0.0f, 1.0f };
	transferData[2].Color = { 0.0f, 0.0f, 1.0f, 1.0f };

	SDL_UnmapGPUTransferBuffer(SDLGPUDevice, transferBuffer);

	// Upload the transfer data to the vertex buffer
	SDL_GPUCommandBuffer* uploadCmdBuf = SDL_AcquireGPUCommandBuffer(SDLGPUDevice);
	SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmdBuf);

	
	SDL_GPUTransferBufferLocation bufferLocation;
	bufferLocation.transfer_buffer = transferBuffer;
	bufferLocation.offset = 0;

	SDL_GPUBufferRegion bufferRegion;
	bufferRegion.buffer = VertexBuffer;
	bufferRegion.offset = 0;
	bufferRegion.size = sizeof(PositionColorVertex) * 3;
	SDL_UploadToGPUBuffer( copyPass, &bufferLocation, &bufferRegion, false );

	SDL_EndGPUCopyPass(copyPass);
	SDL_SubmitGPUCommandBuffer(uploadCmdBuf);
	SDL_ReleaseGPUTransferBuffer(SDLGPUDevice, transferBuffer);
}

// --------------------------------------------------------------------------------------

void Game::OnDetach()
{
	// m_ActiveScene->OnSimulationStop();
	// m_ActiveScene = nullptr;
	// m_DefaultCamera = nullptr;
	LOG_INFO("Detached Game application Layer!");
}

// --------------------------------------------------------------------------------------

void Game::OnUpdate(const Timestep& ts) 
{
	auto SDLGPUDevice = SDLGPURenderAPI::Get()->GetDevice();


	// Begin Frame
	SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(SDLGPUDevice);
    if (cmdbuf == NULL)
    {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return;
    }

    SDL_GPUTexture* swapchainTexture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, EngineApp::GetWindow().GetRawWindow(), &swapchainTexture, NULL, NULL)) {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return;
    }

	// Main Render pass
	if (swapchainTexture != NULL)
	{
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;
		colorTargetInfo.clear_color = { 0.9f, 0.4f, 0.2f, 1.0f };
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass( cmdbuf, &colorTargetInfo, 1, NULL );

		// Draw internal
		SDL_GPUBufferBinding bufferBinding {};
		bufferBinding.buffer = VertexBuffer;
		bufferBinding.offset = 0;

		SDL_BindGPUGraphicsPipeline(renderPass, Pipeline);
		SDL_BindGPUVertexBuffers(renderPass, 0, &bufferBinding, 1);
		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);

		// End Render pass
		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(cmdbuf);
	
#if 0
	RenderGlobals::SetClearColor(0.85, 0.35, 0.2);
	//m_Fbo->Bind();
	// Clear Framebuffer Attachments...
	//m_Fbo->ClearAttachment(1, -1);

	m_ActiveScene->OnUpdateSimulation(ts, m_DefaultCamera);
	

	// Get Selected Entity using inverted coordinates...
	//int pixelData = m_Fbo->ReadPixel(1, m_MousePosition.x, EngineApp::GetWindow().GetHeight() - m_MousePosition.y);
	//LOG_WARNING("{0} {1} {2}", pixelData, m_MousePosition.x, m_MousePosition.y);

	// Unbind Framebuffer...
	//m_Fbo->Unbind();

	// Render Framebuffer Attachment (scene generated texture)...
	//Cober::Render2D::DrawFramebuffer(m_Fbo);
#endif
}

// --------------------------------------------------------------------------------------

void Game::OnEvent(Event& event) 
{
	if (event.GetEventType() == EventType::MouseMoved)
	{
		m_MousePosition.x = static_cast<MouseMovedEvent&>(event).GetX();
		m_MousePosition.y = static_cast<MouseMovedEvent&>(event).GetY();
	}

	if (event.GetEventType() == EventType::WindowResize)
	{
		// Resize Framebuffer...
		float screenWidth = static_cast<WindowResizeEvent&>(event).GetWidth();
		float screenHeight = static_cast<WindowResizeEvent&>(event).GetHeight();
		//m_Fbo->Resize(screenWidth, screenHeight);
	}

	//m_ActiveScene->OnEvent(event, m_DefaultCamera);
	// NativeScriptFn::OnEvent(m_ActiveScene.get(), event);
}

// --------------------------------------------------------------------------------------