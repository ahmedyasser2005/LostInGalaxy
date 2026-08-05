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
	std::array<Vertex, 24> vertices = {
		// Front Face (Z = -0.5f)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 0
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 1
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f ),  DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 2
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f ),  DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 3
		// Back Face (Z = +0.5f)
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f ),  DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 4
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f ),  DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 5
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 6
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 7
		// Top Face (Y = +0.5f)
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 8
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 9
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f ),  DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 10
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f ),  DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 11
		// Bottom Face (Y = -0.5f)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 12
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 13
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f ),  DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 14
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f ),  DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 15
		// Left Face (X = -0.5f)
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 16
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 17
		Vertex{ DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 18
		Vertex{ DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) }, // 19
		// Right Face (X = +0.5f)
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f ),  DirectX::XMFLOAT2( 0.0f, 1.0f ) }, // 20
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f ),  DirectX::XMFLOAT2( 0.0f, 0.0f ) }, // 21
		Vertex{ DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f ),  DirectX::XMFLOAT2( 1.0f, 0.0f ) }, // 22
		Vertex{ DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f ),  DirectX::XMFLOAT2( 1.0f, 1.0f ) }  // 23
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
	if( m_input.IsKeyPressed( 'A' ) )
	{
		m_camera->transform.X( m_camera->transform.X() - (15.0f * m_deltaTime) );
	}
	if( m_input.IsKeyPressed( 'S' ) )
	{
		m_camera->transform.Z( m_camera->transform.Z() - (15.0f * m_deltaTime) );
	}
	if( m_input.IsKeyPressed( 'D' ) )
	{
		m_camera->transform.X( m_camera->transform.X() + (15.0f * m_deltaTime) );
	}

	if( m_input.IsKeyPressed( 'X' ) )
	{
		m_camera->transform.Y( m_camera->transform.Y() - (15.0f * m_deltaTime) );
	}
	if( m_input.IsKeyPressed( 'Z' ) )
	{
		m_camera->transform.Y( m_camera->transform.Y() + (15.0f * m_deltaTime) );
	}

	if( m_input.IsKeyPressed( 'Q' ) )
	{
		m_camera->transform.RY( m_camera->transform.RY() - (5.0f * m_deltaTime) ); // Rotate Around Y axis (YAW)
	}
	if( m_input.IsKeyPressed( 'E' ) )
	{
		m_camera->transform.RY( m_camera->transform.RY() + (5.0f * m_deltaTime) ); // Rotate Around Y axis (YAW)
	}
	if( m_input.IsKeyPressed( 'T' ) )
	{
		m_camera->transform.RX( m_camera->transform.RX() - (5.0f * m_deltaTime) ); // Rotate Around X axis (PITCH)
	}
	if( m_input.IsKeyPressed( 'R' ) )
	{
		m_camera->transform.RX( m_camera->transform.RX() + (5.0f * m_deltaTime) ); // Rotate Around X axis (PITCH)
	}



	if( m_input.IsKeyPressed( 'G' ) )
	{
		m_cube->transform.RX( m_cube->transform.RX() - (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
	}
	if( m_input.IsKeyPressed( 'H' ) )
	{
		m_cube->transform.RX( m_cube->transform.RX() + (5.0f * m_deltaTime) ); // Rotate Around X axis (Pitch)
	}


	//const float x = (float)m_input.GetMouseDeltaX() * 0.5f * m_deltaTime;
	//const float y = (float)m_input.GetMouseDeltaY() * 0.5f * m_deltaTime;

	//m_camera->transform.RotXYZ( m_camera->transform.RX() + y, // rotate around X axis (Pitch)
	//							m_camera->transform.RY() + x, // rotate around Y axis (Yaw)
	//							m_camera->transform.RZ() ); // rotate around Z axis (Roll)

}

void Application::Render() noexcept
{
	m_renderer->Render( m_scene.get() );
}
