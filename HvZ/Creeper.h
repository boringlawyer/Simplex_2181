#pragma once
#include "DynamicEntity.h"
class Creeper :
	public DynamicEntity
{
private:
	void Update();
public:
	Creeper(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID);
	~Creeper();
};

