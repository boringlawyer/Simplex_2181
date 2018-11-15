#include "MyOctant.h"
using namespace Simplex;
uint MyOctant::m_uOctantCount;
uint MyOctant::m_uMaxLevel = 70;
uint MyOctant::m_uIdealEntityCount = 50;
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;
	++m_uOctantCount;
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
	m_v3Min = a_v3Center - (vector3(1, 1, 1) * a_fSize / 2);
	m_v3Max = a_v3Center + (vector3(1, 1, 1) * a_fSize / 2);
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	m_uID = m_uOctantCount;
	++m_uOctantCount;
}

Simplex::MyOctant::MyOctant(MyOctant const & other)
{
}

//MyOctant & Simplex::MyOctant::operator=(MyOctant const & other)
//{
//	// TODO: insert return statement here
//	return this;
//}

Simplex::MyOctant::~MyOctant(void)
{
	for (int i = 0; i < m_uChildren; ++i)
	{
		delete m_pChild[i];
	}
}

uint Simplex::MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

void Simplex::MyOctant::Release(void)
{
}

void Simplex::MyOctant::Init(void)
{
}

void Simplex::MyOctant::ConstructList(void)
{
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
}

void Simplex::MyOctant::ClearEntityList(void)
{
	for (std::vector<uint>::iterator it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
	{
		m_pEntityMngr->GetEntity(*it)->ClearDimensionSet();
	}
	m_EntityList.clear();
	for (int i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i]->ClearEntityList();
	}
}

void Simplex::MyOctant::Subdivide(void)
{
	m_uChildren = 8;
	vector3 extent = m_v3Max - m_v3Center;
	m_pChild[0] = new MyOctant(this->m_v3Center + extent / 2, m_fSize / 2);
	m_pChild[1] = new MyOctant(this->m_v3Center + vector3(-extent.x, extent.y, extent.z) / 2, m_fSize / 2);
	m_pChild[2] = new MyOctant(this->m_v3Center + vector3(-extent.x, extent.y, -extent.z) / 2, m_fSize / 2);
	m_pChild[3] = new MyOctant(this->m_v3Center + vector3(extent.x, extent.y, -extent.z) / 2, m_fSize / 2);
	m_pChild[4] = new MyOctant(this->m_v3Center + vector3(extent.x, -extent.y, extent.z) / 2, m_fSize / 2);
	m_pChild[5] = new MyOctant(this->m_v3Center + vector3(-extent.x, -extent.y, extent.z) / 2, m_fSize / 2);
	m_pChild[6] = new MyOctant(this->m_v3Center + vector3(extent.x, -extent.y, -extent.z) / 2, m_fSize / 2);
	m_pChild[7] = new MyOctant(this->m_v3Center + (-extent / 2), m_fSize / 2);
	for (int i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i]->m_uLevel = this->m_uLevel + 1;
	}
}

MyOctant * Simplex::MyOctant::GetChild(uint a_nChild)
{
	return m_pChild[a_nChild];
}

MyOctant * Simplex::MyOctant::GetParent(void)
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return m_uChildren == 0;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	return m_EntityList.size() > a_nEntities;
}

void Simplex::MyOctant::KillBranches(void)
{
	for (std::vector<uint>::iterator it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
	{
		m_pEntityMngr->GetEntity(*it)->ClearDimensionSet();
	}
	for (int i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i]->KillBranches();
	}
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	int numEntities = 0;
	// for each entity, assign this octant's id to it if they are colliding and if this is a leaf
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); ++i)
	{
		if (IsColliding(i))
		{
			if (IsLeaf())
			{
				m_pEntityMngr->GetEntity(i)->AddDimension(m_uID);
			}
			++numEntities;
			// automatically subdivide if there are too many entities in here and if the max depth has not been reached
			if (numEntities == m_uIdealEntityCount)
			{
				if (m_uLevel >= m_uMaxLevel)
				{
					return;
				}
				else
				{
					Subdivide();
					for (int i = 0; i < m_uChildren; ++i)
					{
						m_pChild[i]->ConstructTree();
					}
					return;
				}
			}
		}
	}
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
}

void Simplex::MyOctant::Swap(MyOctant & other)
{
}

float Simplex::MyOctant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	// does an old-school AABB on the rigidbody and the octant bounds
	MyRigidBody* otherRB = m_pEntityMngr->GetEntity(a_uRBIndex)->GetRigidBody();
	if ((m_v3Min.x <= otherRB->GetMaxGlobal().x && m_v3Max.x >= otherRB->GetMinGlobal().x) && (m_v3Min.y <= otherRB->GetMaxGlobal().y && m_v3Max.y >= otherRB->GetMinGlobal().y) && (m_v3Min.z <= otherRB->GetMaxGlobal().z && m_v3Max.z >= otherRB->GetMinGlobal().z))
	{
		m_EntityList.push_back(a_uRBIndex);
		m_pEntityMngr->GetEntity(a_uRBIndex)->AddDimension(m_uID);
		for (int i = 0; i < m_uChildren; ++i)
		{
			m_pChild[i]->IsColliding(a_uRBIndex);
		}
		return true;
	}
	return false;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::scale(vector3(1, 1, 1) * m_fSize) * glm::translate(m_v3Center), a_v3Color);
	for (int i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i]->Display(a_v3Color);
	}
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_v3Center) * glm::scale(vector3(m_fSize, m_fSize, m_fSize)), a_v3Color);
	for (int i = 0; i < m_uChildren; ++i)
	{
		m_pChild[i]->Display(a_v3Color);
	}
}
