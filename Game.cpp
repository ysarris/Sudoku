#include "Game.h"
#include "StartScreen.h"	// for StartScreen as first screen
#include "FileCheck.h"		// for FileExists()
#include "SavedData_info.h"	// for GetGameSavePath()
#include "Tile_info.h"		// for TILE_SIZE_PIXELS
#include "Window_info.h"	// for WINDOW_HEIGHT_TILES and WINDOW_WIDTH_TILES

static constexpr auto BACKGROUND_MUSIC_FILENAME = "ScaryMusic";
static constexpr auto WINDOW_WIDTH_PIXELS = static_cast<unsigned int>(WINDOW_WIDTH_TILES * TILE_SIZE_PIXELS);
static constexpr auto WINDOW_HEIGHT_PIXELS = static_cast<unsigned int>(WINDOW_HEIGHT_TILES * TILE_SIZE_PIXELS);
static constexpr auto PLAYER_START_POSX = WINDOW_WIDTH_PIXELS * CENTER_ALIGNMENT;
static constexpr auto PLAYER_START_POSY = WINDOW_HEIGHT_PIXELS * CENTER_ALIGNMENT;
static constexpr auto ERROR_BACKGROUND_MUSIC_NO_FILE = "The background music file does not exist - provide correct filename or place file in correct diretory.";

Game::Game()
{
	// Delete the old log file, only in Debug mode
#if !defined(NDEBUG)
	DeleteLogFile();
#endif

	// Create window
	m_Window.create(sf::VideoMode(WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS), WINDOW_TITLE, sf::Style::Close);
	m_Window.setPosition(sf::Vector2i(m_Window.getPosition().x, 0));
	m_Window.setFramerateLimit(WINDOW_FRAME_RATE_LIMIT);
	m_Icon.loadFromFile(TextureHolder::GetPath(WINDOW_ICON_FILENAME));
	m_Window.setIcon(m_Icon.getSize().x, m_Icon.getSize().y, m_Icon.getPixelsPtr());
	m_Window.requestFocus();

	// Load tile sprite sheet
	m_TileTextures = TextureHolder::GetTexture(TextureHolder::GetPath(TILE_SHEET_FILENAME));

	PlayBackgroundMusic(BACKGROUND_MUSIC_FILENAME);

	// Create player
	mp_Player = std::make_unique<Player>(sf::Vector2f(PLAYER_START_POSX, PLAYER_START_POSY));

	CreateGates();

	// Supply window size to screens
	Screen::SetWindowSize(static_cast<sf::Vector2f>(m_Window.getSize()));

	// Supply player to enemies 
	Enemy::SetPlayer(mp_Player.get());

	// Supply projectile vector to weapons
	Weapon::SetProjectiles(&m_Projectiles);

	// Supply difficulty and gates vector to gates
	Gate::SetDifficulty(&m_Difficulty);
	Gate::SetGates(&m_Gates);

	LoadHighscores();

	// Show start screen
	m_Screens.emplace_back(std::make_unique<StartScreen>(FileExists(GetGameSavePath())));
	mp_CurrentScreen = m_Screens.back().get();
}

void Game::Run()
{
	sf::Clock clock;
	sf::Time deltaTime;

	while (m_Window.isOpen())
	{
		// Get elapsed time since last frame and restart clock
		deltaTime = clock.restart();
		
		// Execute main game functions
		Input();
		Update(deltaTime.asSeconds());
		Draw();
	}
}

bool Game::HasWon()
{
	return m_SudokuCompleted;
}

bool Game::HasLost()
{
	return m_TimeRemaining.RanOut() || mp_Player->GetStatus() != HealthStatus::ALIVE;
}

void Game::Pause()
{
	mp_Player->Pause();
	for (const auto& gate : m_Gates) { gate->Pause(); }
	for (const auto& enemy : m_Enemies) { enemy->Pause(); }
	for (const auto& projectile : m_Projectiles) { projectile->Pause(); }
	for (const auto& collectable : m_Collectables) { collectable->Pause(); }
}

void Game::Resume()
{
	mp_Player->Resume();
	for (const auto& gate : m_Gates) { gate->Resume(); }
	for (const auto& enemy : m_Enemies) { enemy->Resume(); }
	for (const auto& projectile : m_Projectiles) { projectile->Resume(); }
	for (const auto& collectable : m_Collectables) { collectable->Resume(); }
}

void Game::Close()
{
	// Clear vectors
	m_Gates.clear();
	m_Enemies.clear();
	m_Projectiles.clear();
	m_Collectables.clear();

	// Reset unique pointers to delete the objects they're pointing to
	mp_Player.reset();
	mp_SudokuBoard.reset();

	m_Window.close();
}

void Game::PlayBackgroundMusic(std::string filename)
{
	auto path = SoundHolder::GetPath(filename);

	if (FileExists(path))
	{
		m_Music.openFromFile(path);
		m_Music.setLoop(true);
		m_Music.play();
	}
	else { Assert(ERROR_BACKGROUND_MUSIC_NO_FILE); }
}