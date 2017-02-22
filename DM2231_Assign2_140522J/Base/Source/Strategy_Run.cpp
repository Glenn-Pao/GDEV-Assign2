#include "Strategy_Run.h"
#include <iostream>

using namespace std;

CStrategy_Run::CStrategy_Run()
{
}
CStrategy_Run::~CStrategy_Run()
{
}
void CStrategy_Run::Update(void)
{
	theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x < 0 ? 5 : -5 );
	//theEnemyPosition.y = theEnemyPosition.y + (theDestination.y - theEnemyPosition.y > 0 ? 5 : -5 );
}
void CStrategy_Run::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}
void CStrategy_Run::GetEnemyPosition(float& x, float& y)
{
	x = theEnemyPosition.x;
	y = theEnemyPosition.y;
}
void CStrategy_Run::SetEnemyPosition(const float x, const float y)
{
	theEnemyPosition.x = x;
	theEnemyPosition.y = y;
}