#include "pch.h"
#include "Application.hpp"

static constexpr const wchar_t* g_texFilepath = L"assets\\textures\\StoneBricksSplitface001\\StoneBricksSplitface001_COL_1K.jpg";
static constexpr const wchar_t* g_ddsFilepath = L"assets\\textures\\StoneBricksSplitface001\\StoneBricksSplitface001_COL_1K.dds";

// Temporary AI Generated Helper Functions //
static void GenerateSphere( float radius, uint32_t sliceCount, uint32_t stackCount,
					std::vector<Vertex>& vertices, std::vector<uint32_t>& indices );
static std::vector<ShaderFilename> LoadShaders( const std::filesystem::path& shaderDir = "shaders" );
/////////////////////////////////////////////

Application::Application( const wchar_t* title, uint32_t width, uint32_t height ) :
	m_window( std::make_unique<Window>( title, width, height ) ),
	m_renderer( std::make_unique<Renderer>( m_window.get() ) ),
	m_scene( std::make_unique<Scene>() ),
	m_input( m_window->GetInput() ),
	m_availableShaders( LoadShaders() ),
	m_activeCamera( nullptr ),
	m_activeObject( nullptr )
{
	HRESULT hr = S_OK; // CoInitializeEx( nullptr, COINIT_MULTITHREADED );

	// Setup ImGui //
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// In release mode, asserts are removed
	// so whatever inside the assert is also removed! (e.g,  assert( ImGui_ImplWin32_Init( ... ) );)
	// which caused imgui to not initialize
	// that was a bug I encountered
	BOOL result = TRUE;
	result = ImGui_ImplWin32_Init( reinterpret_cast<void*>(m_window->GetHandle()) );
	assert( result );
	result = ImGui_ImplDX11_Init( m_renderer->GetGraphicsDevice()->GetDevice(),
								  m_renderer->GetGraphicsDevice()->GetContext() );
	assert( result );
	/////////////////


	// Load Texture
	Image img;
	hr = DirectX::LoadFromWICFile(
		g_texFilepath,
		DirectX::WIC_FLAGS_NONE,
		&img.texMetadata,
		img.scratchImage
	);
	assert( !FAILED( hr ) );

	std::cout << "Dimension: " << img.texMetadata.dimension << '\n';
	std::cout << "Width: " << img.texMetadata.width << '\n';
	std::cout << "Height: " << img.texMetadata.height << '\n';
	std::cout << "Array Size: " << img.texMetadata.arraySize << '\n';
	std::cout << "Depth: " << img.texMetadata.depth << '\n';
	std::cout << "Mip Levels: " << img.texMetadata.mipLevels << '\n';
	std::cout << "Format: " << img.texMetadata.format << '\n';
	std::cout << "Pixels Size: " << img.scratchImage.GetPixelsSize() << '\n';
	std::cout << "Image Count: " << img.scratchImage.GetImageCount() << '\n';

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

	// Setup Mesh & Material
	m_blockMesh = std::make_shared<Mesh>( m_renderer->GetGraphicsDevice(), cubeVertices, cubeIndices );
	m_sphereMesh = std::make_shared<Mesh>( m_renderer->GetGraphicsDevice(), sphereVertices, sphereIndices );

	m_material = std::make_shared<Material>(
		Shader( m_renderer->GetGraphicsDevice(), m_availableShaders.back().vsPath, m_availableShaders.back().psPath ),
		Texture( m_renderer->GetGraphicsDevice(), std::move( img ), 0u ),
		Sampler( m_renderer->GetGraphicsDevice(), 0u )
	);

	// Setup Scene
	Camera camera;
	Object cube = { m_blockMesh, m_material };
	Object sphere = { m_sphereMesh, m_material };

	cube.transform.XYZ( 1.0f, 0.0f, 0.0f );
	sphere.transform.XYZ( -1.0f, 0.0f, 0.0f );
	camera.transform.XYZ( 0.0f, 0.0f, -5.0f );

	m_scene->AddObject( cube );
	m_scene->AddObject( sphere );
	m_scene->AddCamera( camera );
}

Application::~Application() noexcept
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

		// Start the Dear ImGui frame //
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		////////////////////////////////
		Update();
		m_renderer->BeginFrame();
		Render();
		// ImGUI Render //
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
		//////////////////
		m_renderer->EndFrame();

		m_input.Reset();
	}

	return m_exitCode.value();
}

void Application::Exit() noexcept
{
	m_isRunning = false;
}

void Application::Update() noexcept
{
	if( m_input.IsKeyTriggered( VK_ESCAPE ) )
	{
		m_isMenuActive = !m_isMenuActive;
	}
	ImGuiIO& imguiIO = ImGui::GetIO();
	if( imguiIO.WantCaptureKeyboard || imguiIO.WantCaptureMouse )
	{
		return;
	}

	m_activeCamera = m_scene->GetActiveCamera();
	m_activeObject = m_scene->GetActiveObject();

	if( m_activeCamera )
	{
		// Camera Controls
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

	if( m_activeObject )
	{
		// Object Controls
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

		if( m_input.IsKeyTriggered( 'V' ) )
		{
			m_scene->ToggleObject();
		}
	}
}

void Application::Render() noexcept
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

		ImGui::End();
	}

	// Shader Control Menu (AI Generated)
	{
		ImGui::Begin( "Shader" );

		if( m_availableShaders.empty() )
		{
			ImGui::TextDisabled( "No shaders found in directory." );
		}
		else
		{
			static int currentShaderIdx = 0;

			// Bounds check
			if( currentShaderIdx >= static_cast<int>(m_availableShaders.size()) )
			{
				currentShaderIdx = 0;
			}

			if( ImGui::BeginListBox( "Select Shader", ImVec2( -FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing() ) ) )
			{
				for( int n = 0; n < static_cast<int>( m_availableShaders.size() ); n++ )
				{
					const auto& item = m_availableShaders[n];

					// Conversion from std::wstring to std::string
					std::string vsName = []( const std::wstring& wstr ) -> std::string
					{
						if( wstr.empty() ) return std::string();

						int sizeNeeded = WideCharToMultiByte( CP_UTF8, 0, wstr.data(), static_cast<int>( wstr.size() ), nullptr, 0, nullptr, nullptr );
						std::string strTo( sizeNeeded, 0 );
						WideCharToMultiByte( CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), strTo.data(), sizeNeeded, nullptr, nullptr );

						return strTo;
					}(item.vsPath);

					// Strip "VS.cso" suffix for clean label
					size_t vsPos = vsName.find( "VS.cso" );
					std::string displayName = (vsPos != std::string::npos)
						? vsName.substr( 0, vsPos ) + " Shader"
						: vsName;

					const bool isSelected = (currentShaderIdx == n);

					if( ImGui::Selectable( displayName.c_str(), isSelected ) )
					{
						currentShaderIdx = n;
						const auto& selectedShader = m_availableShaders[currentShaderIdx];
						m_material->shader = Shader( m_renderer->GetGraphicsDevice(), selectedShader.vsPath, selectedShader.psPath );
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
}


// NOTE: This is an AI generated helper function I copy pasted for quick prototyping
static void GenerateSphere( float radius, uint32_t sliceCount, uint32_t stackCount,
					std::vector<Vertex>& vertices, std::vector<uint32_t>& indices )
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
// NOTE: This is an AI generated helper function I copy pasted for quick prototyping
static std::vector<ShaderFilename> LoadShaders( const std::filesystem::path& shaderDir )
{
	std::vector<ShaderFilename> shaders;
	std::error_code ec;

	// 1. Verify directory exists
	if( !std::filesystem::exists( shaderDir, ec ) || !std::filesystem::is_directory( shaderDir, ec ) )
	{
		return shaders; // Return empty if directory missing
	}

	// Maps base shader name (e.g., L"Color") -> { VS Path, PS Path }
	std::unordered_map<std::wstring, std::pair<std::wstring, std::wstring>> shaderPairs;

	// 2. Iterate through files in the directory
	for( const auto& entry : std::filesystem::directory_iterator( shaderDir, ec ) )
	{
		if( !entry.is_regular_file() )
			continue;

		const std::filesystem::path& path = entry.path();

		// Check for .cso extension
		if( path.extension() == L".cso" )
		{
			std::wstring filename = path.filename().wstring();
			std::wstring stem = path.stem().wstring();

			// Ensure the stem is long enough to have 'VS' or 'PS' (at least 3 chars)
			if( stem.size() >= 3 )
			{
				std::wstring suffix = stem.substr( stem.size() - 2 );
				std::wstring baseName = stem.substr( 0, stem.size() - 2 );

				if( suffix == L"VS" )
				{
					shaderPairs[baseName].first = std::format( L"{}/{}", shaderDir.c_str(), filename );
				}
				else if( suffix == L"PS" )
				{
					shaderPairs[baseName].second = std::format( L"{}/{}", shaderDir.c_str(), filename );
				}
			}
		}
	}

	// 3. Assemble complete pairs into the result list
	for( const auto& [baseName, paths] : shaderPairs )
	{
		const auto& [vs, ps] = paths;

		// Only include if both Vertex and Pixel shader variants were found
		if( !vs.empty() && !ps.empty() )
		{
			shaders.push_back( ShaderFilename{ vs, ps } );
		}
	}

	return shaders;
}
