#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_pInstance = this;
}
void MyEntityManager::Release(void)
{
	for (int i = 0; i < m_uEntityCount; ++i)
	{
		delete m_entityList[i];
	}
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
	delete m_pInstance;
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (a_uIndex == (uint)-1)
	{
		a_uIndex = m_entityList.size() - 1;
	}
	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	return MyEntity::GetEntity(a_sUniqueID)->GetModel();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	return MyEntity::GetEntity(a_sUniqueID)->GetRigidBody();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (a_uIndex == (uint)-1)
	{
		a_uIndex = m_entityList.size() - 1;
	}
	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	return MyEntity::GetEntity(a_sUniqueID)->GetModelMatrix();
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	MyEntity::GetEntity(a_sUniqueID)->SetModelMatrix(a_m4ToWorld);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (a_uIndex == (uint)-1)
	{
		//for (std::vector<MyEntity*>::iterator it = m_entityList.begin(); it != m_entityList.end(); ++it)
		//{
		//	(*it)->SetModelMatrix(a_m4ToWorld);
		//}
		m_entityList[m_entityList.size() - 1]->SetModelMatrix(a_m4ToWorld);
	}
	else
	{
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
	for (int i = 0; i < m_entityList.size() -1; ++i)
	{
		for (int j = i + 1; j < m_entityList.size(); ++j)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	MyEntity* newEntity = new MyEntity(a_sFileName, a_sUniqueID);
	m_entityList.push_back(newEntity);
	++m_uEntityCount;
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (m_uEntityCount == 0)
	{
		return;
	}
	std::vector<MyEntity*>::iterator iter = m_entityList.begin() + a_uIndex;
	m_entityList.erase(iter, iter);
	--m_uEntityCount;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	MyEntity* toRemove = MyEntity::GetEntity(a_sUniqueID);
	std::vector<MyEntity*>::iterator it = m_entityList._Make_iterator(&toRemove);
	m_entityList.erase(it, it);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	return m_entityList.at(a_uIndex)->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	return m_entityList.at(a_uIndex);
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_uIndex == (uint)-1)
	{
		for (std::vector<MyEntity*>::iterator it = m_entityList.begin(); it != m_entityList.end(); it++)
		{
			(*it)->AddToRenderList(a_bRigidBody);
		}
	}
	else
	{
		m_entityList[a_uIndex]->AddToRenderList();
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	MyEntity::GetEntity(a_sUniqueID)->AddToRenderList(a_bRigidBody);
}