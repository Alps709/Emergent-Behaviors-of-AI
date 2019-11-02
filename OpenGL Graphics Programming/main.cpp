#include "Debug.h" //Includes glew, so include this first
#include <freeglut.h>
#include <iostream>
#include <fmod.hpp>
#include "vld.h"

#include "TextLabel.h"

#include "Utils.h"
#include "GameManager.h"
#include "Input.h"


GameManager* myGameManager = nullptr;

void Render();
void Update();

void ProcessInput();

void SetGlobalGLSettings();

int main(int argc, char ** argv)
{
	//Setup and create at glut controlled window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize((int)Utils::SCREEN_WIDTH, (int)Utils::SCREEN_HEIGHT);
	glutCreateWindow("The Snek Game - By Anton Le Prevost-Smith");

	//Print openGL version
	std::cout << "Currently using OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	//Set up all gl function callbacks based on pc hardware
	if (glewInit() != GLEW_OK)
	{
		//If glew setup failed then application will not run graphics correctly
		std::cout << "Glew Initialization Failed. Aborting Application." << std::endl;
		system("pause");
		return 0;
	}

	SetGlobalGLSettings();

	myGameManager = new GameManager();
	
	//KeyBoard input function callbacks
	glutKeyboardFunc(Input::KeyBoardDown);
	glutKeyboardUpFunc(Input::KeyBoardUp);
	glutSpecialFunc(Input::SpecialKeyBoardDown);
	glutSpecialUpFunc(Input::SpecialKeyBoardUp);

	//Mouse input function callbacks
	glutMouseFunc(Input::MouseClick);
	glutMotionFunc(Input::MouseMove);
	glutPassiveMotionFunc(Input::MousePassiveMove);

	//Screen resize callback
	glutReshapeFunc(Input::WindowResize);

	//Update and render function callbacks
	glutDisplayFunc(Render);
	glutIdleFunc(Update);

	glutMainLoop();

	delete myGameManager;
	return 0;
}

void Render()
{
	myGameManager->Render();
}

void Update()
{
	ProcessInput();
	myGameManager->Update(Input::g_mousePosX, Input::g_mousePosY);
}

//Most input functionality not needed for the game
void ProcessInput()
{
	//Keyboard input

	////Disable camera changing for now
	//if (Input::KeyState['p'] == Input::INPUT_DOWN_FIRST ||
	//	Input::KeyState['P'] == Input::INPUT_DOWN_FIRST)
	//{
	//	//Flip the camera mode
	//	if(Utils::CAMERA_ORTHOGRAPHIC_MODE)
	//	{
	//		//Turn off cursor
	//		glutSetCursor(GLUT_CURSOR_NONE);

	//		//Set the projection to perspective
	//		m_camera->SetOrthographicProjection(false);
	//		Utils::CAMERA_ORTHOGRAPHIC_MODE = false;
	//	}
	//	else
	//	{
	//		//Turn on cursor
	//		glutSetCursor(GLUT_CURSOR_BOTTOM_SIDE);
	//		//Set the projection to orthographic
	//		m_camera->SetOrthographicProjection(true);
	//		Utils::CAMERA_ORTHOGRAPHIC_MODE = true;
	//	}

	//	//The key is has now been processed for a frame, so set it to the appropriate state
	//	Input::KeyState['p'] = Input::INPUT_DOWN;
	//	Input::KeyState['P'] = Input::INPUT_DOWN;
	//}
	//
	////Move the camera forward with the w button
	//if (Input::KeyState['w'] == Input::INPUT_DOWN || Input::KeyState['w'] == Input::INPUT_DOWN_FIRST ||
	//	Input::KeyState['W'] == Input::INPUT_DOWN || Input::KeyState['W'] == Input::INPUT_DOWN_FIRST)
	//{
	//	//move camera forward
	//	m_camera->SetPosition(m_camera->GetPosition() + m_camera->GetLookDirection() * m_camera->GetCamSpeed());

	//	//Print camera m_position for debugging
	//	//std::cout << "Camera pos: x: " << m_camera->GetPosition().x << " y: " << m_camera->GetPosition().y << " z: " << m_camera->GetPosition().z  << std::endl;

	//	//The key is has now been processed for a frame, so set it to the appropriate state
	//	Input::KeyState['w'] = Input::INPUT_DOWN;
	//	Input::KeyState['W'] = Input::INPUT_DOWN;
	//}
	////Move the camera to the left with the a button
	//if (Input::KeyState['a'] == Input::INPUT_DOWN || Input::KeyState['a'] == Input::INPUT_DOWN_FIRST ||
	//	Input::KeyState['A'] == Input::INPUT_DOWN || Input::KeyState['A'] == Input::INPUT_DOWN_FIRST)
	//{
	//	//move camera left
	//	m_camera->SetPosition(m_camera->GetPosition() - m_camera->GetCamRight() * m_camera->GetCamSpeed());

	//	//The key is has now been processed for a frame, so set it to the appropriate state
	//	Input::KeyState['a'] = Input::INPUT_DOWN;
	//	Input::KeyState['A'] = Input::INPUT_DOWN;
	//}
	////Move the camera backwards with the s button
	//if (Input::KeyState['s'] == Input::INPUT_DOWN || Input::KeyState['s'] == Input::INPUT_DOWN_FIRST ||
	//	Input::KeyState['S'] == Input::INPUT_DOWN || Input::KeyState['S'] == Input::INPUT_DOWN_FIRST)
	//{
	//	//move camera backward
	//	m_camera->SetPosition(m_camera->GetPosition() - m_camera->GetLookDirection() * m_camera->GetCamSpeed());

	//	//The key is has now been processed for a frame, so set it to the appropriate state
	//	Input::KeyState['s'] = Input::INPUT_DOWN;
	//	Input::KeyState['S'] = Input::INPUT_DOWN;
	//}
	////Move the camera to the right witht the d button
	//if (Input::KeyState['d'] == Input::INPUT_DOWN || Input::KeyState['d'] == Input::INPUT_DOWN_FIRST ||
	//	Input::KeyState['D'] == Input::INPUT_DOWN || Input::KeyState['D'] == Input::INPUT_DOWN_FIRST)
	//{
	//	//move camera right
	//	m_camera->SetPosition(m_camera->GetPosition() + m_camera->GetCamRight() * m_camera->GetCamSpeed());

	//	//The key is has now been processed for a frame, so set it to the appropriate state
	//	Input::KeyState['d'] = Input::INPUT_DOWN;
	//	Input::KeyState['D'] = Input::INPUT_DOWN;
	//}
	//
	if (Input::KeyState['c'] == Input::INPUT_DOWN_FIRST ||
		Input::KeyState['C'] == Input::INPUT_DOWN_FIRST)
	{
		myGameManager->GetContainment() = !myGameManager->GetContainment();
		myGameManager->m_containmentStateText->SetText((myGameManager->GetContainment()) ? "Containment: On" : "Containment: Off");

		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState['c'] = Input::INPUT_DOWN;
		Input::KeyState['C'] = Input::INPUT_DOWN;
	}

	//Enter key is pressed
	if (Input::KeyState[13] == Input::INPUT_DOWN || Input::KeyState[13] == Input::INPUT_DOWN_FIRST)
	{
		//Start game from menu by pressing the enter key
		if (GameManager::m_gameState == GAME_MENU)
		{
			//Set game state to play
			GameManager::m_gameState = GAME_PLAY;
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[32] = Input::INPUT_DOWN;
	}

	//Enter key is pressed
	if (Input::KeyState[49] == Input::INPUT_DOWN_FIRST)
	{
		//Start game from menu by pressing the enter key
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to play
			GameManager::m_gameplayState = PLAY_SEEK;
			myGameManager->m_boidStateText->SetText("Behaviour: Seek");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[49] = Input::INPUT_DOWN;
	}

	//Enter key is pressed
	if (Input::KeyState[50] == Input::INPUT_DOWN_FIRST)
	{
		//Start game from menu by pressing the enter key
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to play
			GameManager::m_gameplayState = PLAY_ARRIVE;
			myGameManager->m_boidStateText->SetText("Behaviour: Arrive");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[50] = Input::INPUT_DOWN;
	}

	//Enter key is pressed
	if (Input::KeyState[51] == Input::INPUT_DOWN_FIRST)
	{
		//Start game from menu by pressing the enter key
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to play
			GameManager::m_gameplayState = PLAY_FLOCK;
			myGameManager->m_boidStateText->SetText("Behaviour: Flocking");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[51] = Input::INPUT_DOWN;
	}

	//Enter key is pressed
	if (Input::KeyState[52] == Input::INPUT_DOWN_FIRST)
	{
		//Start game from menu by pressing the enter key
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to play
			GameManager::m_gameplayState = PLAY_FOLLOWPATH;
			myGameManager->m_boidStateText->SetText("Behaviour: Path Following");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[52] = Input::INPUT_DOWN;
	}

	////Move the camera up with the space button
	//if (Input::KeyState[32] == Input::INPUT_DOWN || Input::KeyState[32] == Input::INPUT_DOWN_FIRST)
	//{
	//	glm::vec3 tempVec = m_camera->GetPosition();
	//	tempVec.y += 2.5f;
	//	m_camera->SetPosition(glm::vec3(tempVec));

	//	//The key is has now been processed for a frame, so set it to the appropriate state
	//	Input::KeyState[32] = Input::INPUT_DOWN;
	//}
	////Move the camera down with the left shift button
	//if (Input::SpecialKeyState[GLUT_KEY_SHIFT_L] == Input::INPUT_DOWN || Input::SpecialKeyState[GLUT_KEY_SHIFT_L] == Input::INPUT_DOWN_FIRST)
	//{
	//	glm::vec3 tempVec = m_camera->GetPosition();
	//	tempVec.y -= 2.5f;
	//	m_camera->SetPosition(glm::vec3(tempVec));

	//	//The key is has now been processed for a frame, so set it to the appropriate state
	//	Input::SpecialKeyState[GLUT_KEY_SHIFT_L] = Input::INPUT_DOWN;
	//}

	//Mouse Input
	if (Input::MouseState[0] == Input::INPUT_DOWN_FIRST/* || Input::MouseState[0] == Input::INPUT_DOWN*/)
	{	//Left click
		if (GameManager::m_gameState == GAME_PLAY)
		{
			if (myGameManager->GetBoids().size() < 50)
			{
				myGameManager->GetBoids().push_back(myGameManager->MakeBoid());
			}
		}
		Input::MouseState[0] = Input::INPUT_DOWN;
	}

	if (Input::MouseState[2] == Input::INPUT_DOWN_FIRST/* || Input::MouseState[2] == Input::INPUT_DOWN*/)
	{ //Right click
		if (GameManager::m_gameState == GAME_PLAY && myGameManager->GetBoids().size() > 1)
		{
			myGameManager->GetBoids().pop_back();
		}
		Input::MouseState[2] = Input::INPUT_DOWN;
	}
}

void SetGlobalGLSettings()
{
	///Set global settings

	////Disable the cursor for the first person camera
	//glutWarpPointer(Utils::HSCREEN_WIDTH, (int)Utils::HSCREEN_HEIGHT);
	//Input::g_mousePosX = (int)Utils::HSCREEN_WIDTH;
	//Input::g_mousePosY = (int)Utils::HSCREEN_HEIGHT;
	//glutSetCursor(GLUT_CURSOR_NONE);

	//Cull the back faces of objects (only do so for 3D)
	//glEnable(GL_CULL_FACE);
	//GLCall(glCullFace(GL_BACK));

	//Normal are calculated counter clockwise
	GLCall(glFrontFace(GL_CCW));

	//Doesn't draw stuff that is not facing the screen
	//GLCall(glEnable(GL_CULL_FACE));

	//Setup alpha blending
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	//Allows the m_program to continue executing after the glutMainLoop()
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	// Enable depth test (only do so for 3D)
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_NOTEQUAL);

	//Sets the clear colour when calling glClear()
	GLCall(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
}