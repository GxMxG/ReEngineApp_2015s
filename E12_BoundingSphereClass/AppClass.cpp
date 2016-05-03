#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("MyBoundingSphereClass example"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Initialize positions
	m_v3O1 = vector3(-2.5f, 0.0f, 0.0f);
	m_v3O2 = vector3(2.5f, 0.0f, 0.0f);

	//Load Models
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");
<<<<<<< HEAD
<<<<<<< HEAD

	m_pBox1 = new MyBoundingCubeClass(m_pMeshMngr->GetVertexList("Steve"));
	m_pBox2 = new MyBoundingCubeClass(m_pMeshMngr->GetVertexList("Creeper"));
=======
>>>>>>> 8f580f7f9fede253430f22e409d3eac4ead431c5
=======
=======

	m_pBox1 = new MyBoundingCubeClass(m_pMeshMngr->GetVertexList("Steve"));
	m_pBox2 = new MyBoundingCubeClass(m_pMeshMngr->GetVertexList("Creeper"));
>>>>>>> dcacc1e804d49ec9acff5eee2668064d6adc3e8d
>>>>>>> bbf1112fc95df3dbcdefd2839e8b42d9fec553c4
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

	ArcBall();

	//Set the model matrices for both objects and Bounding Spheres
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O1) * ToMatrix4(m_qArcBall), "Steve");
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O2), "Creeper");

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> bbf1112fc95df3dbcdefd2839e8b42d9fec553c4
	m_pBox1->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
	m_pBox2->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Creeper"));

	bool isColliding = m_pBox1->IsColliding(m_pBox2);

	if (isColliding)
	{
		m_pMeshMngr->AddSphereToQueue(
			glm::translate(vector3(m_pBox1->GetCenterG())) *
			glm::scale(vector3(m_pBox1->GetRadius()) * 2.0f), RERED, WIRE);
		m_pMeshMngr->AddSphereToQueue(glm::translate(vector3(m_pBox2->GetCenterG()))  *
			glm::scale(vector3(m_pBox2->GetRadius()) * 2.0f), RERED, WIRE);
	}
	else
	{
		m_pMeshMngr->AddSphereToQueue(
			glm::translate(vector3(m_pBox1->GetCenterG())) *
			glm::scale(vector3(m_pBox1->GetRadius()) * 2.0f), REGREEN, WIRE);
		m_pMeshMngr->AddSphereToQueue(glm::translate(vector3(m_pBox2->GetCenterG()))  *
			glm::scale(vector3(m_pBox2->GetRadius()) * 2.0f), REGREEN, WIRE);
	}
	
<<<<<<< HEAD
=======
>>>>>>> 8f580f7f9fede253430f22e409d3eac4ead431c5
=======
>>>>>>> dcacc1e804d49ec9acff5eee2668064d6adc3e8d
>>>>>>> bbf1112fc95df3dbcdefd2839e8b42d9fec553c4
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	m_m4Steve =
		m_pMeshMngr->GetModelMatrix("Steve") * 
		glm::translate(m_v3Center1);
	//m_pSphere1->Render(m4Projection, m4View, m4Model);
	m_pMeshMngr->AddSphereToQueue(m_m4Steve * glm::scale(vector3(m_fRadius1 * 2.0f)), RERED, WIRE);

	m_m4Creeper =
		m_pMeshMngr->GetModelMatrix("Creeper") * 
		glm::translate(m_v3Center2);
	//m_pSphere2->Render(m4Projection, m4View, m4Model);
	m_pMeshMngr->AddSphereToQueue(m_m4Creeper * glm::scale(vector3(m_fRadius2 * 2.0f)), RERED, WIRE);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> bbf1112fc95df3dbcdefd2839e8b42d9fec553c4

	vector3 v3Center_1_Golbal = vector3(m_m4Steve * vector4(m_v3Center1, 1.0f));
	vector3 v3Center_2_Golbal = vector3(m_m4Creeper * vector4(m_v3Center2, 1.0f));

	float fDistanceCenters = glm::distance(v3Center_1_Golbal, v3Center_2_Golbal);

	bool bAreColliding = false;
	if (fDistanceCenters < m_fRadius1 + m_fRadius2) { bAreColliding = true;	}

	//Collision check goes here
	m_pMeshMngr->Print("x:" + std::to_string(v3Center_1_Golbal.x ) + " ", RERED);
	m_pMeshMngr->Print("y:" + std::to_string(v3Center_1_Golbal.y) + " ", RERED);
	m_pMeshMngr->Print("z:" + std::to_string(v3Center_1_Golbal.z) + " ", RERED);

	m_m4Steve = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(m_v3Center1);

	if(bAreColliding)
		m_pMeshMngr->AddCubeToQueue(m_m4Steve * glm::scale(vector3(m_fRadius1 * 2.0f)), RERED, WIRE);
	else
		m_pMeshMngr->AddCubeToQueue(m_m4Steve * glm::scale(vector3(m_fRadius1 * 2.0f)), REGREEN, WIRE);

	m_m4Creeper = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(m_v3Center2);
	if (bAreColliding)
		m_pMeshMngr->AddCubeToQueue(m_m4Creeper * glm::scale(vector3(m_fRadius2 * 2.0f)), RERED, WIRE);
	else
		m_pMeshMngr->AddCubeToQueue(m_m4Creeper * glm::scale(vector3(m_fRadius2 * 2.0f)), REGREEN, WIRE);

	/*
	m_pMeshMngr->Print("x:" + std::to_string(v3Temp.x ) + " ", RERED);
	m_pMeshMngr->Print("y:" + std::to_string(v3Temp.y) + " ", RERED);
	m_pMeshMngr->Print("z:" + std::to_string(v3Temp.z) + " ", RERED);
	*/

	m_pMeshMngr->PrintLine("");

<<<<<<< HEAD
=======
>>>>>>> 8f580f7f9fede253430f22e409d3eac4ead431c5
=======
>>>>>>> dcacc1e804d49ec9acff5eee2668064d6adc3e8d
>>>>>>> bbf1112fc95df3dbcdefd2839e8b42d9fec553c4
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ResetRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> bbf1112fc95df3dbcdefd2839e8b42d9fec553c4
	if (m_pBox1 != nullptr)
	{
		delete m_pBox1;
		m_pBox1 = nullptr;

	}
	if (m_pBox2 != nullptr)
	{
		delete m_pBox2;
		m_pBox2 = nullptr;

	}
<<<<<<< HEAD
=======
>>>>>>> 8f580f7f9fede253430f22e409d3eac4ead431c5
=======
>>>>>>> dcacc1e804d49ec9acff5eee2668064d6adc3e8d
>>>>>>> bbf1112fc95df3dbcdefd2839e8b42d9fec553c4
	super::Release(); //release the memory of the inherited fields
}