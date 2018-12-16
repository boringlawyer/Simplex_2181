#pragma once
#include "MyEntity.h"
#include "DynamicRigidbody.h"
using namespace Simplex;
class DynamicEntity : public MyEntity
{
private:
	DynamicRigidbody* rb;
	int uClock;
protected:
	vector3 position;
	vector3 velocity;
public:
	DynamicEntity(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID = "NA");
	~DynamicEntity();
	void ApplyForce(vector3 newForce);
	virtual void Update();
	vector3 GetPosition();
	vector3 GetVelocity();
};

