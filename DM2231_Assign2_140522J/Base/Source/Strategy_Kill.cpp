#include "Strategy_Kill.h"
#include <iostream>

using namespace std;

CStrategy_Kill::CStrategy_Kill()
{
}
CStrategy_Kill::~CStrategy_Kill()
{
}
//32 x 32 x 4(tiles)
static float distSquared = 16384.f;
//32 x 32
static float One_Tile_Dist_Squared = 1024.f;
void CStrategy_Kill::Update(void)
{
	//Decide which state to change to
	int distanceHeroToEnemy = CalculateDistance();
	if(distanceHeroToEnemy < distSquared)
	{
		if(distanceHeroToEnemy < One_Tile_Dist_Squared)
			CurrentState = REPEL;
		else
			CurrentState = ATTACK;
	}
	else
		CurrentState = IDLE;

	//Based on the current state, move the enemy
	switch(CurrentState)
	{
	case ATTACK:
		theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? 5 : -5 );
		break;
	case REPEL:
		theEnemyPosition.x = theEnemyPosition.x; //+ (theDestination.x - theEnemyPosition.x > 0 ? 5 : -5 );
		break;
	default:
		//Do nothing if idle
		break;
	}

	//theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? 5 : -5 );
}
void CStrategy_Kill::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}
void CStrategy_Kill::GetEnemyPosition(float& x, float& y)
{
	x = theEnemyPosition.x;
	y = theEnemyPosition.y;
}
void CStrategy_Kill::SetEnemyPosition(const float x, const float y)
{
	theEnemyPosition.x = x;
	theEnemyPosition.y = y;
}
//Get the FSM state for this strategy
CStrategy_Kill::CURRENT_STATE CStrategy_Kill::GetState(void)
{
	return CurrentState;
}
//Set the FSM state for this strategy
void CStrategy_Kill::SetState(CStrategy_Kill::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}