#include "SoundHolder.h"
#include "FileCheck.h"		// for FileExists()

static constexpr auto SOUNDS_DIRECTORY = "Sounds/";
static constexpr auto AUDIO_FILE_EXTENSION = ".wav";

sf::SoundBuffer& SoundHolder::GetSoundBuffer(std::string const& filepath)
{
	// Get reference to m_Buffers using the only instance
	auto& m = Instance().m_Buffers;

	// Search for key using given filepath
	auto key = m.find(filepath);

	// Found match ?
	if (key != m.end())
	{
		// Return buffer
		return key->second;
	}
	// Not found
	else
	{
		// Create new pair
		auto& buffer = m[filepath];
		buffer.loadFromFile(filepath);
		return buffer;
	}
}

std::string SoundHolder::GetPath(std::string filename, std::string directory)
{
	return SOUNDS_DIRECTORY + directory + filename + AUDIO_FILE_EXTENSION;
}

void SoundHolder::Play(std::unique_ptr<sf::Sound>& rpSound, std::string filepath, bool loop)
{
	if (FileExists(filepath))
	{
		if (rpSound == nullptr) { rpSound = std::make_unique<sf::Sound>(); }

		rpSound->setBuffer(SoundHolder::GetSoundBuffer(filepath));
		rpSound->setLoop(loop);
		rpSound->play();
	}
}