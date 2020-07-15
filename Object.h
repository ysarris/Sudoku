#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "TextureHolder.h"		// for sf::Sprite and functions
#include "SoundHolder.h"		// for sf::Sound and functions
#include "ErrorLogging.h"		// for Assert()

static constexpr auto DEFAULT_SCALE = 1.0f;
static constexpr auto SPRITE_MAX_SIZE = 200.0f;
static constexpr auto CENTER_ALIGNMENT = 0.5f;
static constexpr auto NO_ROTATION = 0.0f;
static inline const auto NO_OFFSET = sf::Vector2f(0.0f, 0.0f);
static inline const auto DEFAULT_POSITION = sf::Vector2f(0.0f, 0.0f);

class Object
{
public:
	sf::Vector2f GetPos() const;
	float GetRotation() const;
	float GetHeight() const;
	float GetWidth() const;
	bool SoundIsPlaying() const;
	bool SoundIsPlaying(const std::unique_ptr<sf::Sound>& rpSound) const;
	bool SoundIsOnLoop() const;
	bool SoundIsOnLoop(const std::unique_ptr<sf::Sound>& rpSound) const;
	virtual void Pause();
	virtual void Resume();
	virtual void Draw(sf::RenderWindow& rWindow) const;

protected:
	Object(sf::Vector2f startPos = DEFAULT_POSITION);
	~Object() noexcept = default;

	bool ImageExists(std::string filename) const;
	bool SoundExists(std::string filename) const;
	const sf::Sprite& GetSprite() const;
	std::string GetDirectory() const;
	void AddToDirectory(std::string path);
	void SetPos(sf::Vector2f pos);
	void SetPosX(float posX);
	void SetPosY(float posY);
	// Sprite functions - overloading allows use of derived class sprites, only for SetSprite()
	void SetSprite(std::string filename, std::string customDirectory = "");
	void SetSprite(sf::Sprite& rSprite, sf::Vector2f pos, std::string filename, std::string customDirectory = "");
	void RotateSpriteBy(float degrees);		// increase/decrease rotation
	void SetSpriteRotation(float degrees);	// replace rotation
	void ScaleSpriteBy(float scaleX, float scaleY);		// increase/decrease scale
	void SetSpriteScale(float scaleX, float scaleY);	// replace scale
	void ChangeSpriteTextureRectangle(float width, float height);
	void FlipSpriteHorizontally();
	void FlipSpriteVertically();
	// Sound functions - overloading allows use of derived class sounds
	void PlaySound(std::string filename, bool loop = false);
	void PlaySound(std::string filename, std::unique_ptr<sf::Sound>& rpSound, bool loop = false);
	void PauseSound();
	void PauseSound(std::unique_ptr<sf::Sound>& rpSound);
	void ResumeSound();
	void ResumeSound(std::unique_ptr<sf::Sound>& rpSound);
	void StopSound();
	void StopSound(std::unique_ptr<sf::Sound>& rpSound);
	void SetSoundVolume(float value);
	void SetSoundVolume(std::unique_ptr<sf::Sound>& rpSound, float value);

private:
	sf::Vector2f m_Pos;
	sf::Sprite m_Sprite;
	std::string m_Directory;
	std::unique_ptr<sf::Sound> mp_Sound;

	virtual void Update(float dtAsSeconds) = 0;
	virtual const std::string GetClassName() const = 0;
};

#endif