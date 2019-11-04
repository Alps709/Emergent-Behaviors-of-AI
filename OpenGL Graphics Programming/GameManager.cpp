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
	//AudioInitialise();

	//Create defaut shader
	m_defaultShader = new Shader();

	m_boidMesh = new Mesh(Objects::verticesBoid, Objects::indicesBoid);

	//Set background mesh and texture
	m_backgroundMesh = new Mesh(Objects::verticesBackground, Objects::indicesBackground);
	m_backgroundTexture = new Texture("Resources/Images/Grass.png", 0);

	//Create 1 background object
	m_backgroundObject = Object(m_backgroundMesh, m_defaultShader, glm::vec2(0.0f, 0.0f));
	m_backgroundObject.SetTexture0(m_backgroundTexture);

	//Create the text objects
	m_boidStateText = new TextLabel("Behaviour: Seek", "Resources/Fonts/arial.ttf", glm::vec2(-Utils::HSCREEN_WIDTH + 20.0f, Utils::HSCREEN_HEIGHT - 40.0f));
	m_containmentStateText = new TextLabel("Containment: Off", "Resources/Fonts/arial.ttf", glm::vec2(-Utils::HSCREEN_WIDTH + 20.0f, Utils::HSCREEN_HEIGHT - 80.0f));
	m_menuTitleText = new TextLabel("The Boid Game!", "Resources/Fonts/kirbyss.ttf", glm::vec2(-625, 200), glm::vec3(0.0f, 1.0f, 1.0f), 2.8f);
	m_menuInstructText = new TextLabel("Press enter to play", "Resources/Fonts/kirbyss.ttf", glm::vec2(-600, -200), glm::vec3(0.0f, 1.0f, 1.0f), 2.0f);

	//Create original boid
	m_boids.push_back(MakeBoid());

	//Create the camera
	//Pass in false to say it is not using an orthographic view initially (it will then use a perspective view projection)
	m_camera = new Camera(true);
}


GameManager::~GameManager()
{
	//Delete all the heap allocated objects and clean up others
	m_yeatSound->release();
	m_audioSystem->release();
	delete m_boidStateText;
	delete m_containmentStateText;
	delete m_menuTitleText;
	delete m_menuInstructText;
	delete m_backgroundMesh;
	delete m_backgroundTexture;
	delete m_boidMesh;
	delete m_defaultShader;
	delete m_camera;
}

//void GameManager::AudioInitialise()
//{
//
//	FMOD_RESULT result;
//	//Initialise the m_audioSystem
//	result = FMOD::System_Create(&m_audioSystem);
//	if (result != FMOD_OK)
//	{
//		std::cout << "Audio system failed to initialise!";
//		return;
//	}
//
//	result = m_audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
//	if (result != FMOD_OK)
//	{
//		std::cout << "Audio system failed to initialise!";
//	}
//	else
//	{
//		result = m_audioSystem->createSound("Resources/Audio/yeat.wav", FMOD_DEFAULT, 0, &m_yeatSound);
//		if (result != FMOD_OK)
//		{
//			std::cout << "Failed to load sound: yeat.wav" << std::endl;
//		}
//
//		result = m_audioSystem->createSound("Resources/Audio/shoop.wav", FMOD_DEFAULT, 0, &m_shoopSound);
//		if (result != FMOD_OK)
//		{
//			std::cout << "Failed to load sound: shoop.wav" << std::endl;
//		}
//
//		//Create background music
//		//result = m_audioSystem->createSound("Resources/Audio/Jet Set Run.mp3", FMOD_LOOP_NORMAL, 0, &m_trackBackground);
//		//if (result != FMOD_OK)
//		//{
//		//	std::cout << "Failed to load sound: Jet Set Run.mp3" << std::endl;
//		//}
//		//else
//		//{
//		//	//Created sound
//		//	//Start playing background music
//		//	const FMOD_RESULT play = m_audioSystem->playSound(m_trackBackground, 0, false, 0);
//		//	if (play != FMOD_OK)
//		//	{
//		//		std::cout << "Failed to play background track: You Say Run.mp3" << std::endl;
//		//	}
//		//}
//	}
//
//}

void GameManager::Update(int _mousePosX, int _mousePosY)
{
	//Update clock
	m_clock.Process();

	if (m_gameState == GAME_PLAY)
	{
		//Render boids
		for (Boid& boid : m_boids)
		{
			boid.Process(m_gameplayState, m_boids, m_containment, _mousePosX, _mousePosY, m_clock.GetDeltaTick());
		}
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
		//Render boids
		for (Boid& boid : m_boids)
		{
			boid.Render(*m_camera);
		}

		m_boidStateText->Render();
		m_containmentStateText->Render();

		if (m_gameplayState == PLAY_FOLLOWPATH)
		{
			std::vector<glm::vec2> path;
			path.push_back(glm::vec2((-Utils::HSCREEN_WIDTH)/ Utils::HSCREEN_WIDTH, 0));
			path.push_back(glm::vec2((-Utils::HSCREEN_WIDTH/2)/ Utils::HSCREEN_WIDTH, (Utils::HSCREEN_HEIGHT/ 2)/ Utils::HSCREEN_HEIGHT));
			path.push_back(glm::vec2((Utils::HSCREEN_WIDTH/2)/ Utils::HSCREEN_WIDTH, (-Utils::HSCREEN_HEIGHT/ 2)/ Utils::HSCREEN_HEIGHT));
			path.push_back(glm::vec2((Utils::HSCREEN_WIDTH)/ Utils::HSCREEN_WIDTH, 0));

			//Draw a line from each point of the path to the next
			for (int i = 0; i < path.size() - 1; ++i)
			{
				glm::vec2 a = path[i];
				glm::vec2 b = path[i + 1];

				glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex3f(a.x, a.y, 0.0f);
				glVertex3f(b.x, b.y, 0.0f);
				glEnd();
			}
		}
	}

	glutSwapBuffers();
	u_frameNum++;
}

std::vector<Boid>& GameManager::GetBoids()
{
	return m_boids;
}

Boid GameManager::MakeBoid()
{
	++m_boidIDCounter;
	return Boid(m_boidIDCounter, m_boidMesh, m_defaultShader, glm::vec2(0.0f, 0.0f));
}


