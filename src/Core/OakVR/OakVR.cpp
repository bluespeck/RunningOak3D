#include "OakVR.h"
#include "Time/Timer.h"
#include "input/MouseInput/MouseInput.h"
#include "input/KeyboardInput/KeyboardInput.h"
#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Log/Log.h"

#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/FileLoaders.h"
#include "Renderer/Renderer/Renderer.h"
#include "Renderer/Renderer/RenderWindow.h"
#include "CameraManager.h"

#include "Renderer/Renderer/Shader.h"
#include "Renderer/Renderer/Mesh.h"
#include "Renderer/Renderer/MeshElement.h"
#include "Renderer/Renderer/Material.h"
#include "Renderer/Renderer/Color.h"

#include "Text/Text.h"

#include "Profiler/Profiler.h"


#include <algorithm>
#include <memory>


namespace oakvr
{
	// --------------------------------------------------------------------------------
	bool OakVR::Update(float dt)
	{
		oakvr::input::keyboard::Update();
		oakvr::input::mouse::Update();


		if(m_pRW->IsOpen())
		{
			if ((oakvr::input::keyboard::IsDown(oakvr::input::Key::lShift) ||
				oakvr::input::keyboard::IsDown(oakvr::input::Key::rShift))
				&& oakvr::input::keyboard::IsDown(oakvr::input::Key::f5)
				)
				return false;

			// Keep updateables from interfering with the updateables vector while they are updated
			// there is a problem with deleting updateables while the loop is under way in that deleted objects might
			// have Update called for them
			auto pUpdateables = m_pUpdateables;
			for (auto &e : pUpdateables)
				e->Update(dt);

			m_pRenderer->SetViewMatrix(GetCurrentCamera()->ComputeViewMatrix());
			m_pRenderer->SetProjMatrix(GetCurrentCamera()->GetProjMatrix());
			m_pRenderer->Update(dt);
			profiler::Profiler::GetInstance().PrintSortedData();
		}
		else
		{
			m_pRenderer->Cleanup();
			return false;
		}
		
		return true;
	}

	// --------------------------------------------------------------------------------
	OakVR::OakVR()
		: m_pRM{ std::make_shared<oakvr::core::ResourceManager>() }
		, m_pRW{ std::make_shared<oakvr::render::RenderWindow>("oakvr", 100, 100, 1024, 768) }
		, m_pRenderer{ std::make_shared<oakvr::render::Renderer>() }
		, m_pCM{ std::make_shared<oakvr::render::CameraManager>() }
	{
		m_timer = oakvr::Timer();

		Initialize();
	}

	// --------------------------------------------------------------------------------
	OakVR::~OakVR()
	{
		Cleanup();
	}

	// --------------------------------------------------------------------------------
	// callback functions begin
	// --------------------------------------------------------------------------------

	void WindowSizeChangedCallback(void *pNativeHandler, int width, int height)
	{
		oakvr::OakVR::GetInstance().OnWindowSizeChanged(pNativeHandler, width, height);
	}

	void WindowFocusChangedCallback(void *pNativeHandler, int focused)
	{
		oakvr::OakVR::GetInstance().OnWindowFocusChanged(pNativeHandler, focused);
	}

	// --------------------------------------------------------------------------------
	// callback functions end
	// --------------------------------------------------------------------------------


	// --------------------------------------------------------------------------------
	bool OakVR::Initialize()
	{
		oakvr::core::InitializeFileLoaders();
		
		if(!m_pRW || !m_pRW->Initialize())
			return false;
		
		// Configure callbacks for necessary render window events
		m_pRW->SetWindowSizeCallback(WindowSizeChangedCallback);
		m_pRW->SetWindowFocusCallback(WindowFocusChangedCallback);

		if(m_pRenderer)
		{
			m_pRenderer->SetRenderWindow(m_pRW);
			m_pRenderer->SetResourceManager(m_pRM);
			m_pRenderer->Initialize();
			
			// Initialize Text manager
			oakvr::core::Text::GetInstance().SetResourceManagerPtr(m_pRM);
			oakvr::core::Text::GetInstance().SetRendererPtr(m_pRenderer);
		}

		m_timer.Reset();
		return true;
	}

	void OakVR::Cleanup()
	{
		m_pRenderer->Cleanup();
	}

	// --------------------------------------------------------------------------------
	bool OakVR::Update()
	{
		m_timer.Tick();
		return Update(m_timer.GetDeltaTime());
	}

	void OakVR::DrawLine(const oakvr::math::Vector3 &start, const oakvr::math::Vector3 &end, float thickness, const oakvr::render::Color &color)
	{
		PROFILER_FUNC_SCOPED_TIMER;
		DrawLine(start, end, thickness, color, color);
	}

	void OakVR::DrawLine(const oakvr::math::Vector3 &start, const oakvr::math::Vector3 &end, float thickness, const  oakvr::render::Color &endColor, const oakvr::render::Color &startColor)
	{
		oakvr::math::Vector3 up{ 0, 1, 0 };
		oakvr::math::Vector3 lineDir = end - start;
		oakvr::math::Vector3 p1;
		oakvr::math::Vector3 p2;
		if ((lineDir.GetNormalized() - up).GetLength() > 1e-6f)
		{
			p1 = up.Cross(lineDir).GetNormalized();
		}
		else
		{
			oakvr::math::Vector3 right{ 1, 0, 0 };
			p1 = right.Cross(lineDir).GetNormalized();
		}
		p2 = p1.Cross(lineDir).GetNormalized();
		p1 *= thickness;
		p2 *= thickness;

		std::vector<oakvr::render::VertexElementDescriptor> ved{
			oakvr::render::VertexElementDescriptor::Semantic::position,
			oakvr::render::VertexElementDescriptor::Semantic::color
		};

		oakvr::core::MemoryBuffer vb{ 8 * ComputeVertexStride(ved) * sizeof(float) };
		oakvr::core::MemoryBuffer ib{ 6 * 2 * 3 * sizeof(uint32_t) };
		struct Vertex
		{
			oakvr::math::Vector3 pos;
			oakvr::math::Vector3 color;
		} pVertices[] = {
				{ start - p1 + p2, startColor },
				{ start - p1 - p2, startColor },
				{ end - p1 - p2, endColor },
				{ end - p1 + p2, endColor },
				{ start + p1 + p2, startColor },
				{ start + p1 - p2, startColor },
				{ end + p1 - p2, endColor },
				{ end + p1 + p2, endColor },
		};

		uint32_t pIndices[] = {
			0, 3, 1,
			1, 3, 2,
			1, 6, 5,
			1, 2, 6,

			1, 4, 0,
			1, 5, 4,
			5, 6, 7,
			5, 7, 4,

			2, 3, 7,
			2, 7, 6,
			0, 7, 3,
			0, 4, 7,
		};

		memcpy(vb.GetDataPtr(), pVertices, vb.Size());
		memcpy(ib.GetDataPtr(), pIndices, ib.Size());

		auto pMaterial = std::make_shared<oakvr::render::Material>(std::string("DefaultColor"));

		
		auto pMeshElem = std::make_shared<oakvr::render::MeshElement>(ved, vb, static_cast<uint8_t>(sizeof(uint32_t)), ib, pMaterial, std::vector<std::string>());
		
		auto pMesh = std::make_shared<oakvr::render::Mesh>();
		pMesh->AddMeshElement(pMeshElem);

		m_pRenderer->RegisterOneFrameMesh(pMesh);
		oakvr::render::RegisterShader("DefaultColor");
	}

	// --------------------------------------------------------------------------------
	void OakVR::DrawMeshBoundingBoxes()
	{
		
	}

	// --------------------------------------------------------------------------------
	void OakVR::TriggerInputEvents()
	{
		/*
		using Leaf3D::MouseEvent;
		// trigger mouse events
		auto pMouseInput = oakvr::input::MouseInput::GetInstance();
		MouseEvent ev;
		ev.m_mouseData.m_bLButtonDown = pMouseInput->IsLeftButtonDown();
		ev.m_mouseData.m_bLButtonDown = pMouseInput->IsMiddleButtonDown();
		ev.m_mouseData.m_bRButtonDown = pMouseInput->IsRightButtonDown();
		ev.m_mouseData.m_mousePosition.x = pMouseInput->GetPosition().first;
		ev.m_mouseData.m_mousePosition.y = pMouseInput->GetPosition().second;
		ev.m_mouseData.m_wheelDelta = pMouseInput->GetWheelDelta();
		if(pMouseInput->IsLeftButtonDown())
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeLButtonDown);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);

			if(pMouseInput->IsLeftButtonHeld())
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeLButtonHeld);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
			else
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeLButtonPressed);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
		}
		else if(pMouseInput->IsLeftButtonUp())
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeLButtonUp);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);

			if(pMouseInput->IsLeftButtonReleased())
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeLButtonReleased);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
		}

		if(pMouseInput->IsMiddleButtonDown())
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeMButtonDown);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);

			if(pMouseInput->IsMiddleButtonHeld())
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeMButtonHeld);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
			else
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeMButtonPressed);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
		}
		else if(pMouseInput->IsMiddleButtonUp())
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeMButtonUp);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);

			if(pMouseInput->IsMiddleButtonReleased())
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeMButtonReleased);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
		}

		if(pMouseInput->IsRightButtonDown())
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeRButtonDown);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);

			if(pMouseInput->IsRightButtonHeld())
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeRButtonHeld);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
			else
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeRButtonPressed);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
		}
		else if(pMouseInput->IsRightButtonUp())
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeRButtonUp);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);

			if(pMouseInput->IsRightButtonReleased())
			{
				MouseEvent *pev = new MouseEvent();
				pev->m_mouseData = ev.m_mouseData;
				pev->SetEventSubType(ev.eventSubtypeRButtonReleased);
				Leaf3D::EventManager::GetInstance()->AddEvent(pev);
			}
		}

		if(pMouseInput->HasMouseMoved())
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeMouseMoved);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);
		}

		if(pMouseInput->GetWheelDelta() != 0)
		{
			MouseEvent *pev = new MouseEvent();
			pev->m_mouseData = ev.m_mouseData;
			pev->SetEventSubType(ev.eventSubtypeMouseWheel);
			Leaf3D::EventManager::GetInstance()->AddEvent(pev);
		}

		*/
	}

	// --------------------------------------------------------------------------------
	void OakVR::DrawInterface()
	{
		/*
		using oakvr::Leaf3D::Widget;
		
		if(Widget::GetWidgetList()->size() == 0)
			return;

		oakvr::Render::Texture *pTexture = GetResourceManager()->GetResource<oakvr::Render::Texture>("../resources/Skins/Button.dds");
		if(!pTexture->IsReady())
			return;

		using oakvr::Render::VertexBuffer;
		oakvr::Render::Shader *pVertexShader = nullptr;
		oakvr::Render::Shader *pPixelShader = nullptr;
#if (OAKVR_RENDERER == OAKVR_RENDERER_DIRECTX_9)
		oakvr::Render::DirectX9Shader::DX9AdditionalInitParams params1, params2;
		params1.shaderType = oakvr::Render::eST_VertexShader;
		params1.vertexFormat = VertexBuffer::eVF_XYZ | VertexBuffer::eVF_Tex0;
		params2.shaderType = oakvr::Render::eST_PixelShader;

		pVertexShader	= oakvr::OakVR::GetResourceManager()->GetResource<oakvr::Render::DirectX9Shader>( "../resources/shaders/hlsl_2_0/InterfaceVS.hlsl", &params1);
		pPixelShader	= oakvr::OakVR::GetResourceManager()->GetResource<oakvr::Render::DirectX9Shader>( "../resources/shaders/hlsl_2_0/InterfacePS.hlsl", &params2);
#elif (OAKVR_RENDERER == OAKVR_RENDERER_DIRECTX_11)
		oakvr::Render::Shader::ShaderAdditionalInitParams params1, params2;
		params1.shaderType = oakvr::Render::eST_VertexShader;
		params2.shaderType = oakvr::Render::eST_PixelShader;

		pVertexShader	= oakvr::OakVR::GetResourceManager()->GetResource<oakvr::Render::DirectX11Shader>( "../resources/shaders/hlsl_4_0/InterfaceVS.hlsl", &params1);
		pPixelShader	= oakvr::OakVR::GetResourceManager()->GetResource<oakvr::Render::DirectX11Shader>( "../resources/shaders/hlsl_4_0/InterfacePS.hlsl", &params2);
#elif (OAKVR_RENDERER == OAKVR_RENDERER_OPENGL)
		oakvr::Render::Shader::ShaderAdditionalInitParams params1, params2;
		params1.shaderType = oakvr::Render::eST_VertexShader;
		params2.shaderType = oakvr::Render::eST_PixelShader;

		pVertexShader	= oakvr::OakVR::GetResourceManager()->GetResource<oakvr::Render::OpenGLShader>( "../resources/shaders/glsl/InterfaceVS.glsl", &params1);
		pPixelShader	= oakvr::OakVR::GetResourceManager()->GetResource<oakvr::Render::OpenGLShader>( "../resources/shaders/glsl/InterfacePS.glsl", &params2);
#endif
		if(!pVertexShader->IsReady() || !pPixelShader->IsReady())
			return;
		
		m_pGE->EnableOrtographicProjection();
		m_pGE->EnableFillSolid();
		auto widgets = Widget::GetWidgetList();

		uint32_t numVertices = 0;
		for(auto it = widgets->begin(); it != widgets->end(); ++it)
		{
			if((*it)->IsVisible())
			{
				numVertices += 4;
			}
		}

		assert(numVertices != 0 && "There is nothing for DrawInterface to draw!!!");
		if(numVertices == 0)
			return;

		/////
		// Sort widget list in reverse depth order
		widgets->sort([](Widget * w1, Widget * w2)
			{
				return w1->GetDepth() > w2->GetDepth();
			}
		);


		/////
		// Create and populate vertex buffer
		VertexBuffer vb;
		vb.Create(numVertices, VertexBuffer::eVF_XYZ | VertexBuffer::eVF_Tex0);
		
		uint8_t *buff = new uint8_t[vb.GetVertexSize() * vb.GetVertexCount() * widgets->size()];			
		float *p = (float *) buff;

		float hw = GetRenderWindow()->GetWidth() / 2.f;
		float hh = GetRenderWindow()->GetHeight() / 2.f;

		float invWidth = 1.0f / hw;
		float invHeight = 1.0f / hh;

		auto it = widgets->begin();
		while(it != widgets->end())
		{
			using oakvr::Leaf3D::ScreenPosition2D;
			using oakvr::Leaf3D::ScreenSize2D;

			ScreenPosition2D pos = (*it)->GetPosition();
			ScreenSize2D size = (*it)->GetSize();
			float fx = (pos.x - hw) * invWidth;
			float fy = (hh - pos.y) * invHeight;
			float fw = size.width * invWidth;
			float fh = size.height * invHeight;

			*(p++)	= fx;
			*(p++)	= fy;
			*(p++)	= 0.0f;
			*(p++)	= 0.0f;
			*(p++)	= 0.0f;
			
			*(p++)	= fx + fw;
			*(p++)	= fy;
			*(p++)	= 0.0f;
			*(p++)	= 1.0f;
			*(p++)	= 0.0f;
			
			*(p++)	= fx + fw;
			*(p++)	= fy - fh;
			*(p++)	= 0.0f;
			*(p++)	= 1.0f;
			*(p++)	= 1.0f;
			
			*(p++)	= fx;
			*(p++)	= fy - fh;
			*(p++)	= 0.0f;
			*(p++)	= 0.0f;
			*(p++)	= 1.0f;

			++it;
		}

		void *pBuff = nullptr;
		vb.Lock(&pBuff);
		memcpy(pBuff, buff, vb.GetVertexSize() * vb.GetVertexCount());
		vb.Unlock();
		delete[] buff;

		/////
		// Create associated index buffer
		oakvr::Render::IndexBuffer ib;
		ib.Create(numVertices * 6 / 4);

		buff = new uint8_t[6 * numVertices];	// 6 indices for every 4 vertices, each having 4 bytes
		uint32_t *pp = (uint32_t *)buff;
		for(uint32_t i = 0; i < numVertices; i += 4)
		{
			*(pp++) = i + 0;
			*(pp++) = i + 1;
			*(pp++) = i + 3;
			*(pp++) = i + 1;
			*(pp++) = i + 2;
			*(pp++) = i + 3;
		}

		pBuff = nullptr;
		ib.Lock(&pBuff);
		memcpy(pBuff, buff, 6 * numVertices);
		ib.Unlock();
		delete[] buff;
		buff = nullptr;
		
		m_pGE->UseVertexBuffer(&vb);
		m_pGE->UseIndexBuffer(&ib);
		m_pGE->UseTexture(pTexture);
		m_pGE->UseShader(pVertexShader);
		m_pGE->UseShader(pPixelShader);
		m_pGE->UsePrimitiveTopology(oakvr::Render::ePT_TriangleList);
		m_pGE->DrawIndexedPrimitives(numVertices / 2, numVertices);
		vb.Release();
		ib.Release();
		//m_pRM->ReleaseResource(pVertexShader);

		*/
	}


	void OakVR::RegisterUpdateable(std::shared_ptr<oakvr::Updateable> pUpdateable)
	{
		m_pUpdateables.push_back(pUpdateable);
	}

	void OakVR::UnregisterUpdateable(std::shared_ptr<oakvr::Updateable> pUpdateable)
	{
		auto size = m_pUpdateables.size();
		std::remove_if(std::begin(m_pUpdateables), std::end(m_pUpdateables), [&](const std::shared_ptr<Updateable> &pRegisteredUpdateable)->bool{ return pRegisteredUpdateable == pUpdateable; });
		if (size == m_pUpdateables.size())
		{
			Log::PrintError("Trying to unregister an updateable that was not registered! But how can this be?!... ");
		}
	}

	// --------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------
	// render interface
	// --------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------

	void OakVR::RegisterMesh(std::shared_ptr<oakvr::render::Mesh> pMesh)
	{
		m_pRenderer->RegisterMesh(pMesh);
	}

	std::shared_ptr<oakvr::render::Mesh> OakVR::GetRegisteredMesh(const std::string &name)
	{
		return m_pRenderer->GetRegisteredMesh(name);
	}

	void OakVR::TransformMesh(const std::string &meshName, const oakvr::math::Matrix &mat)
	{
		auto pMesh = m_pRenderer->GetRegisteredMesh(meshName);
		pMesh->SetWorldMatrix(pMesh->GetWorldMatrix() * mat);
	}

	void OakVR::RegisterShader(std::string shaderName)
	{
		m_pRenderer->RegisterShaderProgram(shaderName);
	}

	void OakVR::RegisterCamera(std::shared_ptr<oakvr::render::Camera> pCamera)
	{
		m_pCM->RegisterCamera(pCamera);
	}

	void OakVR::UnregisterCamera(std::shared_ptr<oakvr::render::Camera> pCamera)
	{
		m_pCM->UnregisterCamera(pCamera);
	}

	std::shared_ptr<oakvr::render::Camera> OakVR::GetCamera(const std::string &cameraId)
	{
		return m_pCM->GetCamera(cameraId);
	}

	std::shared_ptr<oakvr::render::Camera> OakVR::GetCurrentCamera()
	{
		return m_pCM->GetCurrentCamera();}

	void OakVR::SetCurrentCamera(std::shared_ptr<oakvr::render::Camera> pCamera)
	{
		m_pCM->SetCurrentCamera(pCamera);
	}

	void OakVR::SetCurrentCamera(const std::string &cameraId)
	{
		m_pCM->SetCurrentCamera(cameraId);
	}
	
	void OakVR::SetRenderWindowPosition(unsigned int x, unsigned int y)
	{
		m_pRW->SetPosition(x, y);
	}

	void OakVR::SetRenderWindowSize(unsigned int width, unsigned int height)
	{
		m_pRW->SetSize(width, height);
	}
	
	WindowSize OakVR::GetRenderWindowSize()
	{
		return{ m_pRW->GetWidth(), m_pRW->GetHeight() };
	}

	float OakVR::GetRenderWindowWidth()
	{
		return static_cast<float>(m_pRW->GetWidth());
	}

	float OakVR::GetRenderWindowHeight()
	{
		return static_cast<float>(m_pRW->GetHeight());
	}

	void OakVR::SetRenderWindowTitle(const std::string &title)
	{
		m_pRW->SetTitle(title);
	}

	void OakVR::OnWindowSizeChanged(void *pNativeWindowHandler, int width, int height)
	{
		if (m_pRW->GetNativeHandle() == (long)pNativeWindowHandler)
		{
			m_pRW->SetSize(width, height);
			m_pRenderer->OnResize(m_pRW->GetWidth(), m_pRW->GetHeight());
		}
	}

	void OakVR::OnWindowFocusChanged(void *pNativeWindowHandler, int focused)
	{
		if (m_pRW->GetNativeHandle() == (long)pNativeWindowHandler)
		{
			m_pRW->OnFocusChanged(!!focused);

			// do other stuff related to fucus
		}
	}

	bool OakVR::HasFocus()
	{
		return m_pRW->HasFocus();
	}

	// --------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------
	// core interface
	// --------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------

	void OakVR::RegisterSubFolderPaths(const std::string &path)
	{
		return m_pRM->AddPathsFromFolder(path);
	}

	std::shared_ptr<oakvr::core::MemoryBuffer> OakVR::GetResource(const std::string &id)
	{
		return m_pRM->GetResource(id);
	}

	// --------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------
	// lifecycle
	// --------------------------------------------------------------------------------
	// --------------------------------------------------------------------------------


	// --------------------------------------------------------------------------------
	bool oakvrInit(std::vector<std::string> cmdLine)
	{
		for (auto e : OakVR::GetInstance().m_initializers)
			e();
		return true;
	}

	// --------------------------------------------------------------------------------
	bool oakvrUpdate()
	{
		PROFILER_FUNC_SCOPED_TIMER;
		return OakVR::GetInstance().Update();
	}

	// --------------------------------------------------------------------------------
	void oakvrExit()
	{
		OakVR::GetInstance().Cleanup();
		Log::PrintInfo("OakVR shutting down!\n");

	}
}	// namespace oakvr