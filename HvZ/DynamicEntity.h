#pragma once
#include "MyEntity.h"
#include "DynamicRigidbody.h"
using namespace Simplex;
class DynamicEntity : public MyEntity
{
private:
	vector3 velocity;
	vector3 position;
	DynamicRigidbody* rb;
//public:
	DynamicEntity(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID = "NA");
//	MyEntity* GetEntity();
	~DynamicEntity();
	void ApplyForce(vector3 newForce);
	void Update();
};

