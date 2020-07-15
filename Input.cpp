#include "Game.h"
#include "ScreenControls.h"		// for Screen input functions

void Game::Input()
{
	// Handle game related input only when game window has focus
	// e.g. player controls, menu/screen navigation
	if (m_Window.hasFocus())
	{
		// In game and level over ?
		if (mp_CurrentScreen->GetID() == ScreenID::GAME && (HasWon() || HasLost()))
		{
			// Ignore any events that occur between the level ending and the screen switching
			sf::Event event;
			while (m_Window.pollEvent(event)){}

			// Stop shooting and movement
			mp_Player->StopAllActions();
		}
		// Not in game or in game and level not over yet
		else
		{
			// Real-time input
			mp_CurrentScreen->HandleRealTimeInput();

			// Event-driven input
			sf::Event event;
			while (m_Window.pollEvent(event))
			{
				mp_CurrentScreen->HandleEventInput(event);

				// Quitting game
				if (mp_CurrentScreen->GetID() != ScreenID::QUIT && (WasWindowCloseAttempted(event) || WasQuitKeyPressed(event)))
					SwitchScreen(ScreenID::QUIT);

				// Pausing game
				if ((mp_CurrentScreen->GetID() == ScreenID::GAME && WasPauseKeyPressed(event))
					// Navigating screens
					|| (mp_CurrentScreen->GetID() != ScreenID::GAME && WasSelectKeyPressed(event)))
				{
					if (mp_CurrentScreen->IsReadyToSelect()) { SwitchScreen(mp_CurrentScreen->Select()); }
				}
			}
		}
	}
	// Handle events that can occur when window doesnt have focus
	// e.g. window focus lost/regained, closing window
	else
	{
		sf::Event event;
		while (m_Window.pollEvent(event))
		{
			// Losing window focus during gameplay
			if (mp_CurrentScreen->GetID() == ScreenID::GAME && WasWindowFocusLost(event))
				SwitchScreen(ScreenID::PAUSE);

			// Closing window
			if (mp_CurrentScreen->GetID() != ScreenID::QUIT && WasWindowCloseAttempted(event))
				SwitchScreen(ScreenID::QUIT);
		}
	}
}