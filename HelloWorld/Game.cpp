#include "Game.h"
#include "BackEnd.h"
#include "HelloWorld.h"
#include "NewScene.h"
#include "AssignmentScene.h"
#include "BioshockScene.h"
#include "Menu.h"
#include <iostream>
#include "GPCSound.h"

#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#include <random>

using namespace std;

bool weapon = false;
int menuchoice = 0;
int scenenum = 0;

int jumpcount = 0;

int Enemyxpos;
int Enemyypos;

int HealthBarnum = 0;

SDL_MouseWheelEvent evntx;

Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//Set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	//Scene names and clear colors
	m_name = "Fury of the Death";
	m_clearColor = vec4(0.15f, 0.33f, 0.58f, 1.f);

	//Initializes the backend
	BackEnd::InitBackEnd(m_name);

	//Grabs the initialized window
	m_window = BackEnd::GetWindow();

	//Creates a new HelloWorld scene
	m_scenes.push_back(new Menu("Menu Screen"));
	m_scenes.push_back(new BioshockScene("Bioshock Scene"));
	m_scenes.push_back(new HelloWorld("HorizontalScrolling"));
	m_scenes.push_back(new NewScene("New Scene"));
	m_scenes.push_back(new HelloWorld("Second Loaded Scene"));
	m_scenes.push_back(new AssignmentScene("Assignment Scene"));
	

	//Sets active scene reference to our HelloWorld scene
	m_activeScene = m_scenes[scenenum];


	//Initializes the scene
	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));

	//*m_activeScene = File::LoadJSON("Hello World.json");

	//Sets m_register to point to the register in the active scene
	m_register = m_activeScene->GetScene();

	//InitVariables();

	PhysicsSystem::Init();

	std::cout << "Controller 1: " << (XInputManager::ControllerConnected(0) ? "Connected" : "Not Connected") << std::endl;
}


bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with clearColor
		m_window->Clear(m_clearColor);
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();
		
		//Flips the windows
		m_window->Flip();
		
		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	if (scenenum == 1)
	{
		//Update Physics System
		PhysicsSystem::Update(m_register, m_activeScene->GetPhysicsWorld());
	}
	

	//Updates Scene
	m_activeScene = m_scenes[scenenum];

}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register, m_activeScene);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
	{
		MouseClick(BackEnd::GetClickEvent());
		AttackMouseClick(evntx);
	}
		

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());

	


	if (scenenum == 0)
	{
		CreateMenuButton(); // Creates the menu
		//Hearts();
	}

}

void Game::AcceptInput()
{
	XInputManager::Update();

	//Just calls all the other input functions
	GamepadInput();

	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::GamepadInput()
{
	XInputController* tempCon;
	//Gamepad button stroked (pressed)
	for (int i = 0; i < 3; i++)
	{
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.1f);

			//If controller is connected, we run the different input types
			GamepadStroke(tempCon);
			GamepadUp(tempCon);
			GamepadDown(tempCon);
			GamepadStick(tempCon);
			GamepadTrigger(tempCon);
		}
	}
}


void Game::KeyboardHold()
{
	if (scenenum == 1)
	{
		BioshockScene* scene = (BioshockScene*)m_activeScene;

		auto playerEntity = scene->GetPlayer();
		auto& playerPhys = m_register->get<PhysicsBody>(playerEntity);
		auto& playerAnim = m_register->get<AnimationController>(playerEntity);
		auto& playerPos = m_register->get<Transform>(EntityIdentifier::MainPlayer());


		vec2 totalForce = vec2(0.f, 0.f); //(0.f, -1.f) <--Imitates gravity


		float speed = 50.f;


		//Keyboard button held
		if (Input::GetKey(Key::W))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(13);

			playerPhys.ApplyForce(vec3(0.f, 100000.f, 0.f));


			//auto& trans = m_register->get<Transform>(EntityIdentifier::MainPlayer());
			//trans.SetPositionY(trans.GetPositionY() + (speed * Timer::deltaTime));
		}
		if (Input::GetKey(Key::A))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(11);


			playerPhys.ApplyForce(vec3(-100000.f, 0.f, 0.f));


		}
		if (Input::GetKey(Key::S))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(8);

			playerPhys.ApplyForce(vec3(0.f, -100000.f, 0.f));



		}
		if (Input::GetKey(Key::D))
		{
			//Sets Active Animation
			playerAnim.SetActiveAnim(12);

			playerPhys.ApplyForce(vec3(100000.f, 0.f, 0.f));

		}

		if (jumpcount < 100)
		{
			vec3 position = m_register->get<Transform>(EntityIdentifier::MainPlayer()).GetPosition();
			if (Input::GetKey(Key::Space) && Input::GetKey(Key::D)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce(vec3(1000000.f, 0.f, 0.f));
				playerAnim.SetActiveAnim(3);
				jumpcount++;
			}
			if (Input::GetKey(Key::Space) && Input::GetKey(Key::A)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce(vec3(-1000000.f, 0.f, 0.f));
				playerAnim.SetActiveAnim(1);
				jumpcount++;
			}

			if (Input::GetKey(Key::Space) && Input::GetKey(Key::W)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce(vec3(0.f, 1000000.f, 0.f));
				playerAnim.SetActiveAnim(2);
				jumpcount++;
			}

			if (Input::GetKey(Key::Space) && Input::GetKey(Key::S)) {
				//m_register->get<Transform>(EntityIdentifier::MainPlayer()).SetPositionX(++position.x + (speed * Timer::deltaTime));
				playerPhys.ApplyForce(vec3(0.f, -1000000.f, 0.f));
				playerAnim.SetActiveAnim(4);
				jumpcount++;
			}
			
		}




		//Melee Attacks
		/*if (Input::GetKey(Key::RightArrow))
		{
			playerAnim.SetActiveAnim(4);
		}
		if (Input::GetKey(Key::LeftArrow))
		{
			if (weapon == false)
			{
				playerAnim.SetActiveAnim(1);

			}
		}
		if (Input::GetKey(Key::DownArrow))
		{
			if (weapon == false)
			{
				playerAnim.SetActiveAnim(0);

			}
		}
		if (Input::GetKey(Key::UpArrow))
		{
			if (weapon == false)
			{
				playerAnim.SetActiveAnim(3);

			}
		}
		*/

		

		//Mass
		float m_mass = 0.5f;
	}
	

	
	
}

void Game::KeyboardDown()
{
	

	
}

void Game::KeyboardUp()
{
	if (scenenum == 1)
	{
		if (Input::GetKeyUp(Key::P))
		{
			PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
		}
	}


	if (Input::GetKeyUp(Key::F1))
	{
		if (!UI::m_isInit)
		{
			UI::InitImGUI();
		}
		m_guiActive = !m_guiActive;
	}

}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	//printf("Mouse Moved (%f, %f)\n", float(evnt.x), float(evnt.y));





	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}
	if (SDL_GetMouseState(NULL, NULL))
	{
		printf("Mouse Moved at (%f, %f)\n", float(evnt.x), float(evnt.y));
	}

	

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	

	if (scenenum == 1)
	{
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			

			//PlaySound(TEXT("SwordSlash.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_NOSTOP);
			
		}


		BioshockScene* scene = (BioshockScene*)m_activeScene;

		auto playerEntity = scene->GetPlayer();
		auto& playerPhys = m_register->get<PhysicsBody>(playerEntity);
		auto& playerAnim = m_register->get<AnimationController>(playerEntity);
		auto& playerPos = m_register->get<Transform>(EntityIdentifier::MainPlayer());

		// Plays animation when clicking mouse
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::D))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			playerAnim.SetActiveAnim(4);
		}
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::A))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			if (weapon == false)
			{
				playerAnim.SetActiveAnim(1);

			}
		}

		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::S))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			if (weapon == false)
			{
				playerAnim.SetActiveAnim(0);

			}
		}

		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && Input::GetKeyDown(Key::W))
		{
			printf("Left Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));

			if (weapon == false)
			{
				playerAnim.SetActiveAnim(3);

			}
		}




		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			printf("Right Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));
		}

		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		{
			printf("Middle Mouse Button Clicked at (%f, %f)\n", float(evnt.x), float(evnt.y));
		}


		if (m_guiActive)
		{
			ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
			ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
			ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
			ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
		}

		//Resets the enabled flag
		m_click = false;
	}

	}
	

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	printf("Mouse Scroll %f\n", float(evnt.y));

	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}

void Game::AttackMouseClick(SDL_MouseWheelEvent evnt)
{
	BioshockScene* slash = (BioshockScene*)m_activeScene;
	auto SlashEntity = slash->GetSlash();
	vec3 Slashposition = m_register->get<Transform>(SlashEntity).GetPosition();
	auto& animSlash = ECS::GetComponent<AnimationController>(SlashEntity);


	SoundManager::init("./Assets/Sounds/");
	Sound2D _Slash("SwordSlash.wav", "group1");
	evntx = evnt;
	if (scenenum == 1 && SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		//ECS::GetComponent<Transform>(EntityIdentifier::Slash()).SetPosition(vec3(evnt.x, evnt.y, 60.f));  //Sets Location
		_Slash.play();
	}
}

void Game::CreateMenuButton()
{
	Menu* scene = (Menu*)m_activeScene;
	auto menuEntity = scene->Menu1();
	vec3 Menuposition = m_register->get<Transform>(menuEntity).GetPosition();
	auto& animController = ECS::GetComponent<AnimationController>(menuEntity);


	m_activeScene = 0;

	if (Input::GetKeyUp(Key::UpArrow) && m_activeScene == 0) {
		//change imagecount to -1 
		menuchoice--;
		if (menuchoice < 0) {
			menuchoice = 2;
		}
		animController.SetActiveAnim(menuchoice);

	}

	if (Input::GetKeyUp(Key::DownArrow) && m_activeScene == 0) {
		//change imagecount to +1
		menuchoice++;
		if (menuchoice > 2) {
			menuchoice = 0;
		}

		animController.SetActiveAnim(menuchoice);


	}

	if (Input::GetKeyUp(Key::Enter) && menuchoice == 0)
	{
		scenenum = 1;

		m_activeScene = m_scenes[scenenum];

		m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
		m_register = m_activeScene->GetScene();
	}

	if (Input::GetKeyUp(Key::Enter) && menuchoice == 1)
	{
		auto entityControls = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entityControls);
		ECS::AttachComponent<Transform>(entityControls);

		//Sets up components
		std::string Controls = "Controls.png"; //400, 200
		ECS::GetComponent<Sprite>(entityControls).LoadSprite(Controls, 380, 200);
		ECS::GetComponent<Transform>(entityControls).SetPosition(vec3(0.f, 0.f, 20.f));

		//Setup up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entityControls, bitHolder, "Credit Entity");

		if (Menuposition.z <= 0)
		{
			Menuposition.z == 20;
		}
		ECS::GetComponent<Transform>(menuEntity).SetPositionZ(Menuposition.z - 20);




	}


	if (Input::GetKeyUp(Key::Enter) && menuchoice == 2)
	{
		auto entityCred = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entityCred);
		ECS::AttachComponent<Transform>(entityCred);



		//Sets up components
		std::string Credits = "Credits.png"; //400, 200
		ECS::GetComponent<Sprite>(entityCred).LoadSprite(Credits, 380, 200);
		ECS::GetComponent<Transform>(entityCred).SetPosition(vec3(0.f, 0.f, 20.f));

		//Setup up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entityCred, bitHolder, "Credit Entity");

		if (Menuposition.z <= 0)
		{
			Menuposition.z == 20;
		}

		ECS::GetComponent<Transform>(menuEntity).SetPositionZ(Menuposition.z - 20);

	}

	// Escapes from selection back to menu
	if (Input::GetKeyUp(Key::Escape))
	{

		ECS::GetComponent<Transform>(menuEntity).SetPositionZ(Menuposition.z + 20);

		if (Menuposition.z >= 20)
		{
			Menuposition.z = 20;
		}

	}


}





void Game::Hearts()
{
	BioshockScene* hearts = (BioshockScene*)m_activeScene;
	auto HeartsEntity = hearts->GetHealth1();
	vec3 heartspos = m_register->get<Transform>(HeartsEntity).GetPosition();
	auto& animHearts = ECS::GetComponent<AnimationController>(HeartsEntity);

	

	if (Input::GetKeyUp(Key::T))
	{
		HealthBarnum++;

		animHearts.SetActiveAnim(HealthBarnum);
		animHearts.GetActiveAnim();

	}
}












void Game::GamepadStroke(XInputController * con)
{
	

	
}

void Game::GamepadUp(XInputController * con)
{
	//Gamepad button up
	if (con->IsButtonReleased(Buttons::RB))
	{
		//printf("RB Released\n");
	}
}

void Game::GamepadDown(XInputController * con)
{
	//Gamepad button down
	if (con->IsButtonPressed(Buttons::THUMB_LEFT))
	{
		printf("Left Thumbstick Clicked In.\n");
	}
}

void Game::GamepadStick(XInputController * con)
{
	

}

void Game::GamepadTrigger(XInputController * con)
{
	//Gamepad trigger stuffs
	Triggers triggers;
	con->GetTriggers(triggers);

	if (triggers.RT > 0.5f && triggers.RT < 0.8f)
	{
		printf("Half Press\n");
	}

	if (triggers.RT > 0.8f)
	{
		printf("Full Press\n");
	}

}


/* We found how to use sound here -> https://adamtcroft.com/playing-sound-with-sdl-c/ */