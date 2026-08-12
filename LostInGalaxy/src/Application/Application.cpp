#include "pch.h"
#include "Application.hpp"



// Temporary AI Generated Helper Functions //
static constexpr void GenerateSphere( float radius, uint32_t sliceCount, uint32_t stackCount,
					std::vector<Vertex>& vertices, std::vector<uint32_t>& indices ) noexcept
{
	vertices.clear();
	indices.clear();

	// 1. Generate Vertices
	// Top pole
	vertices.push_back( {
		DirectX::XMFLOAT3( 0.0f, radius, 0.0f ),
		DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ),
		DirectX::XMFLOAT2( 0.5f, 0.0f )
	} );

	float phiStep = static_cast<float>(3.14159265358979323846) / stackCount;
	float thetaStep = 2.0f * static_cast<float>(3.14159265358979323846) / sliceCount;

	// Rings from top to bottom (excluding the poles)
	for( uint32_t i = 1; i < stackCount; ++i )
	{
		float phi = i * phiStep;

		for( uint32_t j = 0; j <= sliceCount; ++j )
		{
			float theta = j * thetaStep;

			// Unit sphere coordinates (also functions as the normal)
			float x = sinf( phi ) * cosf( theta );
			float y = cosf( phi );
			float z = sinf( phi ) * sinf( theta );

			DirectX::XMFLOAT3 pos( radius * x, radius * y, radius * z );
			DirectX::XMFLOAT3 norm( x, y, z );
			DirectX::XMFLOAT2 uv( static_cast<float>(j) / sliceCount, static_cast<float>(i) / stackCount );

			vertices.push_back( { pos, norm, uv } );
		}
	}

	// Bottom pole
	vertices.push_back( {
		DirectX::XMFLOAT3( 0.0f, -radius, 0.0f ),
		DirectX::XMFLOAT3( 0.0f, -1.0f, 0.0f ),
		DirectX::XMFLOAT2( 0.5f, 1.0f )
	} );

	// 2. Generate Indices
	// Top cap indices
	for( uint32_t i = 1; i <= sliceCount; ++i )
	{
		indices.push_back( 0 );
		indices.push_back( i + 1 );
		indices.push_back( i );
	}

	// Body quads (as two triangles per cell)
	uint32_t baseIndex = 1;
	uint32_t ringVertexCount = sliceCount + 1;

	for( uint32_t i = 0; i < stackCount - 2; ++i )
	{
		for( uint32_t j = 0; j < sliceCount; ++j )
		{
			indices.push_back( baseIndex + i * ringVertexCount + j );
			indices.push_back( baseIndex + i * ringVertexCount + j + 1 );
			indices.push_back( baseIndex + (i + 1) * ringVertexCount + j );

			indices.push_back( baseIndex + (i + 1) * ringVertexCount + j );
			indices.push_back( baseIndex + i * ringVertexCount + j + 1 );
			indices.push_back( baseIndex + (i + 1) * ringVertexCount + j + 1 );
		}
	}

	// Bottom cap indices
	uint32_t southPoleIndex = static_cast<uint32_t>(vertices.size()) - 1;
	baseIndex = southPoleIndex - ringVertexCount;

	for( uint32_t i = 0; i < sliceCount; ++i )
	{
		indices.push_back( southPoleIndex );
		indices.push_back( baseIndex + i );
		indices.push_back( baseIndex + i + 1 );
	}
}
/////////////////////////////////////////////



Application::Application( const wchar_t* title, uint32_t width, uint32_t height ) :
	m_window( std::make_unique<Window>( title, width, height ) ),
	m_renderer( std::make_unique<Renderer>( m_window.get() ) ),
	m_assetManager( std::make_unique<AssetManager>( m_renderer->GetGraphicsDevice() ) ),
	m_scene( std::make_unique<Scene>() ),
	m_input( m_window->GetInput() ),
	m_activeCamera( nullptr ),
	m_activeObject( nullptr ),
	m_activeLight( nullptr )
{
	HRESULT hr = S_OK; // CoInitializeEx( nullptr, COINIT_MULTITHREADED );

	/*************************************************
	* In release mode, asserts are removed			 *
	* so whatever inside the assert is also removed! *
	* (e.g,  assert( ImGui_ImplWin32_Init( ... ) );) *
	* which caused imgui to not initialize			 *
	* that was a bug I encountered					 *
	*************************************************/

	/* Setup ImGui */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	bool ir = true;
	ir = ImGui_ImplWin32_Init( reinterpret_cast<void*>(m_window->GetHandle()) );
	assert( ir );
	ir = ImGui_ImplDX11_Init( m_renderer->GetGraphicsDevice()->GetDevice(),
								  m_renderer->GetGraphicsDevice()->GetContext() );
	assert( ir );



	// Setup Cube's data (AI Gen vertices, I'm just lazy to define it myself)
	std::array<Vertex, 24> cubeVertices = {
		// Front Face (Z = -0.5f) -> Normal: (0, 0, -1)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 0
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 1
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 2
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 3
		// Back Face (Z = +0.5f) -> Normal: (0, 0, 1)
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 4
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 5
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 6
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 7
		// Top Face (Y = +0.5f) -> Normal: (0, 1, 0)
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 8
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 9
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 10
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 11
		// Bottom Face (Y = -0.5f) -> Normal: (0, -1, 0)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 12
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 13
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 14
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( 0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 15
		// Left Face (X = -0.5f) -> Normal: (-1, 0, 0)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 16
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 17
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 18
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 19
		// Right Face (X = +0.5f) -> Normal: (1, 0, 0)
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 20
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 21
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 22
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }  // 23
	};
	std::array<uint32_t, 36> cubeIndices = {
		// Front Face
		0, 1, 2,		0, 2, 3,
		// Back Face
		4, 5, 6,		4, 6, 7,
		// Top Face
		8, 9, 10,		8, 10, 11,
		// Bottom Face
		12, 13, 14,		12, 14, 15,
		// Left Face
		16, 17, 18,		16, 18, 19,
		// Right Face
		20, 21, 22,		20, 22, 23
	};
	// Setup Sphere's data (AI Gen vertices, I'm just lazy to define it myself)
	std::vector<Vertex> sphereVertices;
	std::vector<uint32_t> sphereIndices;
	GenerateSphere( 0.5f, 30, 30, sphereVertices, sphereIndices );


	// Setup Assets
	bool status = true;
	m_assetManager->LoadAllShadersFromDirectory( "Shaders" );
	status = m_assetManager->LoadMesh( "Cube1", cubeVertices, cubeIndices );
	assert( status );
	status = m_assetManager->LoadMesh( "Sphere1", sphereVertices, sphereIndices );
	assert( status );
	status = m_assetManager->LoadSampler( "MrSampler" );
	assert( status );
	status = m_assetManager->LoadTexture(
		"StoneWallTex",
		"assets\\Textures\\StoneBricksSplitface001\\StoneBricksSplitface001_COL_1K.jpg"
	);
	assert( status );
	status = m_assetManager->LoadMaterial( "GoldMat",
								  "LightingVS",
								  "BlinnPhongPS",
								  "StoneWallTex",
								  "MrSampler",
								  { 1.0f, 0.7f, 0.0f },
								  100.0f );
	assert( status );
	status = m_assetManager->LoadMaterial( "LapizMat",
								  "LightingVS",
								  "BlinnPhongPS",
								  "StoneWallTex",
								  "MrSampler",
								  { 0.0f, 0.7f, 1.0f },
								  32.0f );
	assert( status );

	// Setup Scene
	Camera camera;
	Object cube(
		m_assetManager->GetAsset<Mesh>( "Cube1" ),
		m_assetManager->GetAsset<Material>( "GoldMat" )
	);
	Object sphere(
		m_assetManager->GetAsset<Mesh>( "Sphere1" ),
		m_assetManager->GetAsset<Material>( "LapizMat" )
	);

	LightSource light1;
	LightSource light2;
	LightSource light3;
	LightSource light4;

	cube.transform.XYZ( 1.0f, 0.0f, 0.0f );
	sphere.transform.XYZ( -1.0f, 0.0f, 0.0f );
	camera.transform.XYZ( 0.0f, 0.0f, -5.0f );

	light1.transform.XYZ( 3.0f, 3.0f, 3.0f );
	light2.transform.XYZ( 100.0f, 100.0f, 100.0f );
	light3.transform.XYZ( 100.0f, 100.0f, 100.0f );
	light4.transform.XYZ( 100.0f, 100.0f, 100.0f );

	m_scene->AddObject( cube );
	m_scene->AddObject( sphere );

	m_scene->AddCamera( camera );

	m_scene->AddLight( light1 );
	m_scene->AddLight( light2 );
	m_scene->AddLight( light3 );
	m_scene->AddLight( light4 );
}

Application::~Application() noexcept(!_DEBUG)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

const int Application::Run()
{
	while( m_isRunning )
	{
		m_deltaTime = std::clamp( m_timer.Mark(), 0.0f, 10.0f );

		if( m_exitCode = m_window->ProcessMessages() )
		{
			Exit();
			break;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Update();

		m_renderer->BeginFrame();
		Render();
		
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

		m_renderer->EndFrame();


		m_input.Reset();
	}

	return m_exitCode.value();
}

void Application::Exit() noexcept(!_DEBUG)
{
	m_isRunning = false;
}

void Application::Update() noexcept(!_DEBUG)
{
	m_activeCamera = m_scene->GetActiveCamera();
	m_activeObject = m_scene->GetActiveObject();
	m_activeLight = m_scene->GetActiveLight();
	assert( m_activeCamera );
	assert( m_activeObject );
	assert( m_activeLight );

	if( m_input.IsKeyTriggered( VK_ESCAPE ) )
	{
		m_isMenuActive = !m_isMenuActive;
	}
	ImGuiIO& imguiIO = ImGui::GetIO();
	if( imguiIO.WantCaptureKeyboard || imguiIO.WantCaptureMouse )
	{
		return;
	}

	// Camera Controls
	if( m_activeCamera )
	{
		m_activeCamera->Pitch( static_cast<float>(m_input.GetMouseDeltaY()), m_deltaTime );
		m_activeCamera->Yaw( static_cast<float>(m_input.GetMouseDeltaX()), m_deltaTime );

		if( m_input.IsKeyPressed( 'W' ) )
		{
			m_activeCamera->MoveForward( m_deltaTime );
		}
		if( m_input.IsKeyPressed( 'A' ) )
		{
			m_activeCamera->MoveLeft( m_deltaTime );
		}
		if( m_input.IsKeyPressed( 'S' ) )
		{
			m_activeCamera->MoveBackward( m_deltaTime );
		}
		if( m_input.IsKeyPressed( 'D' ) )
		{
			m_activeCamera->MoveRight( m_deltaTime );
		}
		if( m_input.IsKeyPressed( 'Z' ) )
		{
			m_activeCamera->MoveUp( m_deltaTime );
		}
		if( m_input.IsKeyPressed( 'X' ) )
		{
			m_activeCamera->MoveDown( m_deltaTime );
		}

		if( m_input.IsKeyPressed( 'Q' ) )
		{
			m_activeCamera->Yaw( -5.0f, m_deltaTime ); // Rotate Around Y axis(Yaw)
		}
		if( m_input.IsKeyPressed( 'E' ) )
		{
			m_activeCamera->Yaw( 5.0f, m_deltaTime ); // Rotate Around Y axis (Yaw)
		}
		if( m_input.IsKeyPressed( 'T' ) )
		{
			m_activeCamera->Pitch( -5.0f, m_deltaTime ); // Rotate Around X axis (Pitch)
		}
		if( m_input.IsKeyPressed( 'R' ) )
		{
			m_activeCamera->Pitch( 5.0f, m_deltaTime ); // Rotate Around X axis (Pitch)
		}
		if( m_input.IsKeyPressed( 'F' ) )
		{
			m_activeCamera->Roll( -5.0f, m_deltaTime ); // Rotate Around Z axis (Roll)
		}
		if( m_input.IsKeyPressed( 'G' ) )
		{
			m_activeCamera->Roll( 5.0f, m_deltaTime ); // Rotate Around Z axis (Roll)
		}
	}

	// Object Controls
	if( m_activeObject )
	{
		if( m_rotateMode )
		{
			if( m_input.IsKeyPressed( 'I' ) )
			{
				m_activeObject->transform.Pitch( m_activeObject->transform.Pitch() + (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
			}
			if( m_input.IsKeyPressed( 'K' ) )
			{
				m_activeObject->transform.Pitch( m_activeObject->transform.Pitch() - (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
			}
			if( m_input.IsKeyPressed( 'J' ) )
			{
				m_activeObject->transform.Yaw( m_activeObject->transform.Yaw() + (5.0f * m_deltaTime) ); // Rotate Around Y axis (Yaw)
			}
			if( m_input.IsKeyPressed( 'L' ) )
			{
				m_activeObject->transform.Yaw( m_activeObject->transform.Yaw() - (5.0f * m_deltaTime) ); // Rotate Around Y axis (Yaw)
			}
			if( m_input.IsKeyPressed( 'U' ) )
			{
				m_activeObject->transform.Roll( m_activeObject->transform.Roll() + (5.0f * m_deltaTime) ); // Rotate Around Z axis (Roll)
			}
			if( m_input.IsKeyPressed( 'O' ) )
			{
				m_activeObject->transform.Roll( m_activeObject->transform.Roll() - (5.0f * m_deltaTime) ); // Rotate Around Z axis (Roll)
			}
		}
		else
		{
			if( m_input.IsKeyPressed( 'I' ) )
			{
				m_activeObject->transform.Z( m_activeObject->transform.Z() + (5.0f * m_deltaTime) ); // Move Forward
			}
			if( m_input.IsKeyPressed( 'K' ) )
			{
				m_activeObject->transform.Z( m_activeObject->transform.Z() - (5.0f * m_deltaTime) ); // Move Backward
			}
			if( m_input.IsKeyPressed( 'J' ) )
			{
				m_activeObject->transform.X( m_activeObject->transform.X() + (5.0f * m_deltaTime) ); // Move Right
			}
			if( m_input.IsKeyPressed( 'L' ) )
			{
				m_activeObject->transform.X( m_activeObject->transform.X() - (5.0f * m_deltaTime) ); // Move Left
			}
			if( m_input.IsKeyPressed( 'U' ) )
			{
				m_activeObject->transform.Y( m_activeObject->transform.Y() + (5.0f * m_deltaTime) ); // Move Up
			}
			if( m_input.IsKeyPressed( 'O' ) )
			{
				m_activeObject->transform.Y( m_activeObject->transform.Y() - (5.0f * m_deltaTime) ); // Move Down
			}
		}

		if( m_input.IsKeyTriggered( 'V' ) )
		{
			m_scene->ToggleObject();
		}
		if( m_input.IsKeyTriggered( 'B' ) )
		{
			m_rotateMode = !m_rotateMode;
		}
	}
}

void Application::Render() noexcept(!_DEBUG)
{
	m_renderer->Render( m_scene.get() );

	if( !m_isMenuActive )
	{
		return;
	}

	// Camera Control Menu
	if( m_activeCamera )
	{
		float xyz[3] = { m_activeCamera->transform.X(), m_activeCamera->transform.Y(), m_activeCamera->transform.Z() };
		float pyr[3] = { m_activeCamera->transform.Pitch(), m_activeCamera->transform.Yaw(), m_activeCamera->transform.Roll() };
		float fovAngleY = m_activeCamera->GetFovAngleY();
		float aspectRatio = m_activeCamera->GetAspectRatio();
		float nearZ = m_activeCamera->GetNearZ();
		float farZ = m_activeCamera->GetFarZ();

		ImGui::Begin( "Camera" );

		if( ImGui::SliderFloat3( "Position", xyz, -25.0f, 25.0f ) )
		{
			m_activeCamera->transform.XYZ( xyz[0], xyz[1], xyz[2] );
		}

		if( ImGui::SliderFloat3( "Orientation", pyr, -3.14159f, 3.14159f ) ) // Radians or Degrees (-180 to 180)
		{
			m_activeCamera->transform.PitchYawRoll( pyr[0], pyr[1], pyr[2] );
		}

		if( ImGui::Button( "Reset" ) )
		{
			xyz[0] = 0.0f; xyz[1] = 0.0f; xyz[2] = -5.0f;
			pyr[0] = 0.0f; pyr[1] = 0.0f; pyr[2] = 0.0f;

			m_activeCamera->transform.XYZ( xyz[0], xyz[1], xyz[2] );
			m_activeCamera->transform.PitchYawRoll( pyr[0], pyr[1], pyr[2] );
		}

		ImGui::SliderFloat( "Sensitivity", &m_activeCamera->sensitivity, 0.0001f, 5.0f, "%.4f" );
		ImGui::SliderFloat( "Speed", &m_activeCamera->speed, 0.01f, 50.0f );

		if( ImGui::SliderFloat( "FOV", &fovAngleY, 1.0f, 120.0f ) )
		{
			m_activeCamera->SetFovAngleY( fovAngleY );
		}

		if( ImGui::SliderFloat( "Aspect Ratio", &aspectRatio, 0.1f, 3.0f ) )
		{
			m_activeCamera->SetAspectRatio( aspectRatio );
		}

		if( ImGui::SliderFloat( "Near Z", &nearZ, 0.01f, 10.0f ) )
		{
			m_activeCamera->SetNearZ( nearZ );
		}

		if( ImGui::SliderFloat( "Far Z", &farZ, 10.0f, 1000.0f ) )
		{
			m_activeCamera->SetFarZ( farZ );
		}

		ImGui::End();
	}

	// Object Control Menu
	if( m_activeObject )
	{
		ImGui::Begin( "Object" );

		if( m_scene && m_scene->GetActiveObjectIndex().has_value() )
		{
			ImGui::Text( "Selected Object: %zu", m_scene->GetActiveObjectIndex().value() );
		}
		else
		{
			ImGui::TextUnformatted( "Selected Object: None" );
		}

		if( ImGui::Button( "Switch Objects" ) )
		{
			m_scene->ToggleObject();
		}

		float rgb[3] = { m_activeObject->material->color.x,
						m_activeObject->material->color.y,
						m_activeObject->material->color.z };
		if( ImGui::ColorEdit3( "Material Color", rgb ) )
		{
			m_activeObject->material->color.x = rgb[0];
			m_activeObject->material->color.y = rgb[1];
			m_activeObject->material->color.z = rgb[2];
		}

		ImGui::SliderFloat( "Shininess", &m_activeObject->material->shininess, 0.0f, 1000.0f );


		ImGui::End();
	}

	// Shaders Control Menu (AI Generated, Modified by Me)
	{
		const auto& vshaders = m_assetManager->GetAssets<VertexShader>();
		const auto& pshaders = m_assetManager->GetAssets<PixelShader>();

		ImGui::Begin( "Shaders" );

		if( vshaders.empty() )
		{
			ImGui::TextDisabled( "No vertex shaders found in directory." );
		}
		else
		{
			static int currentShaderIdx = 0;

			// Bounds check
			if( currentShaderIdx >= static_cast<int>(vshaders.size()) )
			{
				currentShaderIdx = 0;
			}

			if( ImGui::BeginListBox( "Select Vertex Shader", ImVec2( -FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing() ) ) )
			{
				for( int i = 0; i < static_cast<int>( vshaders.size() ); i++ )
				{
					const auto& item = vshaders[i];
					const bool isSelected = (currentShaderIdx == i);

					if( ImGui::Selectable( vshaders[i].data(), isSelected ) )
					{
						currentShaderIdx = i;
						const auto& selectedShader = vshaders[currentShaderIdx];

						if( m_activeObject )
						{
							m_activeObject->material->vShader = m_assetManager->GetAsset<VertexShader>( vshaders[i] );
						}
					}

					if( isSelected )
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}
		}

		if( pshaders.empty() )
		{
			ImGui::TextDisabled( "No pixel shaders found in directory." );
		}
		else
		{
			static int currentShaderIdx = 0;

			// Bounds check
			if( currentShaderIdx >= static_cast<int>(pshaders.size()) )
			{
				currentShaderIdx = 0;
			}

			if( ImGui::BeginListBox( "Select Pixel Shader", ImVec2( -FLT_MIN, 6 * ImGui::GetTextLineHeightWithSpacing() ) ) )
			{
				for( int i = 0; i < static_cast<int>( pshaders.size() ); i++ )
				{
					const auto& item = pshaders[i];
					const bool isSelected = (currentShaderIdx == i);

					if( ImGui::Selectable( pshaders[i].data(), isSelected ) )
					{
						currentShaderIdx = i;
						const auto& selectedShader = pshaders[currentShaderIdx];

						if( m_activeObject )
						{
							m_activeObject->material->pShader = m_assetManager->GetAsset<PixelShader>( pshaders[i] );
						}
					}

					if( isSelected )
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}
		}

		ImGui::End();
	}

	// LightSource Control Menu
	if( m_activeLight )
	{
		float xyz[3] = { m_activeLight->transform.X(), m_activeLight->transform.Y(), m_activeLight->transform.Z() };
		float rgb[3] = { m_activeLight->tint.x, m_activeLight->tint.y, m_activeLight->tint.z };

		ImGui::Begin( "LightSource" );

		if( m_scene && m_scene->GetActiveLightIndex().has_value() )
		{
			ImGui::Text( "Selected LightSource: %zu", m_scene->GetActiveLightIndex().value() );
		}
		else
		{
			ImGui::TextUnformatted( "Selected LightSource: None" );
		}

		if( ImGui::Button( "Switch Lights" ) )
		{
			m_scene->ToggleLight();
		}

		if( ImGui::SliderFloat3( "Position", xyz, -25.0f, 25.0f ) )
		{
			m_activeLight->transform.XYZ( xyz[0], xyz[1], xyz[2] );
		}

		if( ImGui::ColorEdit3( "Tint", rgb ) )
		{
			m_activeLight->tint = { rgb[0], rgb[1], rgb[2] };
		}
		ImGui::SliderFloat( "Intensity", &m_activeLight->intensity, 0.001f, 15.0f, "%.4f" );

		ImGui::End();
	}

}
