#include "DynamicEntity.h"
#include "MyEntityManager.h"
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
	velocity += newForce *.05f/** SystemSingleton::GetInstance()->GetDeltaTime(uClock)*/;
	//position += velocity * SystemSingleton::GetInstance()->GetDeltaTime(uClock);
	//SetModelMatrix(glm::translate(position));
}

void DynamicEntity::Update()
{
	if (m_sUniqueID == "Creeper")
	{
		DynamicEntity* steve = MyEntityManager::GetInstance()->GetDynamicEntity("Steve");
		if (steve != nullptr)
		{
			ApplyForce(steve->GetPosition() - position);
		}
	}
	else if (m_sUniqueID == "Steve")
	{
	}
	position += velocity * SystemSingleton::GetInstance()->GetDeltaTime(uClock);
	SetModelMatrix(glm::translate(position));
}

vector3 DynamicEntity::GetPosition()
{
	return position;
}

vector3 DynamicEntity::GetVelocity()
{
	return velocity;
}
