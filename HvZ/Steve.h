#pragma once
#include "DynamicEntity.h"
class Steve :
	public DynamicEntity
{
private:
	void Update();
public:
	Steve(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID);
	~Steve();
};

