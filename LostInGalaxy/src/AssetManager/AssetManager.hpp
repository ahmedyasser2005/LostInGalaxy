#pragma once

#include "pch.h"
#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"

template<typename T>
concept AssetType =
std::same_as<T, VertexShader> ||
std::same_as<T, PixelShader> ||
std::same_as<T, Mesh> ||
std::same_as<T, Texture> ||
std::same_as<T, Sampler> ||
std::same_as<T, Material>;

template<AssetType T>
struct Asset {
	std::shared_ptr<T> asset;
	std::filesystem::path path;
};

class AssetManager final {
public:
	AssetManager( GraphicsDevice* graphicsDevice ) noexcept(!_DEBUG);
	~AssetManager() noexcept = default;
	AssetManager( const AssetManager& ) = delete;
	AssetManager& operator=( const AssetManager& ) = delete;
	AssetManager( AssetManager&& ) = delete;
	AssetManager& operator=( AssetManager&& ) = delete;

	void LoadAllShadersFromDirectory( std::filesystem::path shadersDirectory ) noexcept(!_DEBUG);

	bool LoadVertexShader( std::string_view vertexShaderName, std::filesystem::path vertexShaderFilepath ) noexcept(!_DEBUG);
	bool LoadPixelShader( std::string_view pixelShaderName, std::filesystem::path pixelShaderFilepath ) noexcept(!_DEBUG);
	bool LoadMesh( std::string_view meshName, std::span<Vertex> vertices, std::span<uint32_t> indices ) noexcept(!_DEBUG);
	bool LoadSampler( std::string_view samplerName ) noexcept(!_DEBUG);
	bool LoadTexture( std::string_view textureName, std::filesystem::path textureFilepath ) noexcept(!_DEBUG);
	bool LoadMaterial( std::string_view materialName,
					   std::string_view vertexShaderName, std::string_view pixelShaderName,
					   std::string_view textureName, std::string_view samplerName,
					   DirectX::XMFLOAT3 color, float shininess ) noexcept(!_DEBUG);

	template<AssetType T> bool UnloadAsset( std::string_view assetName ) noexcept(!_DEBUG);
	template<AssetType T> void UnloadUnusedAssets() noexcept(!_DEBUG);
	template<AssetType T> std::shared_ptr<T> GetAsset( std::string_view assetName ) const noexcept(!_DEBUG);
	template<AssetType T> [[nodiscard]] const std::vector<std::string_view> GetAssets() const noexcept(!_DEBUG);

private:
	GraphicsDevice* m_device;

	template<AssetType T>
	inline static std::unordered_map<std::string, Asset<T>> m_assets;
};



template<AssetType T>
inline bool AssetManager::UnloadAsset( std::string_view assetName ) noexcept(!_DEBUG)
{
	auto it = m_assets<T>.find( assetName.data() );

	if( it == m_assets<T>.end() )
	{
		std::println( "Asset {} doesn't exist", assetName.data() );
		return false;
	}

	it->second.asset.reset();
	m_assets<T>.erase( it );
	std::println( "Unloaded material {}", assetName.data() );
	return true;
}

template<AssetType T>
inline void AssetManager::UnloadUnusedAssets() noexcept(!_DEBUG)
{
	uint32_t counter = 0u;
	for( auto it = m_assets<T>.begin(); it != m_assets<T>.end(); ++it )
	{
		if( it->second.asset.use_count() <= 1 )
		{
			it->second.asset.reset();
			m_assets<T>.erase( it );

			++counter;
		}
	}

	std::println( "Unloaded {} unused asset.", counter );
}

template<AssetType T>
inline std::shared_ptr<T> AssetManager::GetAsset( std::string_view assetName ) const noexcept(!_DEBUG)
{
	auto it = m_assets<T>.find( assetName.data() );

	if( it == m_assets<T>.end() )
	{
		return nullptr;
	}

	return it->second.asset;
}

template<AssetType T>
[[nodiscard]] inline const std::vector<std::string_view> AssetManager::GetAssets() const noexcept(!_DEBUG)
{
	std::vector<std::string_view> assets;

	for( const auto& element : m_assets<T> )
		assets.emplace_back( element.first );

	return assets;
}
