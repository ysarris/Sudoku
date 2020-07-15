#include "GameplayControls.h"
#include "ErrorLogging.h"		// for Assert()

static constexpr auto SHOOT_WEAPON_KEY = sf::Keyboard::LControl;
static constexpr auto SWAP_WEAPON_KEY = sf::Keyboard::Tab;
static constexpr auto MOVE_UP_KEY = sf::Keyboard::Up;
static constexpr auto MOVE_DOWN_KEY = sf::Keyboard::Down;
static constexpr auto MOVE_LEFT_KEY = sf::Keyboard::Left;
static constexpr auto MOVE_RIGHT_KEY = sf::Keyboard::Right;
static constexpr auto CLEAR_CELL_KEY = sf::Keyboard::BackSpace;
static constexpr auto ERROR_NOT_DIGIT_KEY = "Not a digit key - use IsDigitKey() to check key first";
static constexpr auto ERROR_NOT_DIRECTION_KEY = "Not a direction key - use IsDirectionKey() to check key first";

bool IsShootKeyPressed()
{
	return sf::Keyboard::isKeyPressed(SHOOT_WEAPON_KEY);
}

bool IsMoveUpKeyPressed()
{
	return sf::Keyboard::isKeyPressed(MOVE_UP_KEY);
}

bool IsMoveDownKeyPressed()
{
	return sf::Keyboard::isKeyPressed(MOVE_DOWN_KEY);
}

bool IsMoveLeftKeyPressed()
{
	return sf::Keyboard::isKeyPressed(MOVE_LEFT_KEY);
}

bool IsMoveRightKeyPressed()
{
	return sf::Keyboard::isKeyPressed(MOVE_RIGHT_KEY);
}

bool IsSwapWeaponKey(sf::Keyboard::Key key)
{
	return key == SWAP_WEAPON_KEY;
}

bool IsClearCellKey(sf::Keyboard::Key key)
{
	return key == CLEAR_CELL_KEY;
}

// Returns whether the given keyboard key is a digit key
bool IsDigitKey(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Num1:
	case sf::Keyboard::Numpad1:
	case sf::Keyboard::Num2:
	case sf::Keyboard::Numpad2:
	case sf::Keyboard::Num3:
	case sf::Keyboard::Numpad3:
	case sf::Keyboard::Num4:
	case sf::Keyboard::Numpad4:
	case sf::Keyboard::Num5:
	case sf::Keyboard::Numpad5:
	case sf::Keyboard::Num6:
	case sf::Keyboard::Numpad6:
	case sf::Keyboard::Num7:
	case sf::Keyboard::Numpad7:
	case sf::Keyboard::Num8:
	case sf::Keyboard::Numpad8:
	case sf::Keyboard::Num9:
	case sf::Keyboard::Numpad9:
		return true;
		break;
	default:
		return false; //not a digit key
		break;
	}
}

// Returns whether the given key is a direction key
bool IsDirectionKey(sf::Keyboard::Key key)
{
	switch (key)
	{
	case MOVE_UP_KEY:
	case MOVE_DOWN_KEY:
	case MOVE_LEFT_KEY:
	case MOVE_RIGHT_KEY:
		return true;
		break;
	// Not a direction key
	default:
		return false;
		break;
	}
}

// Returns a digit given a key
int GetDigitForKey(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Num1:
	case sf::Keyboard::Numpad1:
		return 1;
		break;
	case sf::Keyboard::Num2:
	case sf::Keyboard::Numpad2:
		return 2;
		break;
	case sf::Keyboard::Num3:
	case sf::Keyboard::Numpad3:
		return 3;
		break;
	case sf::Keyboard::Num4:
	case sf::Keyboard::Numpad4:
		return 4;
		break;
	case sf::Keyboard::Num5:
	case sf::Keyboard::Numpad5:
		return 5;
		break;
	case sf::Keyboard::Num6:
	case sf::Keyboard::Numpad6:
		return 6;
		break;
	case sf::Keyboard::Num7:
	case sf::Keyboard::Numpad7:
		return 7;
		break;
	case sf::Keyboard::Num8:
	case sf::Keyboard::Numpad8:
		return 8;
		break;
	case sf::Keyboard::Num9:
	case sf::Keyboard::Numpad9:
		return 9;
		break;
	default:
		Assert(ERROR_NOT_DIGIT_KEY);
		break;
	}
}

// Returns a direction given a key
Direction GetDirectionForKey(sf::Keyboard::Key key)
{
	switch (key)
	{
	case MOVE_UP_KEY:
		return Direction::UP;
		break;
	case MOVE_DOWN_KEY:
		return Direction::DOWN;
		break;
	case MOVE_LEFT_KEY:
		return Direction::LEFT;
		break;
	case MOVE_RIGHT_KEY:
		return Direction::RIGHT;
		break;
	default:
		Assert(ERROR_NOT_DIRECTION_KEY);
		break;
	}
}