#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

class TextureHolder
{
public:
	TextureHolder() = default;
	static sf::Texture& GetTexture(std::string const& filepath);
	static std::string GetPath(std::string filename, std::string directory = "");
	static void Set(sf::Sprite& rSprite, std::string filepath, sf::Vector2f pos);

	// Delete these to ensure singleton
	TextureHolder(TextureHolder&&) = delete;		// move ctor
	TextureHolder(TextureHolder const&) = delete;	// copy ctor
	void operator=(TextureHolder&&) = delete;		// move assignment
	void operator=(TextureHolder const&) = delete;	// copy assignment

private:
	// A map container to hold filepaths and their corresponding Textures
	std::map<std::string, sf::Texture> m_Textures;
	
	// Meyer's Singleton
	static TextureHolder& Instance()
	{
		static TextureHolder instance;
		return instance;
	}
};

#endif