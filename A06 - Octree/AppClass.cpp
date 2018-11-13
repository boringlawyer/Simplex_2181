#include "AppClass.h"
#include <vector>
using std::vector;
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up
	octant = new MyOctant(vector3(0, 0, 0), 68);
	octant->Subdivide();
	for (int i = 0; i < 8; ++i)
	{
		octant->GetChild(i)->Subdivide();
	}
	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 900;
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			uIndex++;
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);
		}
	}
	//for (int i = 0; i < m_pEntityMngr->GetEntityCount(); ++i)
	//{
	//	m_pEntityMngr->GetEntity(i)->AddDimension(i);
	//}
	m_uOctantLevels = 2;
	//for (int i = 0; i < m_pEntityMngr->GetEntityCount(); ++i)
	//{
	//	octant->IsColliding(i);
	//}
	octant->ConstructTree();
	m_pEntityMngr->Update();
}
void Application::AssignDimensions()
{
	// create the test rigidbody that, if colliding with boxes, determine the dimension(s) it is in
	//float dimensionSpaceLength = 34 / (m_uOctantLevels + 1);
	//vector<vector3> testRbPoints;
	//testRbPoints.push_back(vector3(-dimensionSpaceLength / 2, dimensionSpaceLength / 2, dimensionSpaceLength / 2));
	//testRbPoints.push_back(vector3(dimensionSpaceLength / 2, dimensionSpaceLength / 2, dimensionSpaceLength / 2));
	//testRbPoints.push_back(vector3(dimensionSpaceLength / 2, dimensionSpaceLength / 2, -dimensionSpaceLength / 2));
	//testRbPoints.push_back(vector3(-dimensionSpaceLength / 2, dimensionSpaceLength / 2, -dimensionSpaceLength / 2));
	//testRbPoints.push_back(vector3(dimensionSpaceLength / 2, -dimensionSpaceLength / 2, dimensionSpaceLength / 2));
	//testRbPoints.push_back(vector3(-dimensionSpaceLength / 2, -dimensionSpaceLength / 2, dimensionSpaceLength / 2));
	//testRbPoints.push_back(vector3(dimensionSpaceLength / 2, -dimensionSpaceLength / 2, -dimensionSpaceLength / 2));
	//testRbPoints.push_back(vector3(-dimensionSpaceLength / 2, -dimensionSpaceLength / 2, -dimensionSpaceLength / 2));
	//MyRigidBody testRb(testRbPoints);
	//testRb.SetModelMatrix(glm::translate(vector3(-dimensionSpaceLength, -dimensionSpaceLength, -dimensionSpaceLength)));
	//int dimCount = 0;
	//for (int z = 0; z < pow(2, m_uOctantLevels); ++z)
	//{
	//	for (int y = 0; y < pow(2, m_uOctantLevels); ++y)
	//	{
	//		for (int x = 0; x < pow(2, m_uOctantLevels); ++x)
	//		{
	//			for (int i = 0; i < m_pEntityMngr->GetEntityCount(); ++i)
	//			{
	//				if (testRb.IsColliding(m_pEntityMngr->GetEntity(i)->GetRigidBody()))
	//				{
	//					m_pEntityMngr->GetEntity(i)->AddDimension(dimCount);
	//				}
	//			}
	//			++dimCount;
	//			testRb.SetModelMatrix(testRb.GetModelMatrix() * glm::translate(vector3(dimensionSpaceLength, 0, 0)));
	//		}
	//		testRb.SetModelMatrix(testRb.GetModelMatrix() * glm::translate(vector3(0, dimensionSpaceLength, 0)));
	//	}
	//	testRb.SetModelMatrix(testRb.GetModelMatrix() * glm::translate(vector3(0, 0, dimensionSpaceLength)));
	//}
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); ++i)
	{
		if (m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal().x < 0)
		{
			if (m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal().y < 0)
			{
				m_pEntityMngr->GetEntity(i)->AddDimension(0);
			}
			else
			{
				m_pEntityMngr->GetEntity(i)->AddDimension(1);
			}
		}
		else
		{
			if (m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal().y < 0)
			{
				m_pEntityMngr->GetEntity(i)->AddDimension(2);
			}
			else
			{
				m_pEntityMngr->GetEntity(i)->AddDimension(3);
			}
		}
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	octant->Display(C_YELLOW);
	//m_pRoot->Display();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}