#include "TextureHolder.h"
#include "FileCheck.h"		// for FileExists()

static constexpr auto GRAPHICS_DIRECTORY = "Graphics/";
static constexpr auto IMAGE_FILE_EXTENSION = ".png";
static constexpr auto SMOOTH_TEXTURE_MIN_SIZE = 550;

sf::Texture& TextureHolder::GetTexture(std::string const& filepath)
{
	// Get reference to m_Textures using the only instance
	auto& m = Instance().m_Textures;

	// Search for key using given filepath
	auto key = m.find(filepath);
	
	// Found match ?
	if (key != m.end())
	{
		// Return texture
		return key->second;
	}
	// Not found
	else
	{
		// Create new pair
		auto& texture = m[filepath];
		texture.loadFromFile(filepath);
		// Smooth large textures
		if (texture.getSize().x >= SMOOTH_TEXTURE_MIN_SIZE || texture.getSize().y >= SMOOTH_TEXTURE_MIN_SIZE)
			texture.setSmooth(true);

		return texture;
	}
}

std::string TextureHolder::GetPath(std::string filename, std::string directory)
{
	return GRAPHICS_DIRECTORY + directory + filename + IMAGE_FILE_EXTENSION;
}

void TextureHolder::Set(sf::Sprite& rSprite, std::string filepath, sf::Vector2f pos)
{
	if (FileExists(filepath))
	{
		rSprite = sf::Sprite(TextureHolder::GetTexture(filepath));

		// Center the origin
		rSprite.setOrigin(0.5f * rSprite.getGlobalBounds().width, 0.5f * rSprite.getGlobalBounds().height);

		rSprite.setPosition(pos);
	}
}