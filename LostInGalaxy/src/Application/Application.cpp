#include "pch.h"
#include "Application.hpp"

static constexpr const wchar_t* g_texFilepath = L"assets\\textures\\StoneBricksSplitface001\\StoneBricksSplitface001_COL_1K.jpg";
static constexpr const wchar_t* g_ddsFilepath = L"assets\\textures\\StoneBricksSplitface001\\StoneBricksSplitface001_COL_1K.dds";

Application::Application( const wchar_t* title, uint32_t width, uint32_t height ) :
	m_window( std::make_unique<Window>( title, width, height ) ),
	m_renderer( std::make_unique<Renderer>( m_window.get() ) ),
	m_input( m_window->GetInput() )
{
	HRESULT hr = CoInitializeEx( nullptr, COINIT_MULTITHREADED );
	assert( !FAILED( hr ) );


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
	std::array<Vertex, 8> vertices = {
		// Front Face Corners (Z = -0.5)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2{ 0.0f, 1.0f } /*{ 255,   0,   0, 255 }*/ }, // 0: Bottom-Left-Front  (Red)		
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT2{ 0.0f, 0.0f } /*{   0, 255,   0, 255 }*/ }, // 1: Top-Left-Front     (Green)	
		Vertex{ DirectX::XMFLOAT3( 0.5f,   0.5f, -0.5f ), DirectX::XMFLOAT2{ 1.0f, 0.0f } /*{   0,   0, 255, 255 }*/ }, // 2: Top-Right-Front    (Blue)		
		Vertex{ DirectX::XMFLOAT3( 0.5f,  -0.5f, -0.5f ), DirectX::XMFLOAT2{ 1.0f, 1.0f } /*{ 255, 255,   0, 255 }*/ }, // 3: Bottom-Right-Front (Yellow)

		// Back Face Corners (Z = 0.5)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT2{ 0.0f, 1.0f } /*{ 255,   0, 255, 255 }*/ }, // 4: Bottom-Left-Back  (Magenta)
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT2{ 0.0f, 0.0f } /*{   0, 255, 255, 255 }*/ }, // 5: Top-Left-Back     (Cyan)
		Vertex{ DirectX::XMFLOAT3( 0.5f,   0.5f,  0.5f ), DirectX::XMFLOAT2{ 1.0f, 0.0f } /*{ 255, 255, 255, 255 }*/ }, // 6: Top-Right-Back    (White)
		Vertex{ DirectX::XMFLOAT3( 0.5f,  -0.5f,  0.5f ), DirectX::XMFLOAT2{ 1.0f, 1.0f } /*{   0,   0,   0, 255 }*/ }  // 7: Bottom-Right-Back (Black)
	};
	std::array<uint32_t, 36> indices = {
		// Front Face
		0, 1, 2,
		0, 2, 3,

		// Back Face
		4, 6, 5,
		4, 7, 6,

		// Top Face
		1, 5, 6,
		1, 6, 2,

		// Bottom Face
		4, 0, 3,
		4, 3, 7,

		// Left Face
		4, 1, 0,
		4, 5, 1,

		// Right Face
		3, 2, 6,
		3, 6, 7
	};

	m_blockMesh = std::make_shared<Mesh>( m_renderer->GetGraphicsDevice(), vertices, indices );
	m_blockMaterial = std::make_shared<Material>(
		Shader( m_renderer->GetGraphicsDevice(), L"TextureVS.cso", L"TexturePS.cso" ),
		Texture( m_renderer->GetGraphicsDevice(), std::move( img ), 0u ),
		Sampler( m_renderer->GetGraphicsDevice(), 0u )
	);

	// Scene
	m_scene = std::make_unique<Scene>();

	// cube
	m_cube = std::make_unique<Object>( m_blockMesh, m_blockMaterial );
	m_cube->transform.PosXYZ( 0.0f, 0.0f, 0.0f );
	uint8_t cubeIndex = m_scene->AddObject( m_cube.get() ); // not saved

	// camera
	m_camera = std::make_unique<Camera>();
	m_camera->transform.PosXYZ( 0.0f, 0.0f, -5.0f );
	uint8_t activeCamIndex = m_scene->AddCamera( m_camera.get() ); // not saved
	m_scene->SetActiveCameraIndex( activeCamIndex );
}

const int Application::Run()
{
	while( m_isRunning )
	{
		m_deltaTime = std::clamp( m_timer.Mark(), 0.0f, 10.0f );

		if( m_exitCode = m_window->ProcessMessages() )
		{
			Exit();
		}

		Update();
		Render();

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
	if( m_input.IsKeyPressed( 'W' ) )
	{
		m_camera->transform.Z( m_camera->transform.Z() + (15.0f * m_deltaTime) );
	}
	if( m_input.IsKeyPressed( 'S' ) )
	{
		m_camera->transform.Z( m_camera->transform.Z() - (15.0f * m_deltaTime) );
	}


	if( m_input.IsKeyPressed( 'A' ) )
	{
		m_camera->transform.X( m_camera->transform.X() - (15.0f * m_deltaTime) );
	}
	if( m_input.IsKeyPressed( 'D' ) )
	{
		m_camera->transform.X( m_camera->transform.X() + (15.0f * m_deltaTime) );
	}


	if( m_input.IsKeyPressed( 'Z' ) )
	{
		m_camera->transform.Y( m_camera->transform.Y() + (15.0f * m_deltaTime) );
	}
	if( m_input.IsKeyPressed( 'X' ) )
	{
		m_camera->transform.Y( m_camera->transform.Y() - (15.0f * m_deltaTime) );
	}



	if( m_input.IsKeyPressed( 'Q' ) )
	{
		m_cube->transform.RX( m_cube->transform.RX() - (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
		//m_camera->transform.RZ( m_camera->transform.RZ() - (5.0f * m_deltaTime) ); // Rotate Around Z axis (ROLL)
	}
	if( m_input.IsKeyPressed( 'E' ) )
	{
		m_cube->transform.RX( m_cube->transform.RX() + (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
		//m_camera->transform.RZ( m_camera->transform.RZ() + (5.0f * m_deltaTime) ); // Rotate Around Z axis (ROLL)
	}

	const float x = (float)m_input.GetMouseDeltaX() * 0.5f * m_deltaTime;
	const float y = (float)m_input.GetMouseDeltaY() * 0.5f * m_deltaTime;

	m_camera->transform.RotXYZ( m_camera->transform.RX() + y, // rotate around X axis (Pitch)
								m_camera->transform.RY() + x, // rotate around Y axis (Yaw)
								m_camera->transform.RZ() ); // rotate around Z axis (Roll)

}

void Application::Render() noexcept
{
	m_renderer->Render( m_scene.get() );
}
