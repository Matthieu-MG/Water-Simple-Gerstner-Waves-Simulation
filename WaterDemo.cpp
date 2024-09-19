//-- UI Lib
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
//-- OpenGL & Windowing Lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//-- RENDERING-RELATED CLASSES
#include "src/Application.h"
#include "src/Shader/Shader.h"
#include "src/Buffer/Buffer.h"
#include "src/Buffer/VertexArray.h"
#include "src/Buffer/VertexBufferLayout.h"
#include "src/Buffer/Framebuffer.h"
#include "src/Model/CubeMap.h"
//-- MODELS
#include "src/Model/Model.h"
#include "src/Model/Quad.h"
//-- MATHS
#include <glm/gtc/matrix_transform.hpp>
//-- UTILS
#include "Camera.h"
#include "Input.h"

namespace WATER_SIM
{
	#define SCR_WIDTH 1000
	#define SCR_HEIGHT 600
	void process_input(GLFWwindow* window);
	void mouse_callback(GLFWwindow* window, double xposition, double yposition);

	bool firstMouse = true;
	float lastPosX = 0.0f;
	float lastPosY = 0.0f;
	Camera camera(0.0f, 5.0f, 30.0f);

	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	int WaveCount = 32;
	bool EditMode = false;

	int main()
	{
		//-- GLFW and GLAD SETUP
		Application& app = Application::Get();
		app.Init(SCR_WIDTH, SCR_HEIGHT, "Water");
		if (app.GetStatus() != 0)
			return 1;

		auto* window = app.GetWindow();
		
		Input::Init(window);
		app.SetBackgroundColor(0.2f, 0.2f, 0.2f);

		// DISABLE VSYNC
		glfwSwapInterval(0);

		// Set GLFW Mouse Callbacks
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);

		// ImGui Setup
		EditMode = false;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Shader instancing
		ShaderManager::AddShader("shader\\model.vs", "shader\\model.frags", "BASIC_MAT");
		ShaderManager::AddShader("shader\\ScreenTexture.vs", "shader\\ScreenTexture.frags", "SCREEN_TEXTURE");
		ShaderManager::AddShader("shader\\gerstner.vs", "shader\\gerstner.frags", "WATER");
		ShaderManager::AddShader("shader\\CubeMap.vs", "shader\\CubeMap.frags", "CUBE_MAP");

		Shader modelShader = ShaderManager::Shaders["BASIC_MAT"];
		Shader screenTextureShader = ShaderManager::Shaders["SCREEN_TEXTURE"];
		Shader WaterShader = ShaderManager::Shaders["WATER"];

		// Basic matrices for MVP in vertex shader of the water plane
		glm::mat4 model(1.0f);
		glm::mat4 view(1.0f);
		view = glm::translate(view, glm::vec3(0.0, 0.0f, -5.0f));
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)app.GetWidth() / (float)app.GetHeight(), 0.1f, 1000.0f);

		// Offscreen FrameBuffer
		FrameBuffer fbo = FrameBuffer(app.GetWidth(), app.GetHeight());
		fbo.AttachTexture(FBTarget::COLOR);
		fbo.AttachRenderObject(FBTarget::DEPTH);
		LOG( (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) );
		fbo.Unbind();

		// Quad drawn on default framebuffer to apply post-processing via SCREEN_TEXTURE shader
		ScreenQuad screenQuad = ScreenQuad();

		// Water plane & shader
		Model plane("planeXZ.obj");
		plane.m_materials[0].BindShader(&WaterShader);
		
		WaterShader.use();
		WaterShader.setMat4("model", &model);
		WaterShader.setMat4("view", &view);
		WaterShader.setMat4("projection", &proj);

		// Skybox
		std::vector<const char*> faces
		{
				"skybox/right.jpg",
				"skybox/left.jpg",
				"skybox/top.jpg",
				"skybox/bottom.jpg",
				"skybox/front.jpg",
				"skybox/back.jpg",
		};
		CubeMap skybox = CubeMap(faces);
		ShaderManager::Shaders["CUBE_MAP"].use();
		ShaderManager::Shaders["CUBE_MAP"].setMat4("projection", &proj);
		ShaderManager::Shaders["CUBE_MAP"].setInt("CubeMapTexture", 0);

		// Timers
		unsigned int FrameCounter = 0u;
		float Timer = static_cast<float>(glfwGetTime());

		// Water waves parameters
		float Amplitude = 0.2f;
		float Frequency = 0.5f;
		float Steepness = 0.7f;
		float MaxAngle = 360.0f;

		glm::vec3 DiffuseCol  = glm::vec3(0.3, 0.61, 0.71);
		glm::vec3 AmbientCol  = glm::vec3(0.0, 0.4, 0.5);
		glm::vec3 SpecularCol = glm::vec3(1.0, 0.27, 0.0);

		float Shininess = 64.0f;
		float SpecStrength = 0.7f;
		float SpecFresnelIntensity = 0.2f;
		float FoamHeight = 0.5f;
		
		float SSS_Distortion = 0.2f;
		float SSS_Scale = 0.4f;
		float SSS_Power = 8.0f;
		float SSS_Dist = 50.0f;
		float SSS_Strength = 0.5f;
		
		float SunHeight = 0.1f;
		glm::vec2 SunDir = glm::vec2(0.0, -1.0);
		glm::vec2 WaveDirection = glm::vec2(-0.60, 0.5);

		WaterShader.use();
		WaterShader.setInt("WaveCount", WaveCount);
		WaterShader.setFloat("Amp", Amplitude);
		WaterShader.setFloat("Frequency", Frequency);
		WaterShader.setFloat("Steepness", Steepness);
		WaterShader.setFloat("MAX_ANGLE", MaxAngle);
		WaterShader.setVec2("Direction", WaveDirection);
		
		WaterShader.setVec3("_AmbientCol", AmbientCol);
		WaterShader.setVec3("_DiffuseCol", DiffuseCol);
		WaterShader.setVec3("_SpecularCol", SpecularCol);

		WaterShader.setFloat("Shininess", Shininess);
		WaterShader.setFloat("SpecStrength", SpecStrength);
		WaterShader.setFloat("SpecFresnelIntensity", SpecFresnelIntensity);
		WaterShader.setFloat("FoamHeight", FoamHeight);

		WaterShader.setFloat("SSS_Distortion", SSS_Distortion);
		WaterShader.setFloat("SSS_Strength", SSS_Strength);
		WaterShader.setFloat("SSS_Power", SSS_Power);
		WaterShader.setFloat("SSS_Dist", SSS_Dist);
		WaterShader.setFloat("SSS_Scale", SSS_Scale);
		WaterShader.setFloat("SunHeight", SunHeight);
		WaterShader.setVec2("SunDirXZ", SunDir);

		while (!glfwWindowShouldClose(window))
		{
			float currentTime = static_cast<float>(glfwGetTime());
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			process_input(window);

			// OFF-SCREEN FRAMEBUFFER
			fbo.Bind();
			glViewport(0, 0, fbo.GetWidth(), fbo.GetHeight());
			app.SetBackgroundColor(0.0f, 0.0f, 1.0f);
			app.EnableDepthTest();
			app.UpdateAndClearBuffers();
			view = camera.GetViewMatrix();
			proj = glm::perspective(glm::radians(45.0f), (float)app.GetWidth() / (float)app.GetHeight(), 0.1f, 1000.0f);
			// --------------

			// ImGUI New Frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			// --------------

			// Draws Scene
			skybox.BindCubeMapTex();
			WaterShader.use();
			WaterShader.setMat4("view", &view);
			WaterShader.setMat4("projection", &proj);
			WaterShader.setFloat("Time", currentTime);
			WaterShader.setVec3("viewPos", camera.GetCameraPosition());
			plane.Draw();
			// --------------

			// Draws Skybox
			glDepthFunc(GL_LEQUAL);
			ShaderManager::Shaders["CUBE_MAP"].use();
			glm::mat4 ViewNoTranslation = glm::mat4(glm::mat3(view));
			ShaderManager::Shaders["CUBE_MAP"].setMat4("view", &ViewNoTranslation);
			ShaderManager::Shaders["CUBE_MAP"].setMat4("projection", &proj);
			skybox.Draw();
			glDepthFunc(GL_LESS);
			// --------------

			// DEFAULT FRAMEBUFFER
			fbo.Unbind();
			glViewport(0, 0, app.GetWidth(), app.GetHeight());
			app.SetBackgroundColor(0.0f, 0.0f, 1.0f);
			app.DisableDepthTest();
			app.UpdateAndClearBuffers();

			screenTextureShader.use();
			glBindTexture(GL_TEXTURE_2D, fbo.GetTextureAttachmentID(FBTarget::COLOR));
			screenQuad.Draw();
			// --------------

			// ImGui Window
			if (EditMode)
			{
				ImGui::Begin("Water Settings");
				ImGui::SetNextWindowSize(ImVec2(30.0, 50.0));
				
				ImGui::Text("Geometry");
				ImGui::SliderInt("Waves", &WaveCount, 1, 500);
				ImGui::SliderFloat("Amp", &Amplitude, 0.01f, 1.0f);
				ImGui::SliderFloat("Frequency", &Frequency, 0.01f, 1.0f);
				ImGui::SliderFloat("Steepness", &Steepness, 0.0f, 1.0f);
				ImGui::SliderFloat("Max Angle", &MaxAngle, 0.0f, 360.0f);
				ImGui::SliderFloat2("Wave Direction", &WaveDirection.x, -1.0f, 1.0f);

				ImGui::Text("Shading");
				ImGui::Text("Lighting");
				ImGui::ColorEdit3("Ambient", &AmbientCol.x);
				ImGui::ColorEdit3("Diffuse", &DiffuseCol.x);
				ImGui::ColorEdit3("Specular", &SpecularCol.x);
				ImGui::SliderFloat("Shininess", &Shininess, 0.0f, 512.0f);
				ImGui::SliderFloat("SpecStrength", &SpecStrength, 0.0f, 1.0f);
				ImGui::SliderFloat("SpecFresnelIntensity", &SpecFresnelIntensity, 0.0f, 1.0f);
				ImGui::SliderFloat("FoamHeight", &FoamHeight, 0.0f, 1.0f);

				ImGui::Text("(fake) Subsurface Scattering");
				ImGui::SliderFloat("Distortion", &SSS_Distortion, 0.0f, 1.0f);
				ImGui::SliderFloat("Power", &SSS_Power, 1.0f, 64.0f);
				ImGui::SliderFloat("Scale", &SSS_Scale, 0.0f, 1.0f);
				ImGui::SliderFloat("Distance", &SSS_Dist, 0.1f, 1000.0f);
				ImGui::SliderFloat("Strength", &SSS_Strength, 0.0f, 1.0f);
				
				ImGui::Text("Atmosphere");
				ImGui::SliderFloat("Sun Height", &SunHeight, 0.1f, 1.0f);
				ImGui::SliderFloat2("Sun Direction", &SunDir.x, -1.0f, 1.0f);
				ImGui::End();

				WaterShader.use();
				WaterShader.setInt("WaveCount", WaveCount);
				WaterShader.setFloat("Amp", Amplitude);
				WaterShader.setFloat("Frequency", Frequency);
				WaterShader.setFloat("Steepness", Steepness);
				WaterShader.setFloat("MAX_ANGLE", MaxAngle);
				WaterShader.setVec2("Direction", WaveDirection);
				
				WaterShader.setVec3("_AmbientCol", AmbientCol);
				WaterShader.setVec3("_DiffuseCol", DiffuseCol);
				WaterShader.setVec3("_SpecularCol", SpecularCol);

				WaterShader.setFloat("Shininess", Shininess);
				WaterShader.setFloat("SpecStrength", SpecStrength);
				WaterShader.setFloat("SpecFresnelIntensity", SpecFresnelIntensity);
				WaterShader.setFloat("FoamHeight", FoamHeight);

				WaterShader.setFloat("SSS_Distortion", SSS_Distortion);
				WaterShader.setFloat("SSS_Strength", SSS_Strength);
				WaterShader.setFloat("SSS_Power", SSS_Power);
				WaterShader.setFloat("SSS_Dist", SSS_Dist);
				WaterShader.setFloat("SSS_Scale", SSS_Scale);
				
				WaterShader.setFloat("SunHeight", SunHeight);
				WaterShader.setVec2("SunDirXZ", SunDir);
			}
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// --------------
			
			glfwSwapBuffers(window);
			glfwPollEvents();

			FrameCounter += 1;
			if (currentTime - Timer >= 1.0f)
			{
				Timer += 1.0f;
				LOG("FPS: " << FrameCounter);
				FrameCounter = 0;
			}
		}

		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
		glDeleteProgram(WaterShader.ID);
		
		return 0;
	}

	void mouse_callback(GLFWwindow* window, double xposition, double yposition)
	{
		if(EditMode)
		{
			return;
		}

		float xPos = static_cast<float>(xposition);
		float yPos = static_cast<float>(yposition);

		if (firstMouse)
		{
			lastPosX = xPos;
			lastPosY = yPos;
			firstMouse = false;
		}

		float offsetX = xPos - lastPosX;
		float offsetY = lastPosY - yPos;

		camera.ProcessMouseMovement(offsetX, offsetY);

		lastPosX = xPos;
		lastPosY = yPos;
	}


	void process_input(GLFWwindow* window)
	{
		// Close Window
		if (Input::IsKeyHeld(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, true);

		// Movement
		if (Input::IsKeyHeld(GLFW_KEY_A))
			camera.ProcessKeyboardInput(LEFT, deltaTime);
		if (Input::IsKeyHeld(GLFW_KEY_D))
			camera.ProcessKeyboardInput(RIGHT, deltaTime);
		if (Input::IsKeyHeld(GLFW_KEY_W))
			camera.ProcessKeyboardInput(FORWARD, deltaTime);
		if (Input::IsKeyHeld(GLFW_KEY_S))
			camera.ProcessKeyboardInput(BACKWARD, deltaTime);

		// Edit Mode
		if (Input::IsKeyPressed(GLFW_KEY_R))
		{
			EditMode = !EditMode;
			if (EditMode)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}
}

int main()
{
	return WATER_SIM::main();
}