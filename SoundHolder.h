#pragma once
#ifndef SOUND_HOLDER_H
#define SOUND_HOLDER_H

#include <SFML/Audio.hpp>
#include <map>

static constexpr auto VOLUME_MUTE = 0.0f;
static constexpr auto VOLUME_MAX = 100.0f;

class SoundHolder
{
public:
	SoundHolder() = default;
	static sf::SoundBuffer& GetSoundBuffer(std::string const& filepath);
	static std::string GetPath(std::string filename, std::string directory = "");
	static void Play(std::unique_ptr<sf::Sound>& rpSound, std::string filepath, bool loop = false);

	// Delete these to ensure singleton
	SoundHolder(SoundHolder&&) = delete;			// move ctor
	SoundHolder(SoundHolder const&) = delete;		// copy ctor
	void operator=(SoundHolder&&) = delete;			// move assignment
	void operator=(SoundHolder const&) = delete;	// copy assignment

private:
	// A map container to hold filepaths and their corresponding SoundBuffers
	std::map<std::string, sf::SoundBuffer> m_Buffers;

	// Meyer's Singleton
	static SoundHolder& Instance()
	{
		static SoundHolder instance;
		return instance;
	}
};

#endif