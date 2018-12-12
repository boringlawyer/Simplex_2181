#include "Creeper.h"
#include "MyEntityManager.h"

Creeper::Creeper(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID) : DynamicEntity(pos, vel, a_sFileName, a_sUniqueID)
{
}


Creeper::~Creeper()
{
}

void Creeper::Update()
{
	DynamicEntity* steve = MyEntityManager::GetInstance()->GetDynamicEntity("Steve");
	if (steve != nullptr)
	{
		ApplyForce(steve->GetPosition() - position);
	}
	DynamicEntity::Update();
}
