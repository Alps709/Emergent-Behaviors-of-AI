#include "GameManager.h"
#include "Obj.h"
#include "Renderer.h"

#include <fmod.h>
#include "Input.h"


GameManager::GameManager()
{
	//Start clock
	m_clock.Initialise();

	//Start audio system
	AudioInitialise();

	//Create defaut shader
	m_defaultShader = new Shader();

	//Set pill mesh and texture
	m_pillMesh = new Mesh(Objects::verticesPill, Objects::indicesPill);
	m_pillTexture = new Texture("Resources/Images/Circle.png", 0);

	//Set background mesh and texture
	m_backgroundMesh = new Mesh(Objects::verticesBackground, Objects::indicesBackground);
	m_backgroundTexture = new Texture("Resources/Images/Grass.png", 0);

	//Create 1 background object
	m_backgroundObject = Object(m_backgroundMesh, m_defaultShader, glm::vec3(0.0f, 0.0f, 0.0f));
	m_backgroundObject.SetTexture0(m_backgroundTexture);

	//Create random generator for pill positions
	std::random_device dev;
	std::mt19937 rng(dev());
	const std::uniform_real_distribution<double> randXPos(-Utils::HSCREEN_WIDTH + 50, Utils::HSCREEN_WIDTH - 50);
	const std::uniform_real_distribution<double> randYPos(-Utils::HSCREEN_HEIGHT + 50, Utils::HSCREEN_HEIGHT - 50);

	////Create 10 pill models
	//for (int i = 0; i < 10; i++)
	//{
	//	Pill myTempObject = Pill(m_pillMesh, m_defaultShader, glm::vec3(randXPos(rng), randYPos(rng), 0.0f));
	//	myTempObject.SetTexture0(m_pillTexture);
	//	m_pillObjects.push_back(myTempObject);
	//}

	//Create the text objects
	m_scoreText = new TextLabel("Score: 0", "Resources/Fonts/arial.ttf", glm::vec2(-Utils::HSCREEN_WIDTH + 20.0f, Utils::HSCREEN_HEIGHT - 40.0f));
	m_menuTitleText = new TextLabel("The Snek Game!", "Resources/Fonts/kirbyss.ttf", glm::vec2(-625, 200), glm::vec3(0.0f, 1.0f, 1.0f), 2.8f);
	m_menuInstructText = new TextLabel("Press enter to play", "Resources/Fonts/kirbyss.ttf", glm::vec2(-600, -200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);
	m_overText = new TextLabel("Game Over!", "Resources/Fonts/kirbyss.ttf", glm::vec2(-625, 200), glm::vec3(1.0f, 0.0f, 0.0f), 4);
	m_overScoreText = new TextLabel("Your final score is: ", "Resources/Fonts/arial.ttf", glm::vec2(-600, -200), glm::vec3(1.0f, 0.0f, 0.0f), 1.5f);

	//Create snek
	m_snek = new Snek(&m_pillObjects, m_scoreText, m_audioSystem, m_yeatSound, m_shoopSound);

	//Create the camera
	//Pass in false to say it is not using an orthographic view initially (it will then use a perspective view projection)
	m_camera = new Camera(true);
}


GameManager::~GameManager()
{
	//Delete all the heap allocated objects and clean up others
	m_yeatSound->release();
	m_audioSystem->release();
	delete m_snek;
	delete m_scoreText;
	delete m_overText;
	delete m_overScoreText;
	delete m_menuTitleText;
	delete m_menuInstructText;
	delete m_timeText;
	delete m_pillMesh;
	delete m_pillTexture;
	delete m_backgroundMesh;
	delete m_backgroundTexture;
	delete m_defaultShader;
	delete m_camera;
}

void GameManager::AudioInitialise()
{

	FMOD_RESULT result;
	//Initialise the m_audioSystem
	result = FMOD::System_Create(&m_audioSystem);
	if (result != FMOD_OK)
	{
		std::cout << "Audio system failed to initialise!";
		return;
	}

	result = m_audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
	{
		std::cout << "Audio system failed to initialise!";
	}
	else
	{
		result = m_audioSystem->createSound("Resources/Audio/yeat.wav", FMOD_DEFAULT, 0, &m_yeatSound);
		if (result != FMOD_OK)
		{
			std::cout << "Failed to load sound: yeat.wav" << std::endl;
		}

		result = m_audioSystem->createSound("Resources/Audio/shoop.wav", FMOD_DEFAULT, 0, &m_shoopSound);
		if (result != FMOD_OK)
		{
			std::cout << "Failed to load sound: shoop.wav" << std::endl;
		}

		//Create background music
		result = m_audioSystem->createSound("Resources/Audio/Jet Set Run.mp3", FMOD_LOOP_NORMAL, 0, &m_trackBackground);
		if (result != FMOD_OK)
		{
			std::cout << "Failed to load sound: Jet Set Run.mp3" << std::endl;
		}
		else
		{
			//Created sound
			//Start playing background music
			const FMOD_RESULT play = m_audioSystem->playSound(m_trackBackground, 0, false, 0);
			if (play != FMOD_OK)
			{
				std::cout << "Failed to play background track: You Say Run.mp3" << std::endl;
			}
		}
	}

}

//Do substance abuse here
void GameManager::ProcessPills()
{
	//Find all pills that have existed for 15 seconds and erase them from the vector
	for (auto pillIt = m_pillObjects.begin(); pillIt != m_pillObjects.end();)
	{
		if (pillIt->GetTimeAlive() > 5.0)
		{
			//Pill has been alive for more than 5 seconds
			pillIt = m_pillObjects.erase(pillIt);

			//Play sound
			const FMOD_RESULT play = m_audioSystem->playSound(m_shoopSound, 0, false, 0);
			if (play != FMOD_OK)
			{
				std::cout << "Failed to play sound: Thump.wav" << std::endl;
			}
		}
		else
		{
			//Only move the iterator forward if nothing was erased
			//Because it is automatically moved forward after erase() is called
			++pillIt;
		}
	}

	m_pillSpawnTimer += m_clock.GetDeltaTick();

	//Spawn a pill every 333 milliseconds
	if (m_pillSpawnTimer > 333.333333)
	{
		//Reset timer
		m_pillSpawnTimer = 0.0;

		//Create random generator for pill positions
		std::random_device dev;
		std::mt19937 rng(dev());
		const std::uniform_real_distribution<double> randXPos(-Utils::HSCREEN_WIDTH + 50, Utils::HSCREEN_WIDTH - 50);
		const std::uniform_real_distribution<double> randYPos(-Utils::HSCREEN_HEIGHT + 50, Utils::HSCREEN_HEIGHT - 50);

		//Create 1 pill model
		Pill myTempObject = Pill(m_pillMesh, m_defaultShader, glm::vec3(randXPos(rng), randYPos(rng), 0.0f));
		myTempObject.SetTexture0(m_pillTexture);
		m_pillObjects.push_back(myTempObject);
	}
}

void GameManager::Update(int _mousePosX, int _mousePosY)
{
	//Update clock
	m_clock.Process();

	if (m_gameState == GAME_PLAY)
	{
		//Process snek movement
		m_snek->Process(m_gameState, m_gameScore, _mousePosX, _mousePosY, m_clock.GetDeltaTick());

		//Process pill spawning and despawning
		ProcessPills();
	}

	//Update sounds
	m_audioSystem->update();

	//Tell glut to call the render function again
	glutPostRedisplay();
}

void GameManager::Render()
{
	//Clear the screen before every frame
	Renderer::Clear();

	//Draw background
	m_backgroundObject.Render(*m_camera);

	if (m_gameState == GAME_MENU)
	{
		m_menuTitleText->Render();
		m_menuInstructText->Render();
	}
	else if (m_gameState == GAME_PLAY)
	{
		//Render pills
		for (auto it = m_pillObjects.begin(); it != m_pillObjects.end(); ++it)
		{
			it->Render(*m_camera);
		}

		//Render snek
		m_snek->Render(*m_camera);

		m_scoreText->Render();
	}
	else if (m_gameState == GAME_OVER)
	{
		std::string tempText = "Your final score is: ";
		tempText = tempText + std::to_string(m_gameScore);

		m_overScoreText->SetText(tempText);
		m_overText->Render();
		m_overScoreText->Render();
	}

	glutSwapBuffers();
	u_frameNum++;
}


