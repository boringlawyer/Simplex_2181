#include "DynamicEntity.h"

DynamicEntity::DynamicEntity(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID) : MyEntity(a_sFileName, a_sUniqueID)
{
	position = pos;
	velocity = vel;
	uClock = SystemSingleton::GetInstance()->GenClock();
}
//
//MyEntity * DynamicEntity::GetEntity()
//{
//	return entity;
//}
//
DynamicEntity::~DynamicEntity()
{
	delete this;
}

void DynamicEntity::ApplyForce(vector3 newForce)
{
	velocity += newForce * SystemSingleton::GetInstance()->GetDeltaTime(uClock);
	position += velocity * SystemSingleton::GetInstance()->GetDeltaTime(uClock);
	SetModelMatrix(glm::translate(position));
}

void DynamicEntity::Update()
{
	position += velocity * SystemSingleton::GetInstance()->GetDeltaTime(uClock);
	SetModelMatrix(glm::translate(position));
}
