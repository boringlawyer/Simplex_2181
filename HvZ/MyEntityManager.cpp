#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
}
void MyEntityManager::Release(void)
{
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
	
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//Iterate through the array
	for (uint i = 0; i < m_uEntityCount; i++) 
	{
		if (a_sUniqueID == m_entityList[i]->GetUniqueID())
		{
			return i;
		}
	}

	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	// If the entity list is empty, return a null pointer
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	// Make sure you are in bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	// Get the entity with the intended ID
	MyEntity* tempPtr = MyEntity::GetEntity(a_sUniqueID);

	// Make sure tempPtr points to an actual value
	if (tempPtr)
	{
		return tempPtr->GetModel();
	}

	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	// If the entity list is empty, return a null pointer
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	// Make sure you are in bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	// Get the entity with the intended ID
	MyEntity* tempPtr = MyEntity::GetEntity(a_sUniqueID);

	// Make sure tempPtr points to an actual value
	if (tempPtr)
	{
		return tempPtr->GetRigidBody();
	}

	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	// If the entity list is empty, return the identity matrix
	if (m_entityList.size() == 0)
	{
		return IDENTITY_M4;
	}

	// Make sure you are in bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	return m_entityList[a_uIndex]->GetModelMatrix();
	
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	// Get the entity with the intended ID
	MyEntity* tempPtr = MyEntity::GetEntity(a_sUniqueID);

	// Make sure tempPtr points to an actual value
	if (tempPtr)
	{
		return tempPtr->GetModelMatrix();
	}

	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	// Get the entity with the intended ID
	MyEntity* tempPtr = MyEntity::GetEntity(a_sUniqueID);

	// Make sure tempPtr points to an actual value
	if (tempPtr)
	{
		tempPtr->SetModelMatrix(a_m4ToWorld);
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex, bool a_bSetDynamic)
{
	// If the entity list is empty, leave the method
	if (m_entityList.size() == 0 && m_dynamicEntityList.size() == 0)
	{
		return;
	}

	// Make sure you are in bounds
	if (a_bSetDynamic)
	{
		if (a_uIndex >= m_uDynamicEntityCount)
		{
			a_uIndex = m_uDynamicEntityCount - 1;
		}
		m_dynamicEntityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
	}
	else
	{
		if (a_uIndex >= m_uEntityCount)
		{
			a_uIndex = m_uEntityCount - 1;
		}
		m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
	}	
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	// Check for collisions here
	for (uint i = 0; i < m_uEntityCount - 1; i++)
	{
		if (m_uEntityCount == 0)
		{
			break;
		}
		for (uint j = 0; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
	for (int i = 0; i < m_uDynamicEntityCount; ++i)
	{
		m_dynamicEntityList[i]->Update();
	}
	for (uint i = 0; i < m_uDynamicEntityCount - 1; i++)
	{
		if (m_uDynamicEntityCount == 0)
		{
			break;
		}
		for (uint j = 0; j < m_uDynamicEntityCount; j++)
		{
			if (m_dynamicEntityList[i]->IsColliding(m_dynamicEntityList[j]))
			{
				if (m_dynamicEntityList[i]->GetUniqueID() == m_dynamicEntityList[j]->GetUniqueID())
				{
					continue;
				}
				m_dynamicEntityList[i]->GetRigidBody()->SetColorColliding(C_RED);
				m_dynamicEntityList[j]->GetRigidBody()->SetColorColliding(C_RED);
				m_dynamicEntityList[i]->GetRigidBody()->SetColorNotColliding(C_WHITE);
				m_dynamicEntityList[j]->GetRigidBody()->SetColorNotColliding(C_WHITE);
			}
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	// Create an entity to store the new
	MyEntity* tempPtr = new MyEntity(a_sFileName, a_sUniqueID);

	// If tempPtr points to an actual value, add it to the list
	if (tempPtr)
	{
		m_entityList.push_back(tempPtr);
		m_uEntityCount = m_entityList.size();
	}
}
void Simplex::MyEntityManager::AddDynamicEntity(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID)
{
	DynamicEntity* tempPtr = new DynamicEntity(pos, vel, a_sFileName, a_sUniqueID);
	if (tempPtr)
	{
		m_dynamicEntityList.push_back(tempPtr);
		m_uDynamicEntityCount = m_dynamicEntityList.size();
	}
}
void Simplex::MyEntityManager::AddSteve(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID)
{
	Steve* temp = new Steve(pos, vel, a_sFileName, a_sUniqueID);
	if (temp)
	{
		m_dynamicEntityList.push_back(temp);
		m_uDynamicEntityCount = m_dynamicEntityList.size();
	}
}
void Simplex::MyEntityManager::AddCreeper(vector3 pos, vector3 vel, String a_sFileName, String a_sUniqueID)
{
	Creeper* temp = new Creeper(pos, vel, a_sFileName, a_sUniqueID);
	if (temp)
	{
		m_dynamicEntityList.push_back(temp);
		m_uDynamicEntityCount = m_dynamicEntityList.size();
	}

}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	// If the list is empty, there are no entities to remove
	if (m_entityList.size() == 0)
	{
		return;
	}

	// Make sure you are in bounds
	if (a_uIndex >= m_uEntityCount)
	{
		a_uIndex = m_uEntityCount - 1;
	}

	// We need the entity to be at the back for pop_back() to remove it
	// Swap the intended entity with the one at the back of the list
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
	}

	// Now, just remove the entity at the back of the list
	m_entityList.pop_back();
	m_uEntityCount--;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int nIndex = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)nIndex);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	// If the list is empty, leave the method
	if (m_entityList.size() == 0)
	{
		return "";
	}

	// Make sure you are in bounds
	if (a_uIndex >= m_entityList.size())
	{
		a_uIndex = m_entityList.size();
	}

	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	// If the list is empty, return a null pointer
	if (m_entityList.size() == 0)
	{
		return nullptr;
	}

	// Make sure you are in bounds
	if (a_uIndex >= m_entityList.size())
	{
		a_uIndex = m_entityList.size();
	}

	return m_entityList[a_uIndex];
}
DynamicEntity * Simplex::MyEntityManager::GetDynamicEntity(uint a_uIndex)
{
	if (m_dynamicEntityList.size() == 0)
	{
		return nullptr;
	}

	// Make sure you are in bounds
	if (a_uIndex >= m_dynamicEntityList.size())
	{
		a_uIndex = m_entityList.size();
	}
	return m_dynamicEntityList[a_uIndex];
}
DynamicEntity * Simplex::MyEntityManager::GetDynamicEntity(String a_uId)
{
	if (m_dynamicEntityList.size() == 0)
	{
		return nullptr;
	}

	for (int i = 0; i < m_dynamicEntityList.size(); ++i)
	{
		if (m_dynamicEntityList[i]->GetUniqueID() == a_uId)
		{
			return m_dynamicEntityList[i];
		}
	}
	return nullptr;
}

void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	// If you are out of bounds, add all entities to the render list
	if (a_uIndex >= m_uEntityCount)
	{
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; ++a_uIndex)
		{
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
		for (a_uIndex = 0; a_uIndex < m_uDynamicEntityCount; ++a_uIndex)
		{
			m_dynamicEntityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	// If you are in bounds, only the intended entity is rendered
	else
	{
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->AddToRenderList(a_bRigidBody);
	}
}