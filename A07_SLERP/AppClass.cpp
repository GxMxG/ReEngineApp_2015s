#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - Greg Bednarowicz"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	// set up matrices for the models
	m4Sun = IDENTITY_M4;
	m4Earth = IDENTITY_M4;
	m4Moon = IDENTITY_M4;


	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();

	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	// make a copy of the current runTime
	double fRunTime_Temp = fRunTime;
	double fRunTime_Temp_M = fRunTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

#pragma region My Code
#pragma region Variables and necessary values
	// const quick access variables for 90, 180, and 360 degrees
	const float rad360 = PI * 2;
	const float rad180 = PI;
	const float rad90 = PI / 2;

	// variables to keep track of orbits and revolutions
	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	// matricies for the earthspace and moonspace calculations
	matrix4 earthspace;
	matrix4 moonspace;

	// distances of the earth and the moon
	matrix4 distanceEarth = glm::translate(11.0f, 0.0f, 0.0f);
	matrix4 distanceMoon = glm::translate(2.0f, 0.0f, 0.0f);

	// scales of the sun, earth, and moon
	matrix4 sunScale = glm::scale(vector3(5.936f, 5.936f, 5.936f));
	matrix4 earthScale = glm::scale(vector3(0.524f, 0.524f, 0.524f));
	matrix4 moonScale = glm::scale(vector3(0.27f, 0.27f, 0.27f));
	
	//matrices for the orbits and revolutions of the earth and moon
	matrix4 earthOrbit;
	matrix4 earthRev;
	matrix4 moonOrbit;

	// assortment of quaternions
	glm::quat earthQuat_Start = glm::quat(vector3(0.0f, 0.0f, 0.0f));
	glm::quat earthQuat_Mid = glm::quat(vector3(0.0f, rad180, 0.0f));
	glm::quat earthQuat_End = glm::quat(vector3(0.0f, rad360, 0.0f));

	glm::quat moonQuat_Start = glm::quat(vector3(0.0f, 0.0f, 0.0f));
	glm::quat moonQuat_Mid = glm::quat(vector3(0.0f, rad180, 0.0f));
	glm::quat moonQuat_End = glm::quat(vector3(0.0f, rad360, 0.0f));

	// value to keep track of which quaternion pair we are using for earth's orbit, either 1 or 2
	static uint indexer_EarthOrbit = 0;
	static uint indexer_EarthRev = 0;
	static uint indexer_MoonOrbit = 0;
#pragma endregion

#pragma region Earth movement
	// ------------------------------------------------------------------------------ ORBIT
	// check if we switch quaternions pairs for ORBIT
	if (fRunTime > fEarthHalfOrbTime)
	{
		// increment the counters to change the pair
		indexer_EarthOrbit++;

		// if the indexer hits 2, reset it to 0 and increment the nEarthOrbits
		if (indexer_EarthOrbit >= 2)
		{
			indexer_EarthOrbit = 0;
			nEarthOrbits++;
		}

		fRunTime = 0;
	}


	// map the value of the time relative to earth's orbit
	float fPercentEarthOrbit = MapValue(
			static_cast<float>(fRunTime),	// value to change
			0.0f,							// original min
			static_cast<float>(fEarthHalfOrbTime),	// original max
			0.0f,							// new min
			1.0f);							// new max


	// calculate earth's ORBIT position depending on quaternion pair
	if (indexer_EarthOrbit == 0)
	{
		glm::quat earthQuat = glm::mix(earthQuat_Start, earthQuat_Mid, fPercentEarthOrbit);
		earthOrbit = glm::mat4_cast(earthQuat);
	}
	else if (indexer_EarthOrbit == 1)
	{
		glm::quat earthQuat = glm::mix(earthQuat_Mid, earthQuat_End, fPercentEarthOrbit);
		earthOrbit = glm::mat4_cast(earthQuat);
	}
	else
	{
		std::cout << "Error in indexer" << std::endl;
	}

	// ------------------------------------------------------------------------------ REV
	// check if we switch quaternions pairs for REV
	if (fRunTime_Temp > fEarthHalfRevTime * nEarthRevolutions*2)
	{
		// increment the counters to change the pair
		indexer_EarthRev++;

		// if the indexer hits 2, reset it to 0 and increment the nEarthOrbits
		if (indexer_EarthRev >= 2)
		{
			indexer_EarthRev = 0;
			nEarthRevolutions++;
		}

		fRunTime_Temp = 0;
	}


	// map the value of the time relative to earth's orbit
	float fPercentEarthRev = MapValue(
		static_cast<float>(fRunTime_Temp),	// value to change
		0.0f,							// original min
		static_cast<float>(fEarthHalfRevTime),	// original max
		0.0f,							// new min
		1.0f);							// new max


										// calculate earth's ORBIT position depending on quaternion pair
	if (indexer_EarthRev == 0)
	{
		glm::quat earthQuatR = glm::mix(earthQuat_Start, earthQuat_Mid, fPercentEarthRev);
		earthRev = glm::mat4_cast(earthQuatR);
	}
	else if (indexer_EarthRev == 1)
	{
		glm::quat earthQuatR = glm::mix(earthQuat_Mid, earthQuat_End, fPercentEarthRev);
		earthRev = glm::mat4_cast(earthQuatR);
	}
	else
	{
		std::cout << "Error in indexer" << std::endl;
	}
#pragma endregion
#pragma region Moon movement
	// ------------------------------------------------------------------------------ ORBIT
	// check if we switch quaternions pairs for ORBIT
	if (fRunTime_Temp_M > fMoonHalfOrbTime * nMoonOrbits)
	{
		// increment the counters to change the pair
		indexer_MoonOrbit++;

		// if the indexer hits 2, reset it to 0 and increment the nEarthOrbits
		if (indexer_MoonOrbit >= 2)
		{
			indexer_MoonOrbit = 0;
			nMoonOrbits++;
		}

		fRunTime_Temp_M = 0;
	}


	// map the value of the time relative to earth's orbit
	float fPercentMoonOrbit = MapValue(
		static_cast<float>(fRunTime_Temp_M),	// value to change
		0.0f,							// original min
		static_cast<float>(fMoonHalfOrbTime),	// original max
		0.0f,							// new min
		1.0f);							// new max


										// calculate earth's ORBIT position depending on quaternion pair
	if (indexer_MoonOrbit == 0)
	{
		glm::quat moonQuat = glm::mix(moonQuat_Start, moonQuat_Mid, fPercentMoonOrbit);
		moonOrbit = glm::mat4_cast(moonQuat);
	}
	else if (indexer_MoonOrbit == 1)
	{
		glm::quat moonQuat = glm::mix(moonQuat_Mid, moonQuat_End, fPercentMoonOrbit);
		moonOrbit = glm::mat4_cast(moonQuat);
	}
	else
	{
		std::cout << "Error in indexer" << std::endl;
	}


#pragma endregion
	//TEMPLATE  rotation * translation
	m4Sun = sunScale;

	// earth's position
	earthspace = earthOrbit * distanceEarth * earthScale;
	m4Earth = earthspace * earthRev;

	// moon's position
	moonspace = moonOrbit * distanceMoon * moonScale; // *glm::mat4_cast(myQuat); //(glm::mix(quat1, quat2, 0.5f));
	m4Moon = earthspace * moonspace;

#pragma endregion

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
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

	//

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}