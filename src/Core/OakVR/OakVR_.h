#pragma once

#include "Utils/Singleton.h"
#include "Interface.h"
#include "Time/Timer.h"
#include "Renderer/Renderer/Mesh.h"
#include "OakVR/Camera.h"
#include "Updateable.h"

#include "OakVR/UtilityTypes.h"

#include <memory>

namespace oakvr
{
	// forward declarations
	namespace render
	{
		class Renderer;
		class RenderWindow;
		class CameraManager;
	}
	namespace core
	{
		class ResourceManager;
	}

	// Engine class
	class OakVR : public Singleton<OakVR>
	{
		friend class Singleton < OakVR >;
		friend bool oakvrUpdate();
		friend bool oakvrInit(std::vector<std::string> cmdLine);
		friend void oakvrExit();
	public:

		bool Initialize();
		bool Update();
			

		// render related interface
		static ScreenSize GetScreenSize();
		static WindowSize GetWindowSize();

		void RegisterUpdateable(std::shared_ptr<oakvr::Updateable> pUpdateable);
		void UnregisterUpdateable(std::shared_ptr<oakvr::Updateable> pUpdateable);

		void RegisterMesh(std::shared_ptr<oakvr::render::Mesh> pMesh);
		void RegisterShader(std::string shaderName);

		void RegisterCamera(std::shared_ptr<oakvr::render::Camera> pCamera);
		void UnregisterCamera(std::shared_ptr<oakvr::render::Camera> pCamera);
		std::shared_ptr<oakvr::render::Camera> GetCamera(const std::string &cameraId);
		std::shared_ptr<oakvr::render::Camera> GetCurrentCamera();
		void SetCurrentCamera(std::shared_ptr<oakvr::render::Camera> pCamera);
		void SetCurrentCamera(const std::string &cameraId);


		void RegisterSubFolderPaths(const std::string &path);
		std::shared_ptr<oakvr::core::MemoryBuffer> GetResource(const std::string &id);
				
		void Cleanup();
		void RegisterInitializer(std::function<void()> fct) { m_initializers.push_back(fct); }
		
	private:			
		bool Update(double dt);

		void TriggerInputEvents();
		void DrawInterface();
		void DrawMeshes();
		void DrawDebugText();
		void DrawAxes();
		void DrawMeshBoundingBoxes();

	private:
		OakVR();
		~OakVR();

	private:
		Timer m_timer;

		std::shared_ptr<oakvr::core::ResourceManager> m_pRM;
		std::shared_ptr<oakvr::render::RenderWindow> m_pRW;
		std::shared_ptr<oakvr::render::Renderer> m_pRenderer;
		std::shared_ptr<oakvr::render::CameraManager> m_pCM;
		
		std::vector<std::shared_ptr<oakvr::Updateable>> m_pUpdateables;

		std::vector<std::function<void()>> m_initializers;
	};
}	// namespace oakvr

#define _OAKVR_REGISTER_INITIALIZER(myInitializer, file, line) \
	struct OakVRInitializer  ## file ## line\
	{\
		OakVRInitializer  ## file ## line() { oakvr::OakVR::GetInstance().RegisterInitializer(myInitializer); }\
	} oakVRInitializer ## __FILE__ ## line;

// OAKVR initializer; Use it in a compilation unit to have one of your functions called
#define OAKVR_REGISTER_INITIALIZER(myInitializer) _OAKVR_REGISTER_INITIALIZER(myInitializer, __FILE__, __LINE__)