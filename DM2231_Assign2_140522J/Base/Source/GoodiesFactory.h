#pragma once
#include "Goodies.h"
#include "TreasureChest.h"
#include "Health.h"

typedef int GoodiesID;
#define TREASURECHEST 0
#define HEALTH 1

class CGoodiesFactory
{
public:
	CGoodiesFactory(void);
	~CGoodiesFactory(void);

	CGoodies* Create(GoodiesID);
};

