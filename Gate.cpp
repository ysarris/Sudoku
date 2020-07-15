#include "Gate.h"
#include "Tile_info.h"			// for gate tile values
#include "Difficulty_info.h"	// for ValidateDifficulty()

static constexpr auto GATE_CLASS_NAME = "Gate";
static constexpr auto GATE_SOUND_FILENAME_OPEN = "Open";
static constexpr auto GATE_SOUND_FILENAME_CLOSED = "Closed";
static constexpr auto GATE_SOUND_FILENAME_OPENING_CLOSING = "Opening-Closing";
static constexpr auto GATE_ROTATION_FACING_RIGHT = -90;
static constexpr auto GATE_ROTATION_FACING_LEFT = 90;
static constexpr auto GATE_ROTATION_FACING_DOWN = 0;
static constexpr auto GATE_ROTATION_FACING_UP = 180;
static constexpr auto GATE_ANIM_DELAY = 0.5f;
static constexpr auto GATE_TIME_CLOSED_BASE = 50.0f;
static constexpr auto GATE_TIME_CLOSED_DIFFICULTY_MULTIPLIER = 10.0f;
static constexpr auto GATE_TIME_OPEN_BASE = 10.0f;
static constexpr auto GATE_TIME_OPEN_DIFFICULTY_MULTIPLIER = 10.0f;
static constexpr auto GATE_ERROR_NO_DIFFICULTY_POINTER = "A pointer to the difficulty value has not been supplied - use SetDifficulty() function.";
static constexpr auto GATE_ERROR_NO_GATES_POINTER = "A pointer to the gates vector has not been supplied - use SetGates() function.";
static constexpr auto GATE_ERROR_NO_GATES = "The gates vector is empty - no gate to play sound.";

void Gate::SetDifficulty(int* pDifficulty)
{
	msp_Difficulty = pDifficulty;
}

void Gate::SetGates(std::vector<std::unique_ptr<Gate>>* pGates)
{
	msp_Gates = pGates;
}

void Gate::UpdateGates(float dtAsSeconds)
{
	// Time to change status ?
	if (ms_StatusChangeTimer.RanOut())
	{
		// What was the old status ?
		switch (ms_Status)
		{
		case GateStatus::OPEN:
			ms_Status = GateStatus::CLOSING;
			break;
		case GateStatus::CLOSED:
			ms_Status = GateStatus::OPENING;
			break;
		case GateStatus::OPENING:
		case GateStatus::CLOSING:
			// Update tile value accordingly
			(ms_Status == GateStatus::OPENING) ? ms_TileValue++ : ms_TileValue--;

			// Finished closing ?
			if (ms_TileValue == CLOSED_GATE_TILE_VALUE) { Close(); }
			// Finished opening ?
			else if (ms_TileValue == OPEN_GATE_TILE_VALUE) { Open(); }
			// Still opening/closing
			else { NextOpeningClosingFrame(); }
			break;
		default:
			break;
		}
	}
	else { ms_StatusChangeTimer.Decrement(dtAsSeconds); }

	// Update each gate
	for (auto& gate : *msp_Gates) { gate->Update(dtAsSeconds); }
}

void Gate::ResetGates()
{
	Close();
	GetFirstGate()->StopSound(); // so closed sound doesnt play
	ms_TileValue = CLOSED_GATE_TILE_VALUE;
}

bool Gate::AreGatesOpen()
{
	return ms_Status == GateStatus::OPEN;
}

Gate::Gate(sf::Vector2f startPos, Direction exitFacing, int& rTileValue) :
	Object(startPos),
	mr_TileValue(rTileValue),
	m_ExitFacing(exitFacing)
{
	SetSprite(GATE_CLASS_NAME);
	// Rotate to face correct direction
	if (exitFacing == Direction::RIGHT) { RotateSpriteBy(GATE_ROTATION_FACING_RIGHT); }
	else if (exitFacing == Direction::LEFT) { RotateSpriteBy(GATE_ROTATION_FACING_LEFT); }
	else if (exitFacing == Direction::DOWN) { RotateSpriteBy(GATE_ROTATION_FACING_DOWN); }
	else if (exitFacing == Direction::UP) { RotateSpriteBy(GATE_ROTATION_FACING_UP); }
}

Direction Gate::GetExitFacing() const
{
	return m_ExitFacing;
}

void Gate::Pause()
{
	GetFirstGate()->PauseSound();
}

void Gate::Resume()
{
	GetFirstGate()->ResumeSound();
}

void Gate::Open()
{
	ms_Status = GateStatus::OPEN;
	ms_StatusChangeTimer.Reset(GATE_TIME_OPEN_BASE + (GetDifficulty() * GATE_TIME_OPEN_DIFFICULTY_MULTIPLIER));
	GetFirstGate()->PlaySound(GATE_SOUND_FILENAME_OPEN);
}

void Gate::Close()
{
	ms_Status = GateStatus::CLOSED;
	ms_StatusChangeTimer.Reset(GATE_TIME_CLOSED_BASE - (GetDifficulty() * GATE_TIME_CLOSED_DIFFICULTY_MULTIPLIER));
	GetFirstGate()->PlaySound(GATE_SOUND_FILENAME_CLOSED);
}

void Gate::NextOpeningClosingFrame()
{
	ms_StatusChangeTimer.Reset(GATE_ANIM_DELAY);
	if (!GetFirstGate()->SoundIsPlaying()) { GetFirstGate()->PlaySound(GATE_SOUND_FILENAME_OPENING_CLOSING, true); }
}

int Gate::GetDifficulty()
{
	Assert(GATE_ERROR_NO_DIFFICULTY_POINTER, msp_Difficulty != nullptr );
	ValidateDifficulty(*msp_Difficulty);
	return *msp_Difficulty;
}

Gate* Gate::GetFirstGate()
{
	Assert(GATE_ERROR_NO_GATES_POINTER, msp_Gates != nullptr );
	Assert(GATE_ERROR_NO_GATES, msp_Gates->size() > 0);
	return msp_Gates->begin()->get();
}

void Gate::Update(float dtAsSeconds)
{
	// Need to update tile value ?
	if (mr_TileValue != ms_TileValue) { mr_TileValue = ms_TileValue; }
}

const std::string Gate::GetClassName() const
{
	return GATE_CLASS_NAME;
}