#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
	invader = new MyMesh[46];
	for (int i = 0; i < 46; ++i)
	{
		invader[i].GenerateCube(1);
	}
	invaderPositions[0] = vector3(2, 7, 0);
	invaderPositions[1] = vector3(8, 7, 0);
	invaderPositions[2] = vector3(3, 6, 0);
	invaderPositions[3] = vector3(7, 6, 0);
	invaderPositions[4] = vector3(2, 5, 0);
	invaderPositions[5] = vector3(3, 5, 0);
	invaderPositions[6] = vector3(4, 5, 0);
	invaderPositions[7] = vector3(5, 5, 0);
	invaderPositions[8] = vector3(6, 5, 0);
	invaderPositions[9] = vector3(7, 5, 0);
	invaderPositions[10] = vector3(8, 5, 0);
	invaderPositions[11] = vector3(1, 4, 0);
	invaderPositions[12] = vector3(2, 4, 0);
	invaderPositions[13] = vector3(4, 4, 0);
	invaderPositions[14] = vector3(5, 4, 0);
	invaderPositions[15] = vector3(6, 4, 0);
	invaderPositions[16] = vector3(8, 4, 0);
	invaderPositions[17] = vector3(9, 4, 0);
	invaderPositions[18] = vector3(0, 3, 0);
	invaderPositions[19] = vector3(1, 3, 0);
	invaderPositions[20] = vector3(2, 3, 0);
	invaderPositions[21] = vector3(3, 3, 0);
	invaderPositions[22] = vector3(4, 3, 0);
	invaderPositions[23] = vector3(5, 3, 0);
	invaderPositions[24] = vector3(6, 3, 0);
	invaderPositions[25] = vector3(7, 3, 0);
	invaderPositions[26] = vector3(8, 3, 0);
	invaderPositions[27] = vector3(9, 3, 0);
	invaderPositions[28] = vector3(10, 3, 0);
	invaderPositions[29] = vector3(0, 2, 0);
	invaderPositions[30] = vector3(2, 2, 0);
	invaderPositions[31] = vector3(3, 2, 0);
	invaderPositions[32] = vector3(4, 2, 0);
	invaderPositions[33] = vector3(5, 2, 0);
	invaderPositions[34] = vector3(6, 2, 0);
	invaderPositions[35] = vector3(7, 2, 0);
	invaderPositions[36] = vector3(8, 2, 0);
	invaderPositions[37] = vector3(10, 2, 0);
	invaderPositions[38] = vector3(0, 1, 0);
	invaderPositions[39] = vector3(2, 1, 0);
	invaderPositions[40] = vector3(8, 1, 0);
	invaderPositions[41] = vector3(10, 1, 0);
	invaderPositions[42] = vector3(3, 0, 0);
	invaderPositions[43] = vector3(4, 0, 0);
	invaderPositions[44] = vector3(6, 0, 0);
	invaderPositions[45] = vector3(7, 0, 0);
	
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	m_pMesh->Render(m4Projection, m4View, m4Model);
	
	for (int i = 0; i < 46; ++i)
	{
		m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f) + invaderPositions[i]);
		matrix4 m4Model = m4Scale * m4Translate;
		invader[i].Render(m4Projection, m4View, m4Model);
	}
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}