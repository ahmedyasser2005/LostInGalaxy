#include "pch.h"
#include "Application.hpp"

static constexpr const wchar_t* g_texFilepath = L"assets\\textures\\StoneBricksSplitface001\\StoneBricksSplitface001_COL_1K.jpg";
static constexpr const wchar_t* g_ddsFilepath = L"assets\\textures\\StoneBricksSplitface001\\StoneBricksSplitface001_COL_1K.dds";

Application::Application( const wchar_t* title, uint32_t width, uint32_t height ) :
	m_window( std::make_unique<Window>( title, width, height ) ),
	m_renderer( std::make_unique<Renderer>( m_window.get() ) ),
	m_input( m_window->GetInput() )
{
	HRESULT hr = S_OK; // CoInitializeEx( nullptr, COINIT_MULTITHREADED );

	// Setup ImGui //
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	BOOL result = TRUE; // In release mode, asserts are removed so whatever inside the assert is also removed!, that was a bug I encountered here :/
	result = ImGui_ImplWin32_Init( reinterpret_cast<void*>(m_window->GetHandle()) );
	assert( result );
	result = ImGui_ImplDX11_Init( m_renderer->GetGraphicsDevice()->GetDevice(), m_renderer->GetGraphicsDevice()->GetContext() );
	assert( result );
	/////////////////

	Image img;
	hr = DirectX::LoadFromWICFile(
		g_texFilepath,
		DirectX::WIC_FLAGS_NONE,
		&img.texMetadata,
		img.scratchImage
	);
	assert( !FAILED( hr ) );

	//hr = DirectX::LoadFromDDSFile(
	//	g_ddsFilepath,
	//	DirectX::DDS_FLAGS_NONE,
	//	&img.texMetadata,
	//	img.scratchImage
	//);
	//assert( !FAILED( hr ) );

	std::cout << "Dimension: " << img.texMetadata.dimension << '\n';
	std::cout << "Width: " << img.texMetadata.width << '\n';
	std::cout << "Height: " << img.texMetadata.height << '\n';
	std::cout << "Array Size: " << img.texMetadata.arraySize << '\n';
	std::cout << "Depth: " << img.texMetadata.depth << '\n';
	std::cout << "Mip Levels: " << img.texMetadata.mipLevels << '\n';
	std::cout << "Format: " << img.texMetadata.format << '\n';
	std::cout << "Pixels Size: " << img.scratchImage.GetPixelsSize() << '\n';
	std::cout << "Image Count: " << img.scratchImage.GetImageCount() << '\n';

	/*
	//auto pixs = img.scratchImage.GetPixels();
	//for( size_t i = 0; i < img.scratchImage.GetPixelsSize(); ++i )
	//{
	//	std::print( "{}: {}\t\t", i, pixs[i] );
	//}

	//// Compress to BC7
	//DirectX::ScratchImage compressedImage;
	//hr = DirectX::Compress(
	//	loadedImage.GetImages(),
	//	loadedImage.GetImageCount(),
	//	loadedImage.GetMetadata(),
	//	DXGI_FORMAT_BC7_UNORM,           // GPU Compression target format
	//	DirectX::TEX_COMPRESS_BC7_QUICK, // Compression speed flag
	//	DirectX::TEX_THRESHOLD_DEFAULT,
	//	compressedImage
	//);
	//assert( !FAILED( hr ) );

	//hr = DirectX::SaveToDDSFile(
	//	compressedImage.GetImages(),
	//	compressedImage.GetImageCount(),
	//	compressedImage.GetMetadata(),
	//	DirectX::DDS_FLAGS_NONE,
	//	L"StoneBricksSplitface001_COL_1K.dds"
	//);
	//assert( !FAILED( hr ) );

	*/

	// Assets
	std::array<Vertex, 24> vertices = {
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
	std::array<uint32_t, 36> indices = {
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

	m_blockMesh = std::make_shared<Mesh>( m_renderer->GetGraphicsDevice(), vertices, indices );
	m_blockMaterial = std::make_shared<Material>(
		Shader( m_renderer->GetGraphicsDevice(), L"FlatShadingVS.cso", L"FlatShadingPS.cso" ),
		Texture( m_renderer->GetGraphicsDevice(), std::move( img ), 0u ),
		Sampler( m_renderer->GetGraphicsDevice(), 0u )
	);

	// Scene
	m_scene = std::make_unique<Scene>();

	// cube
	m_cube = std::make_unique<Object>( m_blockMesh, m_blockMaterial );
	m_cube->transform.XYZ( 0.0f, 0.0f, 0.0f );
	uint8_t cubeIndex = m_scene->AddObject( m_cube.get() ); // not saved

	// camera
	m_camera = std::make_unique<Camera>();
	m_camera->transform.XYZ( 0.0f, 0.0f, -5.0f );
	uint8_t activeCamIndex = m_scene->AddCamera( m_camera.get() ); // not saved
	m_scene->SetActiveCameraIndex( activeCamIndex );
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
	ImGuiIO& imguiIO = ImGui::GetIO();

	if( imguiIO.WantCaptureKeyboard || imguiIO.WantCaptureMouse )
	{
		return;
	}

	// Camera Controls
	m_camera->Pitch( static_cast<float>(m_input.GetMouseDeltaY()), m_deltaTime );
	m_camera->Yaw( static_cast<float>(m_input.GetMouseDeltaX()), m_deltaTime );

	if( m_input.IsKeyPressed( 'W' ) )
	{
		m_camera->MoveForward( m_deltaTime );
	}
	if( m_input.IsKeyPressed( 'A' ) )
	{
		m_camera->MoveLeft( m_deltaTime );
	}
	if( m_input.IsKeyPressed( 'S' ) )
	{
		m_camera->MoveBackward( m_deltaTime );
	}
	if( m_input.IsKeyPressed( 'D' ) )
	{
		m_camera->MoveRight( m_deltaTime );
	}
	if( m_input.IsKeyPressed( 'Z' ) )
	{
		m_camera->MoveUp( m_deltaTime );
	}
	if( m_input.IsKeyPressed( 'X' ) )
	{
		m_camera->MoveDown( m_deltaTime );
	}

	if( m_input.IsKeyPressed( 'Q' ) )
	{
		m_camera->Yaw( -5.0f, m_deltaTime ); // Rotate Around Y axis(Yaw)
	}
	if( m_input.IsKeyPressed( 'E' ) )
	{
		m_camera->Yaw( 5.0f, m_deltaTime ); // Rotate Around Y axis (Yaw)
	}
	if( m_input.IsKeyPressed( 'T' ) )
	{
		m_camera->Pitch( -5.0f, m_deltaTime ); // Rotate Around X axis (Pitch)
	}
	if( m_input.IsKeyPressed( 'R' ) )
	{
		m_camera->Pitch( 5.0f, m_deltaTime ); // Rotate Around X axis (Pitch)
	}
	if( m_input.IsKeyPressed( 'F' ) )
	{
		m_camera->Roll( -5.0f, m_deltaTime ); // Rotate Around Z axis (Roll)
	}
	if( m_input.IsKeyPressed( 'G' ) )
	{
		m_camera->Roll( 5.0f, m_deltaTime ); // Rotate Around Z axis (Roll)
	}


	// Cube Controls
	if( m_input.IsKeyPressed( 'I' ) )
	{
		m_cube->transform.Pitch( m_cube->transform.Pitch() + (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
	}
	if( m_input.IsKeyPressed( 'K' ) )
	{
		m_cube->transform.Pitch( m_cube->transform.Pitch() - (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
	}
	if( m_input.IsKeyPressed( 'J' ) )
	{
		m_cube->transform.Yaw( m_cube->transform.Yaw() + (5.0f * m_deltaTime) ); // Rotate Around Y axis (Yaw)
	}
	if( m_input.IsKeyPressed( 'L' ) )
	{
		m_cube->transform.Yaw( m_cube->transform.Yaw() - (5.0f * m_deltaTime) ); // Rotate Around Y axis (Yaw)
	}

}

void Application::Render() noexcept
{
	m_renderer->Render( m_scene.get() );

	float xyz[3] = { m_camera->transform.X(),m_camera->transform.Y(), m_camera->transform.Z() };
	float pyr[3] = { m_camera->transform.Pitch(),m_camera->transform.Yaw(), m_camera->transform.Roll() };
	float fovAngleY = m_camera->GetFovAngleY();
	float aspectRatio = m_camera->GetAspectRatio();
	float nearZ = m_camera->GetNearZ();
	float farZ = m_camera->GetFarZ();

	// ImGui UI
	ImGui::Begin( "Camera" );
	ImGui::SliderFloat3( "Position", xyz, -25.0f, 25.0f );
	ImGui::SliderFloat3( "Orientation", pyr, -1.0f, 1.0f );
	if( ImGui::Button( "Reset" ) )
	{
		xyz[0] = 0.0f, xyz[1] = 0.0f, xyz[2] = -5.0f;
		pyr[0] = 0.0f, pyr[1] = 0.0f, pyr[2] = 0.0f;
	}
	ImGui::SliderFloat( "Sensitivity", &m_camera->sensitivity, 0.000001f, 5.0f );
	ImGui::SliderFloat( "Speed", &m_camera->speed, 0.000001f, 50.0f );
	ImGui::SliderFloat( "Fov", &fovAngleY, 0.001f, 360.0f );
	ImGui::SliderFloat( "Aspect Ratio", &aspectRatio, 0.001f, 360.0f );
	ImGui::SliderFloat( "Near", &nearZ, 0.001f, 360.0f );
	ImGui::SliderFloat( "Far", &farZ, 0.01f, 360.0f );
	ImGui::End();

	m_camera->transform.XYZ( xyz[0], xyz[1], xyz[2] );
	m_camera->transform.PitchYawRoll( pyr[0], pyr[1], pyr[2] );
	m_camera->SetFovAngleY( fovAngleY );
	m_camera->SetAspectRatio( aspectRatio );
	m_camera->SetNearZ( nearZ );
	m_camera->SetFarZ( farZ );
}
