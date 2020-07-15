#include "Object.h"
#include "FileCheck.h"	// for FileExists()

static constexpr auto ERROR_NO_SPRITE = "A sprite hasnt been assigned yet - cannot return height/width.";
static constexpr auto ERROR_SPRITE_TOO_BIG = "Sprite used is too big. Use smaller sprite or scale down.";
static constexpr auto DIRECTORY_CHAR = "/";

Object::Object(sf::Vector2f startPos) :
	m_Pos(startPos)
{
}

sf::Vector2f Object::GetPos() const
{
	return m_Pos;
}

float Object::GetRotation() const
{
	return m_Sprite.getRotation();
}

float Object::GetHeight() const
{
	Assert(ERROR_NO_SPRITE, m_Sprite.getGlobalBounds().height > 0);
	Assert(ERROR_SPRITE_TOO_BIG, m_Sprite.getGlobalBounds().height <= SPRITE_MAX_SIZE);
	return m_Sprite.getGlobalBounds().height;
}

float Object::GetWidth() const
{
	Assert(ERROR_NO_SPRITE, m_Sprite.getGlobalBounds().width > 0);
	Assert(ERROR_SPRITE_TOO_BIG, m_Sprite.getGlobalBounds().width <= SPRITE_MAX_SIZE);
	return m_Sprite.getGlobalBounds().width;
}

bool Object::ImageExists(std::string filename) const
{
	return FileExists(TextureHolder::GetPath(filename, GetDirectory()));
}

bool Object::SoundExists(std::string filename) const
{
	return FileExists(SoundHolder::GetPath(filename, GetDirectory()));
}

const sf::Sprite& Object::GetSprite() const
{
	return m_Sprite;
}

std::string Object::GetDirectory() const
{
	return m_Directory + GetClassName() + DIRECTORY_CHAR;
}

void Object::AddToDirectory(std::string path)
{
	m_Directory.append(path).append(DIRECTORY_CHAR);
}

void Object::SetPos(sf::Vector2f pos)
{
	m_Pos = pos;
	m_Sprite.setPosition(m_Pos);
}

void Object::SetPosX(float posX)
{
	m_Pos.x = posX;
	m_Sprite.setPosition(m_Pos);
}

void Object::SetPosY(float posY)
{
	m_Pos.y = posY;
	m_Sprite.setPosition(m_Pos);
}

void Object::SetSprite(std::string filename, std::string customDirectory)
{
	SetSprite(m_Sprite, m_Pos, filename, (customDirectory == "" ? GetDirectory() : customDirectory));
}

void Object::SetSprite(sf::Sprite& rSprite, sf::Vector2f pos, std::string filename, std::string customDirectory)
{
	// Store scale temporarily, use absolute values to get rid of any flips
	auto scaleX = abs(GetSprite().getScale().x);
	auto scaleY = abs(GetSprite().getScale().y);
	TextureHolder::Set(rSprite, TextureHolder::GetPath(filename, (customDirectory == "" ? GetDirectory() : customDirectory)), pos);
	// Restore to previous scale
	rSprite.setScale(scaleX, scaleY);
}

void Object::RotateSpriteBy(float degrees)
{
	m_Sprite.setRotation(m_Sprite.getRotation() + degrees);
}

void Object::SetSpriteRotation(float degrees)
{
	m_Sprite.setRotation(degrees);
}

void Object::ScaleSpriteBy(float scaleX, float scaleY)
{
	auto scale = m_Sprite.getScale();
	// Subtract scale if sprite was flipped
	m_Sprite.setScale(scale.x + (scale.x < 0 ? -scaleX : scaleX), scale.y + (scale.y < 0 ? -scaleY : scaleY));
}

void Object::SetSpriteScale(float scaleX, float scaleY)
{
	m_Sprite.setScale(scaleX, scaleY);
}

void Object::ChangeSpriteTextureRectangle(float width, float height)
{
	m_Sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));
}

void Object::FlipSpriteHorizontally()
{
	m_Sprite.setScale(-m_Sprite.getScale().x, m_Sprite.getScale().y);
}

void Object::FlipSpriteVertically()
{
	m_Sprite.setScale(m_Sprite.getScale().x, -m_Sprite.getScale().y);
}

bool Object::SoundIsPlaying() const
{
	return SoundIsPlaying(mp_Sound);
}

bool Object::SoundIsPlaying(const std::unique_ptr<sf::Sound>& rpSound) const
{
	// False if no sound pointed to or not playing
	return (rpSound == nullptr) ? false : (rpSound->sf::Sound::getStatus() == sf::Sound::Playing);
}

bool Object::SoundIsOnLoop() const
{
	return SoundIsOnLoop(mp_Sound);
}

bool Object::SoundIsOnLoop(const std::unique_ptr<sf::Sound>& rpSound) const
{
	// False if no sound pointed to or not on loop
	return (rpSound == nullptr) ? false : (rpSound->getLoop());
}

void Object::Pause()
{
	PauseSound();
}

void Object::Resume()
{
	ResumeSound();
}

void Object::PlaySound(std::string filename, bool loop)
{
	// Use the default object sound pointer
	PlaySound(filename, mp_Sound, loop);
}

void Object::PlaySound(std::string filename, std::unique_ptr<sf::Sound>& rpSound, bool loop)
{
	SoundHolder::Play(rpSound, SoundHolder::GetPath(filename, GetDirectory()), loop);
}

void Object::PauseSound()
{
	PauseSound(mp_Sound);
}

void Object::PauseSound(std::unique_ptr<sf::Sound>& rpSound)
{
	if (rpSound != nullptr) { if (SoundIsPlaying(rpSound)) { rpSound->pause(); } }
}

void Object::ResumeSound()
{
	ResumeSound(mp_Sound);
}

void Object::ResumeSound(std::unique_ptr<sf::Sound>& rpSound)
{
	if (rpSound != nullptr) { if (rpSound->sf::Sound::getStatus() == sf::Sound::Paused) { rpSound->play(); } }
}

void Object::StopSound()
{
	StopSound(mp_Sound);
}

void Object::StopSound(std::unique_ptr<sf::Sound>& rpSound)
{
	if (rpSound != nullptr) { rpSound->stop(); }
}

void Object::SetSoundVolume(float value)
{
	SetSoundVolume(mp_Sound, value);
}

void Object::SetSoundVolume(std::unique_ptr<sf::Sound>& rpSound, float value)
{
	if (rpSound != nullptr) { rpSound->setVolume(value); }
}

void Object::Draw(sf::RenderWindow& rWindow) const
{
	rWindow.draw(m_Sprite);
}