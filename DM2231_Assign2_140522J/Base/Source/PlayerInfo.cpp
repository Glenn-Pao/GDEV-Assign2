#include "PlayerInfo.h"
#include "Application.h"
#include <iostream>

CPlayerInfo::CPlayerInfo(void)
	: hero_inMidAir_Up(false)
	, hero_inMidAir_Down(false)
	, jumpspeed(0)
	, heroAnimationCounter(0)
	, heroAnimationInvert(false)
	, mapOffset_x(0)
	, mapOffset_y(0)
	, mapFineOffset_x(0)
	, mapFineOffset_y(0)
	, movingLeft(false)
	, movingRight(false)
	, ActiveLeft(true)			//default is left so this is initialized to be true
	, ActiveRight(false)
	, isOnJumpUpwards(false)
	, attacking(false)
{
}
CPlayerInfo::~CPlayerInfo(void)
{
}
void CPlayerInfo::setMapOffset_x(int mapOffset_x)
{
	this->mapOffset_x = mapOffset_x;
}
void CPlayerInfo::setMapOffset_y(int mapOffset_y)
{
	this->mapFineOffset_y = mapOffset_y;
}
void CPlayerInfo::Init(void)
{
	theHeroPosition.x=0;
	theHeroPosition.y=0;
}
bool CPlayerInfo::isOnGround(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
		return true;

	return false;
}
bool CPlayerInfo::isJumpUpwards(void)
{
	if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
		return true;

	return false;
}
bool CPlayerInfo::isFreeFall(void)
{
	if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
		return true;

	return false;
}
void CPlayerInfo::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
		jumpspeed = 0;
	}
}
void CPlayerInfo::setAttack(bool attacking)
{
	//for reference on the get function
	this->attacking = attacking;

	if(hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
		if (attacking == true)
		{
			hero_inMidAir_Up = true;
			hero_inMidAir_Down = false;
			jumpspeed = 6;
		}
	}
}
bool CPlayerInfo::getAttack(void)
{
	return attacking;
}
void CPlayerInfo::SetToJumpUpwards(bool isOnJumpUpwards)
{
	//for reference on the get function
	this->isOnJumpUpwards = isOnJumpUpwards;

	if(hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
		if (isOnJumpUpwards == true)
		{
			hero_inMidAir_Up = true;
			hero_inMidAir_Down = false;
			jumpspeed = 12;
		}
	}
}
bool CPlayerInfo::GetToJumpUpwards(void)
{
	return isOnJumpUpwards;
}
void CPlayerInfo::SetPos_x(int pos_x)
{
	theHeroPosition.x = pos_x;
}
void CPlayerInfo::SetPos_y(int pos_y)
{
	theHeroPosition.y = pos_y;
}
void CPlayerInfo::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}
void CPlayerInfo::SetToStop(void)
{
	hero_inMidAir_Up = false;
	hero_inMidAir_Down = false;
	jumpspeed = 0;
}
void CPlayerInfo::MoveUpDown(const bool mode, const float timeDiff)
{
	if (mode)
	{
		theHeroPosition.y = theHeroPosition.y + (int) (5.0f * timeDiff);
	}
	else
	{
		theHeroPosition.y = theHeroPosition.y - (int) (5.0f * timeDiff);
	}
}
void CPlayerInfo::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (mode)
	{
		theHeroPosition.x = theHeroPosition.x - (int) (5.0f * timeDiff);
		heroAnimationInvert = true;
		heroAnimationCounter--;
		if (heroAnimationCounter==0)
			heroAnimationCounter = 3;
	}
	else
	{
		theHeroPosition.x = theHeroPosition.x + (int) (5.0f * timeDiff);
		heroAnimationInvert = false;
		heroAnimationCounter++;
		if (heroAnimationCounter>3)
			heroAnimationCounter = 0;
	}
}
int CPlayerInfo::GetPos_x(void)
{
	return theHeroPosition.x;
}
int CPlayerInfo::GetPos_y(void)
{
	return theHeroPosition.y;
}
int CPlayerInfo::GetJumpspeed(void)
{
	return jumpspeed;
}
int CPlayerInfo::GetMapOffset_x(void)
{
	return mapOffset_x;
}
int CPlayerInfo::GetMapOffset_y(void)
{
	return mapOffset_y;
}
int CPlayerInfo::GetMapFineOffset_x(void)
{
	return mapFineOffset_x;
}
int CPlayerInfo::GetMapFineOffset_y(void)
{
	return mapFineOffset_y;
}
void CPlayerInfo::UpdateJumpUpwards()
{
	theHeroPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
		hero_inMidAir_Up = false;
		hero_inMidAir_Down = true;
	}
}
void CPlayerInfo::UpdateFreeFall()
{
	theHeroPosition.y += jumpspeed;
	jumpspeed += 1;
}
void CPlayerInfo::SetAnimationInvert(bool heroAnimationInvert)
{
	this->heroAnimationInvert = heroAnimationInvert;
}
bool CPlayerInfo::GetAnimationInvert(void)
{
	return heroAnimationInvert;
}
void CPlayerInfo::SetAnimationCounter(int heroAnimationCounter)
{
	this->heroAnimationCounter = heroAnimationCounter;
}
int CPlayerInfo::GetAnimationCounter(void)
{
	return heroAnimationCounter;
}
void CPlayerInfo::ConstrainHero(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff)
{
	if (theHeroPosition.x < leftBorder)
	{
		theHeroPosition.x = leftBorder;
		mapOffset_x =  mapOffset_x - (int) (5.0f * timeDiff);
		if (mapOffset_x < 0)
			mapOffset_x = 0;
	}
	else if (theHeroPosition.x > rightBorder)
	{
		theHeroPosition.x = rightBorder;
		mapOffset_x =  mapOffset_x + (int) (5.0f * timeDiff);

		if(SceneAssignment::getControlswap())
		{
			if (mapOffset_x > Application::getWindowWidth()*3)	// This must be changed to soft-coded
				mapOffset_x = Application::getWindowWidth()*3;
		}
		else
		{
			if (mapOffset_x > Application::getWindowWidth())	// This must be changed to soft-coded
				mapOffset_x = Application::getWindowWidth();
		}
		
	}
	if (theHeroPosition.y < topBorder)
	{
		theHeroPosition.y = topBorder;
	}
	else if (theHeroPosition.y > bottomBorder)
	{
		theHeroPosition.y = bottomBorder;
	}
}
void CPlayerInfo::HeroUpdate(CMap* m_cMap)
{
	// Update Hero's info
  	if (hero_inMidAir_Up == false && hero_inMidAir_Down == false)
	{
		// Don't jump
		CPlayerInfo::SetToJumpUpwards(false);
		CPlayerInfo::setAttack(false);
	
		//free fall when there are no tiles to hold the character
		int checkPosition_X = (int) ((mapOffset_x+theHeroPosition.x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theHeroPosition.y + m_cMap->GetTileSize() + jumpspeed) / m_cMap->GetTileSize());
		
		if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] != 1) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] != 1))
		{
			CPlayerInfo::SetOnFreeFall(true);
		}
	}
	else if (hero_inMidAir_Up == true && hero_inMidAir_Down == false)
	{
		// Check if the hero can move up into mid air...
		int checkPosition_X = (int) ((mapOffset_x+theHeroPosition.x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theHeroPosition.y + m_cMap->GetTileSize() + jumpspeed) / m_cMap->GetTileSize());
		if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == 1) )
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			theHeroPosition.y = ((int) (theHeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			hero_inMidAir_Up = false;
			jumpspeed = 0;
		}
		else
		{
			theHeroPosition.y += jumpspeed;
			jumpspeed -= 1;
			if (jumpspeed == 0)
			{
				hero_inMidAir_Up = false;
				hero_inMidAir_Down = true;
			}
		}
	}
	else if (hero_inMidAir_Up == false && hero_inMidAir_Down == true)
	{
		// Check if the hero is still in mid air...
		int checkPosition_X = (int) ((mapOffset_x+theHeroPosition.x) / m_cMap->GetTileSize());
		if (checkPosition_X < 0)
			checkPosition_X = 0;
		if (checkPosition_X > m_cMap->getNumOfTiles_MapWidth())
			checkPosition_X = m_cMap->getNumOfTiles_MapWidth();
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theHeroPosition.y - jumpspeed) / m_cMap->GetTileSize());
		if (checkPosition_Y < 0)
			checkPosition_Y = 0;
		if (checkPosition_Y > m_cMap->GetNumOfTiles_Height())
			checkPosition_Y = m_cMap->GetNumOfTiles_Height();
		if (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == 1 ||
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == 1))
		{
			// Since the new position does not allow the hero to move into, then go back to the old position
			theHeroPosition.y = ((int) (theHeroPosition.y / m_cMap->GetTileSize())) * m_cMap->GetTileSize();
			hero_inMidAir_Down = false;
			jumpspeed = 0;
		}
		else
		{
			theHeroPosition.y -= jumpspeed;
			jumpspeed += 1;
		}
	}

	ConstrainHero(64, 650, 32, 550, 1.0f);

	// Calculate the fine offset
	mapFineOffset_x = mapOffset_x % m_cMap->GetTileSize();

}
void CPlayerInfo::SetActiveLeft(bool ActiveLeft)
{
	this->ActiveLeft = ActiveLeft;
}
bool CPlayerInfo::GetActiveLeft(void)
{
	return ActiveLeft;
}
void CPlayerInfo::SetActiveRight(bool ActiveRight)
{
	this->ActiveRight = ActiveRight;
}
bool CPlayerInfo::GetActiveRight(void)
{
	return ActiveRight;
}
void CPlayerInfo::SetMoveLeft(bool movingLeft)
{
	this->movingLeft = movingLeft;
}
bool CPlayerInfo::GetMoveLeft(void)
{
	return movingLeft;
}
void CPlayerInfo::SetMoveRight(bool movingRight)
{
	this->movingRight = movingRight;
}
bool CPlayerInfo::GetMoveRight(void)
{
	return movingRight;
}
bool CPlayerInfo::checkCollision(CMap *cMap, bool checkleft, bool checkright, bool checkdown, bool checkup)
{
	int tileTopLeft_x = (int)floor ((float) (mapOffset_x + theHeroPosition.x) / cMap->GetTileSize() );
	int tileTopLeft_y = cMap->GetNumOfTiles_Height() - (int)ceil( (float) (theHeroPosition.y + cMap->GetTileSize() + jumpspeed) / cMap->GetTileSize() );
	
	if(checkleft)
	{
		if(cMap->theScreenMap[tileTopLeft_y][tileTopLeft_x] == 1)
			return true;
	}
	if(checkright)
	{
		int tileTopLeft_x = (int) ( (mapOffset_x + theHeroPosition.x) / cMap->GetTileSize() + 1);
		if(cMap->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == 1)
			return true;
	}
	if(checkup)
	{
		if(cMap->theScreenMap[tileTopLeft_y][tileTopLeft_x] == 1)
			return true;
		if(cMap->theScreenMap[tileTopLeft_y][tileTopLeft_x + 1] == 1)
			return true;
	}
	if(checkdown)
	{
		if(cMap->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x] == 1)
			return true;
		if(cMap->theScreenMap[tileTopLeft_y + 1][tileTopLeft_x + 1] == 1)
			return true;
	}
	
	return false;
}