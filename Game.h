#pragma once
#ifndef GAME_H
#define GAME_H

#include "Screen.h"
#include "Timer.h"
#include "Gate.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Collectable.h"
#include "Board.h"			// also includes "Cell.h"
#include "Player.h"			// also includes "Actor.h", "Movable.h", "Object.h", "TextureHolder.h" and "SoundHolder.h"
#include "ErrorLogging.h"	// for Assert() and DeleteLogFile()
#include "Window_info.h"	// for WINDOW_HEIGHT_TILES and WINDOW_WIDTH_TILES

class Game
{
public:
	Game();
	void Run();
	// Meyer's Singleton
	static Game& Instance()
	{
		static Game instance;
		return instance;
	}

	// Delete these to ensure singleton
	Game(Game&&) = delete;					// move ctor
	Game(Game const&) = delete;				// copy ctor
	void operator=(Game&&) = delete;		// move assignment
	void operator=(Game const&) = delete;	// copy assignment

private:
	// Resource holders
	TextureHolder m_TextureHolder;
	SoundHolder m_SoundHolder;

	// Window
	sf::RenderWindow m_Window;
	sf::Image m_Icon;

	// Screens
	std::vector<std::unique_ptr<Screen>> m_Screens;
	Screen* mp_CurrentScreen;

	// Tiles
	sf::VertexArray m_Tiles;
	sf::Texture m_TileTextures;
	int m_HighlightedTileIndex;
	int m_TileLayout[WINDOW_HEIGHT_TILES][WINDOW_WIDTH_TILES] =
	{
		{2,2,2,2,2,3,2,2,2,2,2},
		{2,0,0,0,0,0,0,0,0,0,2},
		{2,0,0,0,0,0,0,0,0,0,2},
		{2,0,0,0,0,0,0,0,0,0,2},
		{2,0,0,0,0,0,0,0,0,0,2},
		{3,0,0,0,0,0,0,0,0,0,3},
		{2,0,0,0,0,0,0,0,0,0,2},
		{2,0,0,0,0,0,0,0,0,0,2},
		{2,0,0,0,0,0,0,0,0,0,2},
		{2,0,0,0,0,0,0,0,0,0,2},
		{2,2,2,2,2,3,2,2,2,2,2}
	};

	// Game objects
	std::unique_ptr<Player> mp_Player;
	std::vector<std::unique_ptr<Gate>> m_Gates;
	std::vector<std::unique_ptr<Enemy>> m_Enemies;
	std::vector<std::unique_ptr<Projectile>> m_Projectiles;
	std::vector<std::unique_ptr<Collectable>> m_Collectables;

	// Sudoku board
	std::unique_ptr<Board> mp_SudokuBoard = std::make_unique<Board>();
	bool m_SudokuCompleted;

	// Scores
	int m_Difficulty, m_Level, m_TotalScore, m_KillsScore, m_SolutionScore, m_TimeScore;
	std::vector <std::pair<std::string, int>> m_Highscores;

	// Sounds
	sf::Music m_Music;
	std::unique_ptr<sf::Sound> mp_SelectSound;
	std::unique_ptr<sf::Sound> mp_LevelEndSound;
	
	// Timers & Counts
	Timer m_TimeRemaining;
	Timer m_HealthPackSpawnTimer;
	Timer m_SmallEnemySpawnTimer;
	int m_LargeEnemySpawnCount;

	// Main functions
	void Draw();
	void Input();
	void Update(float dtAsSeconds);

	// Collision management
	void DetectCollisions();

	// Screen management
	void SwitchScreen(ScreenID newScreenID);

	// Saved data management
	void SaveGame();
	void LoadSave();
	void DeleteSave();
	void LoadHighscores();
	void SaveNewHighscore(std::string name);

	// Tile management
	void CreateGates();
	void SetTVAfromTLA();
	void HighlightPlayerTile();
	sf::Vector2f GetRandomIncorrectCellPos() const;

	// Game management
	bool HasWon();
	bool HasLost();
	void Pause();
	void Resume();
	void Close();
	void PlayBackgroundMusic(std::string filename);

	// Level management
	void NewGame();
	void CleanupLevel();
	void PrepareLevel();
};

#endif