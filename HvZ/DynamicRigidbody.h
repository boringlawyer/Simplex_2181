#pragma once
#include "Definitions.h"
#include "MyRigidBody.h"
using namespace Simplex;
class DynamicRigidbody : MyRigidBody
{
public:
	DynamicRigidbody(std::vector<vector3> a_pointList);
	~DynamicRigidbody();
	void AddForce(vector3 newForce);
	vector3 GetPosition();
	vector3 GetVelocity();
	void TrackHuman();
	void MakeZombie();
	void Update();
private:
	vector3 position;
	vector3 velocity;
	DynamicRigidbody* target;
	bool isHuman;
};

