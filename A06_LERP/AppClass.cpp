#include "AppClass.h"

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");
	
	fDuration = 1.0f;

	// populate the std::vector for all the points we will use
	pointsInSpace.push_back(vector3(-4.0f, -2.0f, 5.0f));
	pointsInSpace.push_back(vector3(1.0f, -2.0f, 5.0f));
	pointsInSpace.push_back(vector3(-3.0f, -1.0f, 3.0f));
	pointsInSpace.push_back(vector3(2.0f, -1.0f, 3.0f));
	pointsInSpace.push_back(vector3(-2.0f, 0.0f, 0.0f));
	pointsInSpace.push_back(vector3(3.0f, 0.0f, 0.0f));
	pointsInSpace.push_back(vector3(-1.0f, 1.0f, -3.0f));
	pointsInSpace.push_back(vector3(4.0f, 1.0f, -3.0f));
	pointsInSpace.push_back(vector3(0.0f, 2.0f, -5.0f));
	pointsInSpace.push_back(vector3(5.0f, 2.0f, -5.0f));
	pointsInSpace.push_back(vector3(1.0f, 3.0f, -5.0f));

	// create a sphere and it's matrix
	m_pSphere = new PrimitiveClass();
	m_m4Sphere = IDENTITY_M4;

	// create a small red sphere
	m_pSphere->GenerateSphere(0.1f, 5, RERED);
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	// value to keep track of what path we are going to take
	static uint indexer = 0;
	static uint indexer2 = 1;

	// check if we change the path
	if (fRunTime > fDuration)
	{
		// increment the counters to change the path
		indexer++;
		indexer2++;

		// if the first indexer is the last value of the vector, then the second indexer should be set to the first value of the vector
		// if the indexer would go past the size of the vector, set it to 0
		if (indexer+1 == pointsInSpace.size())
		{
			indexer2 = 0;
		}
		
		if (indexer+1 > pointsInSpace.size())
		{
			indexer = 0;
		}

		fRunTime = 0;
	}

	vector3 v3Start(pointsInSpace[indexer]);
	vector3 v3End(pointsInSpace[indexer2]);
	vector3 v3Current;

	float fPercent = MapValue(
		static_cast<float>(fRunTime),	// value to change
		0.0f,							// original min
		static_cast<float>(fDuration),	// original max
		0.0f,							// new min
		1.0f);							// new max

	v3Current = glm::lerp(v3Start, v3End, fPercent); // last variable goes from 0 - 1

	m_m4Guy = glm::translate(v3Current);
	m_pMeshMngr->SetModelMatrix(m_m4Guy, "WallEye");


#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	// store the projection matrix into a value
	matrix4 projection = m_pCameraMngr->GetProjectionMatrix();

	// store the view matrix into a value
	matrix4 view = m_pCameraMngr->GetViewMatrix();

	// draw the spheres at the point locations
	for (uint i = 0; i < pointsInSpace.size(); i++)
	{
		m_m4Sphere = glm::translate(pointsInSpace[i]); // set the sphere to be at the point locations
		m_pSphere->Render(projection, view, m_m4Sphere); // draw the spheres
	}

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}