#include "pch.h"
#include "AssetManager.hpp"

AssetManager::AssetManager( GraphicsDevice* graphicsDevice ) noexcept(!_DEBUG) :
	m_device( graphicsDevice )
{
	assert( m_device );
}

void AssetManager::LoadAllShadersFromDirectory( std::filesystem::path shadersDirectory ) noexcept(!_DEBUG)
{
	for( auto& entry : std::filesystem::directory_iterator( shadersDirectory ) )
	{
		const auto p = entry.path();

		if( !(entry.is_regular_file() && p.extension() == ".cso") ) [[unlikely]]
			continue;

		const auto s = p.filename().string();

		if( s.contains( "VS" ) ) [[unlikely]]
		{
			LoadVertexShader( s.substr( 0, s.size() - 4 ), p );
		}
		else if( s.contains( "PS" ) ) [[likely]]
		{
			LoadPixelShader( s.substr( 0, s.size() - 4 ), p );
		}
	}
}

bool AssetManager::LoadVertexShader( std::string_view vertexShaderName, std::filesystem::path vertexShaderFilepath ) noexcept(!_DEBUG)
{
	if( m_assets<VertexShader>.contains( vertexShaderName.data() ) )
	{
		std::println( "Vertex Shader {} is already loaded.", vertexShaderName.data() );
		return false;
	}

	m_assets<VertexShader>[vertexShaderName.data()] =
	{
		.asset = std::make_shared<VertexShader>( m_device, vertexShaderFilepath ),
		.path = vertexShaderFilepath,
	};

	std::println( "Loaded {} : {}", vertexShaderName.data(), vertexShaderFilepath.string() );

	return true;
}

bool AssetManager::LoadPixelShader( std::string_view pixelShaderName, std::filesystem::path pixelShaderFilepath ) noexcept(!_DEBUG)
{
	if( m_assets<PixelShader>.contains( pixelShaderName.data() ) )
	{
		std::println( "Pixel Shader {} is already loaded.", pixelShaderName.data() );
		return false;
	}

	m_assets<PixelShader>[pixelShaderName.data()] =
	{
		.asset = std::make_shared<PixelShader>( m_device, pixelShaderFilepath ),
		.path = pixelShaderFilepath,
	};

	std::println( "Loaded {} : {}", pixelShaderName.data(), pixelShaderFilepath.string() );

	return true;
}

bool AssetManager::LoadMesh( std::string_view meshName, std::span<Vertex> vertices, std::span<uint32_t> indices ) noexcept(!_DEBUG)
{
	if( m_assets<Mesh>.contains( meshName.data() ) )
	{
		std::println( "Mesh {} is already loaded.", meshName.data() );
		return false;
	}

	m_assets<Mesh>[meshName.data()] = {
		.asset = std::make_shared<Mesh>( m_device, vertices, indices ),
		.path = ""
	};

	return true;
}

bool AssetManager::LoadSampler( std::string_view samplerName ) noexcept(!_DEBUG)
{
	if( m_assets<Sampler>.contains( samplerName.data() ) )
	{
		std::println( "Sampler {} is already loaded.", samplerName.data() );
		return false;
	}

	m_assets<Sampler>[samplerName.data()] = {
		.asset = std::make_shared<Sampler>( m_device, static_cast<uint16_t>(m_assets<Sampler>.size()) ),
		.path = ""
	};

	return true;
}

bool AssetManager::LoadTexture( std::string_view textureName, std::filesystem::path textureFilepath ) noexcept(!_DEBUG)
{
	if( m_assets<Texture>.contains( textureName.data() ) )
	{
		std::println( "Texture {} is already loaded.", textureName.data() );
		return false;
	}

	Image img = {};
	const HRESULT hr = DirectX::LoadFromWICFile(
		textureFilepath.c_str(),
		DirectX::WIC_FLAGS_NONE,
		&img.texMetadata,
		img.scratchImage
	);
	assert( !FAILED( hr ) );


	m_assets<Texture>[textureName.data()] =
	{
		// BUG: When subtracting 1 from the .size() method, it causes an underflow because the element is not 
		// fully constructed yet, which causes size() to return 0 and subtracting 1 results in an underflow (-1)
		.asset = std::make_shared<Texture>( m_device, std::move( img ), static_cast<uint16_t>(m_assets<Texture>.size()) ),
		.path = textureFilepath,
	};

	std::println( "Loaded {} : {}", textureName.data(), textureFilepath.string() );

	return true;
}

bool AssetManager::LoadMaterial( std::string_view materialName,
								 std::string_view vertexShaderName, std::string_view pixelShaderName,
								 std::string_view textureName, std::string_view samplerName,
								 DirectX::XMFLOAT3 color, float shininess ) noexcept(!_DEBUG)
{
	auto materialIt = m_assets<Material>.find( materialName.data() );

	if( m_assets<Material>.contains( materialName.data() ) )
	{
		std::println( "Material {} is already loaded.", materialName.data() );
		return false;
	}

	auto vertexShaderIt = m_assets<VertexShader>.find( vertexShaderName.data() );
	auto pixelShaderIt = m_assets<PixelShader>.find( pixelShaderName.data() );
	auto textureIt = m_assets<Texture>.find( textureName.data() );
	auto samplerIt = m_assets<Sampler>.find( samplerName.data() );

	assert( vertexShaderIt != m_assets<VertexShader>.end() );
	assert( pixelShaderIt != m_assets<PixelShader>.end() );
	assert( textureIt != m_assets<Texture>.end() );
	assert( samplerIt != m_assets<Sampler>.end() );

	m_assets<Material>[materialName.data()] = {
		.asset = std::make_shared<Material>( vertexShaderIt->second.asset,
											 pixelShaderIt->second.asset,
											 textureIt->second.asset,
											 samplerIt->second.asset,
											 color,
											 shininess ),
		.path = "",
	};

	return true;
}
