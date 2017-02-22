#pragma once
#include "Vector2.h"
#include "Map.h"

class CPlayerInfo
{
public:
	CPlayerInfo(void);
	~CPlayerInfo(void);

	enum GEOMETRY_TYPE
	{
		GEO_TILEHERO_FRAME0,
		GEO_TILEHERO_FRAME1,
		GEO_TILEHERO_FRAME2,
		GEO_TILEHERO_FRAME3,
		NUM_GEOMETRY,
	};

	// Initialise this class instance
	void Init(void);
	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);

	//check if the the jump upwards is already active
	bool GetToJumpUpwards(void);

	// Stop the player's movement
	void SetToStop(void);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveUpDown(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);

	void setMapOffset_x(int mapOffset_x);
	void setMapOffset_y(int mapOffset_y);

	// Get position x of the player
	int GetPos_x(void);
	// Get position y of the player
	int GetPos_y(void);
	// Get Jumpspeed of the player
	int GetJumpspeed(void);
	// Get mapOffset_x
	int GetMapOffset_x(void);
	// Get mapOffset_y
	int GetMapOffset_y(void);
	// Get mapFineOffset_x
	int GetMapFineOffset_x(void);
	// Get mapFineOffset_y
	int GetMapFineOffset_y(void);

	// Update Jump Upwards
	void UpdateJumpUpwards();
	// Update FreeFall
	void UpdateFreeFall();
	// Hero Update
	void HeroUpdate(CMap* m_cMap);

	// Set Animation Invert status of the player
	void SetAnimationInvert(bool heroAnimationInvert);
	// Get Animation Invert status of the player
	bool GetAnimationInvert(void);
	// Set Animation Counter of the player
	void SetAnimationCounter(int heroAnimationCounter);
	// Get Animation Counter of the player
	int GetAnimationCounter(void);

	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder, 
		const int topBorder, const int bottomBorder, 
		float timeDiff);

	bool checkCollision(CMap *cMap, bool checkleft, bool checkright, bool checkdown, bool checkup);

	//check whether the sprite is actively moving
	void SetActiveLeft(bool ActiveLeft);
	bool GetActiveLeft(void);

	void SetActiveRight(bool ActiveRight);
	bool GetActiveRight(void);

	void SetMoveLeft(bool movingLeft);
	bool GetMoveLeft(void);

	void SetMoveRight(bool movingRight);
	bool GetMoveRight(void);

	//code is similar to jump upwards
	void setAttack(bool attacking);
	bool getAttack(void);
private:
	// Hero's information
	Vector2 theHeroPosition;
	int jumpspeed;
	bool hero_inMidAir_Up;
	bool hero_inMidAir_Down;
	bool heroAnimationInvert;
	int heroAnimationCounter;

	bool ActiveLeft;
	bool ActiveRight;
	bool movingLeft;
	bool movingRight;

	bool isOnJumpUpwards;

	bool attacking;

	// For scrolling.
	int mapOffset_x, mapOffset_y;
	int mapFineOffset_x, mapFineOffset_y;
};