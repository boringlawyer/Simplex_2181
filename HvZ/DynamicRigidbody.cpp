#include "DynamicRigidbody.h"



DynamicRigidbody::DynamicRigidbody(std::vector<vector3> a_pointList) : MyRigidBody::MyRigidBody(a_pointList)
{
	
}


DynamicRigidbody::~DynamicRigidbody()
{
}

void DynamicRigidbody::AddForce(vector3 newForce)
{
	velocity += newForce;
}

vector3 DynamicRigidbody::GetPosition()
{
	return position;
}

vector3 DynamicRigidbody::GetVelocity()
{
	return velocity;
}

void DynamicRigidbody::Update()
{
	position += velocity;
}
