#pragma once
#ifndef GATE_H
#define GATE_H

#include "Object.h"
#include "Timer.h"
#include "Direction.h"

enum class GateStatus { OPEN, OPENING, CLOSED, CLOSING };

class Gate : public Object
{
public:
	static void SetDifficulty(int* rDifficulty);
	static void SetGates(std::vector<std::unique_ptr<Gate>>* pGates);
	static void UpdateGates(float dtAsSeconds);
	static void ResetGates();
	static bool AreGatesOpen();
	Gate(sf::Vector2f startPos, Direction exitFacing, int& rTileVal);
	Direction GetExitFacing() const;
	virtual void Pause() final override;
	virtual void Resume() final override;

private:
	static inline std::vector<std::unique_ptr<Gate>>* msp_Gates;
	static inline int ms_TileValue;
	static inline int* msp_Difficulty;
	static inline GateStatus ms_Status;
	static inline Timer ms_StatusChangeTimer;
	int& mr_TileValue;
	Direction m_ExitFacing;

	static void Open();
	static void Close();
	static void NextOpeningClosingFrame();
	static int GetDifficulty();
	static Gate* GetFirstGate(); // only use one gate to play sounds
	virtual void Update(float dtAsSeconds) final override;
	virtual const std::string GetClassName() const final override;
};

#endif