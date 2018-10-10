#include "AppClass.h"

vector3** stopsList;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Caleb Katzenstein - cnk5777@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis) 
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	// holds the stops for every sphere
	stopsList = new vector3*[m_uOrbits];
	// the angle used to make the stop
	float angle = 0;
	// the change in the angle each iteration of the second loop
	float angleDelta = 0;
	// initialize the 2D array. The outer array will be for each orbit, and the inner
	// array will be the stops in each orbit
	for (int i = 0, j = 3; i < m_uOrbits; ++i, ++j)
	{
		stopsList[i] = new vector3[j];
		angleDelta = (2 * PI) / j;
		for (int k = 0; k < j; k++, angle += angleDelta)
		{
			vector3 debug = vector3(cos(angle), sin(angle), 0);
			stopsList[i][k] = vector3(cos(angle), sin(angle), 0) * (1 + (i * .5f));
		}
	}
	
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Z);

	//Get timers for sphere movement and orbit scaling
	static float fOrbitTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fOrbitTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer
	// do the same with the orbit scale timer
	static uint uScaleClock = m_pSystem->GenClock();
	static float fScaleTimer = 1;
	static bool scaleIncreasing = true;
	// if fScaleTimer is greater than 2, decrement it until it is less than 1, and vice versa.
	if (scaleIncreasing)
	{
		fScaleTimer += m_pSystem->GetDeltaTime(uScaleClock);
	}
	else
	{
		fScaleTimer -= m_pSystem->GetDeltaTime(uScaleClock);
	}

	if (fScaleTimer > 2)
	{
		scaleIncreasing = false;
	}
	if (fScaleTimer < 1)
	{
		scaleIncreasing = true;
	}

	// indirectly measures what point a sphere is interpolating towards
	static unsigned int stopIndex = 0;
	// if two seconds pass, reset timer and increment stopIndex
	if (fOrbitTimer > 2)
	{
		fOrbitTimer = 0;
		++stopIndex;
	}
	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		// scale each of the orbit's transform by vector3(fScaleTimer) so they cyclically
		// scale and shrink
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X) * glm::scale(vector3(fScaleTimer, fScaleTimer, fScaleTimer)));

		//calculate the current position
		// i represents the orbit itself. By using modulus, we ensure that the 2nd index will 
		// always be in range. i.e. on the 3rd orbit with 5 sides, stop index will between 0 and 4.
		// The "a" value in glm::lerp is fOrbitTimer divided by 2 because it takes 2 seconds 
		// to travel between stops. 
		// Multiplying by fScaleTimer causes the paths to expand or shrink along with the orbits.
		vector3 v3CurrentPos = glm::lerp(stopsList[i][stopIndex % (i + 3)], stopsList[i][((stopIndex + 1) % (i + 3))], fOrbitTimer / 2) * fScaleTimer;
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	for (int i = 0; i < m_uOrbits; ++i)
	{
		delete stopsList[i];
	}
	delete[] stopsList;
	//release GUI
	ShutdownGUI();
}