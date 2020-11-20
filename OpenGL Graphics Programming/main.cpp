#include "Debug.h" //Includes glew, so include this first
#include <freeglut.h>
#include <iostream>
#include <fmod.hpp>

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

	//Turn the boid containment on/off
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

	//'1' key is pressed
	if (Input::KeyState[49] == Input::INPUT_DOWN_FIRST)
	{
		//Check if the game is being played
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to have the boids seek towards the mouse
			GameManager::m_gameplayState = PLAY_SEEK;
			myGameManager->m_boidStateText->SetText("Behaviour: Seek");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[49] = Input::INPUT_DOWN;
	}

	//'2' key is pressed
	if (Input::KeyState[50] == Input::INPUT_DOWN_FIRST)
	{
		//Check if the game is being played
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to have the boids arrive at the mouse position
			GameManager::m_gameplayState = PLAY_ARRIVE;
			myGameManager->m_boidStateText->SetText("Behaviour: Arrive");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[50] = Input::INPUT_DOWN;
	}

	//'3' key is pressed
	if (Input::KeyState[51] == Input::INPUT_DOWN_FIRST)
	{
		//Check if the game is being played
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to have the boids flock
			GameManager::m_gameplayState = PLAY_FLOCK;
			myGameManager->m_boidStateText->SetText("Behaviour: Flocking");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[51] = Input::INPUT_DOWN;
	}

	//'4' key is pressed
	if (Input::KeyState[52] == Input::INPUT_DOWN_FIRST)
	{
		//Check if the game is being played
		if (GameManager::m_gameState == GAME_PLAY)
		{
			//Set game state to have the boids follow a path
			GameManager::m_gameplayState = PLAY_FOLLOWPATH;
			myGameManager->m_boidStateText->SetText("Behaviour: Path Following");
		}
		//The key is has now been processed for a frame, so set it to the appropriate state
		Input::KeyState[52] = Input::INPUT_DOWN;
	}


	//Mouse Input

	if (Input::MouseState[0] == Input::INPUT_DOWN_FIRST)
	{	//Left click
		if (GameManager::m_gameState == GAME_PLAY)
		{
			if (myGameManager->GetBoids().size() < 50)
			{
				//Add a boid if the player left clicks
				myGameManager->GetBoids().push_back(myGameManager->MakeBoid());
			}
		}
		Input::MouseState[0] = Input::INPUT_DOWN;
	}

	if (Input::MouseState[2] == Input::INPUT_DOWN_FIRST)
	{ 
		if (GameManager::m_gameState == GAME_PLAY && myGameManager->GetBoids().size() > 1)
		{
			//Remove a boid if the player right clicks
			myGameManager->GetBoids().pop_back();
		}
		Input::MouseState[2] = Input::INPUT_DOWN;
	}
}

void SetGlobalGLSettings()
{
	///Set global settings

	//Normal are calculated counter clockwise
	GLCall(glFrontFace(GL_CCW));

	//Setup alpha blending
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	//Allows the program to continue executing after the glutMainLoop()
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	//Sets the clear colour when calling glClear()
	GLCall(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
}