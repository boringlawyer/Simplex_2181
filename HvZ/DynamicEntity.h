#pragma once
#include "MyEntity.h"
using namespace Simplex;
class DynamicEntity
{
private:
	MyEntity* entity;
	vector3 velocity;
	vector3 position;
public:
	DynamicEntity(MyEntity* entityParam, vector3 pos, vector3 vel);
	MyEntity* GetEntity();
	~DynamicEntity();
	void ApplyForce(vector3 newForce);
	void Update();
};

