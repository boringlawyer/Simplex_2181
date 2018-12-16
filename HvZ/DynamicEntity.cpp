#include "DynamicEntity.h"

DynamicEntity::DynamicEntity(MyEntity * entityParam, vector3 pos, vector3 vel)
{
	entity = entityParam;
	position = pos;
	velocity = vel;
	entity->SetModelMatrix(entity->GetModelMatrix() * glm::translate(position));
}

MyEntity * DynamicEntity::GetEntity()
{
	return entity;
}

DynamicEntity::~DynamicEntity()
{
	delete entity;
}

void DynamicEntity::ApplyForce(vector3 newForce)
{
	velocity += newForce;
}

void DynamicEntity::Update()
{
	position += velocity;
	entity->SetModelMatrix(glm::translate(position));
}
