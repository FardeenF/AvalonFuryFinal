#include "BioshockScene.h"
#include "Game.h"

BioshockScene::BioshockScene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(0.f)); //Set Gravity (only second value)
	m_physicsWorld->SetGravity(m_gravity);
}

void BioshockScene::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register (so when you unload the scene when you switch between scenes
	//you can reInit this scene
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//m_listener = BioshockContactListener(m_sceneReg);
	//m_physicsWorld->SetContactListener(&m_listener);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera Entity
	{
		//Creates Camera Entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to the horizontal scroll
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);

		//Attaches the camera to the vertical scroll
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetOffset(15.f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::HoriScrollCameraBit() | EntityIdentifier::VertScrollCameraBit() | EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Horizontal and Vertical Scrolling Cam");
		ECS::SetIsMainCamera(entity, true);
	}
	/*
	//Setup Health Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Creates new orthographic camera
		ECS::AttachComponent<HealthBar>(entity);
		ECS::GetComponent<HealthBar>(entity).SetHealth(0.7f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::HealthBarBit();
		ECS::SetUpIdentifier(entity, bitHolder, "HealthBar Entity");
	}
	*/


	//Setup Background Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "BioshockFullBG.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 800.f), vec2(1200.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 1000, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 1.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Background");


	}

	//Setup Extended Background Entity
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "ExtendedBG.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 800.f), vec2(1200.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 1000, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-550.f, 0.f, 1.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Background");


	}

	/*
	//Setup R1 Boundry Ceiling Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-380.f), float32(415.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);

		

		tempPhsBody = PhysicsBody(tempBody, float(500.f), float(20),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R1 Boundry Leftwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-498.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(1000.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R1 Boundry Floor Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-380.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(500.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R1 Boundry Rightwall1 Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-145.f), float32(410.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(200.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R1 Boundry Rightwall2 Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-145.f), float32(165.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(120.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R2 Boundry Floor Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-70.f), float32(215.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(170.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R2 Boundry Ceiling Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-10.f), float32(320.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(280.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R2 Boundry Rightwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(80.f), float32(200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(280.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R2 Boundry Leftwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(5.f), float32(150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(150.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R3 Boundry Ceiling1 Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-84.f), float32(70.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(200.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R3 Boundry Ceiling2 Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(106.f), float32(70.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(70.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R3 Boundry Leftwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-125.f), float32(-150.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(500.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R3 Boundry Rightwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(120.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(400.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R3 Boundry Floor Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(20.f), float32(-370.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(700.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R3 Boundry Ceiling3 Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(200.f), float32(-160.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(150.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R4 Boundry Rightwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(338.f), float32(-100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(600.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R4 Boundry Leftwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(265.f), float32(20.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(370.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R5 Boundry Leftwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(250.f), float32(305.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(400.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R5 Boundry Rightwall Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(430.f), float32(305.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(400.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R5 Boundry Floor Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(385.f), float32(200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(115.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup R5 Boundry Ceiling Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(345.f), float32(404.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(200.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup Out of Bounds Leftwall Entity1
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-1010.f), float32(180.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(200.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup Out of Bounds Rightwall Entity2
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-850.f), float32(180.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(20.f), float(200.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup Out of Bounds Ceiling Entity3
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-935.f), float32(250.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(200.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup Out of Bounds Floor Entity4
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Empty.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 1.f), vec2(1.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 400.f, 1.f));  //Sets Location

		//Sets up components

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-935.f), float32(80.f)); // Inside Cage (-930, 165)

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);



		tempPhsBody = PhysicsBody(tempBody, float(200.f), float(20.f),
			vec2(0.f, 0.f), false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boundry");


	}

	//Setup Audio Diary Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("AudioDiary.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "AudioDiary.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Empty"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["Fill"]);
		animController.GetAnimation(1).SetRepeating(true);
		animController.SetActiveAnim(1);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-400.f, 220.f, 2.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Audio Diary");

		m_diaryEntity = entity;
	}

	//Setup Message Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Message.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 800.f), vec2(1200.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 100, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1000.f, -2000.f, 34.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Message");

		m_speechEntity = entity;

	}
	*/

	//Setup Jack Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("JackAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);
		EntityIdentifier::MainPlayer(entity);

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "JackSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds animations
		animController.AddAnimation(animation["AttackDown"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["AttackLeft"]);
		animController.GetAnimation(1).SetRepeating(true);
		animController.AddAnimation(animation["AttackRight"]);
		animController.GetAnimation(2).SetRepeating(true);
		animController.AddAnimation(animation["AttackUp"]);
		animController.GetAnimation(3).SetRepeating(true);
		animController.AddAnimation(animation["CastDown"]);
		animController.GetAnimation(4).SetRepeating(true);
		animController.AddAnimation(animation["CastLeft"]);
		animController.GetAnimation(5).SetRepeating(true);
		animController.AddAnimation(animation["CastRight"]);
		animController.GetAnimation(6).SetRepeating(true);
		animController.AddAnimation(animation["CastUp"]);
		animController.GetAnimation(7).SetRepeating(true);
		animController.AddAnimation(animation["WalkDown"]);
		animController.GetAnimation(8).SetRepeating(true);
		animController.AddAnimation(animation["Hurt"]);
		animController.GetAnimation(9).SetRepeating(true);
		animController.AddAnimation(animation["Idle"]);
		animController.GetAnimation(10).SetRepeating(true);
		animController.AddAnimation(animation["WalkLeft"]);
		animController.GetAnimation(11).SetRepeating(true);
		animController.AddAnimation(animation["WalkRight"]);
		animController.GetAnimation(12).SetRepeating(true);
		animController.AddAnimation(animation["WalkUp"]);
		animController.GetAnimation(13).SetRepeating(true);
		

		//Sets active animation
		animController.SetActiveAnim(10);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 80.f, 50.f));  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-400.f), float32(180.f));  //tempDef.position.Set(float32(-400.f), float32(180.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);

		//tempPhsBody.SetBodyID(2);
		//tempPhsBody.SetCollideID(1);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, 0.f), false);


		tempPhsBody.SetBodyID(1);
		tempPhsBody.SetCollideID(1);

		tempBody->SetFixedRotation(true);


		tempBody->SetLinearDamping(4.0f);
		

		//Sets the percentage of velocity lost to friction
		//tempPhsBody.SetFriction(0.15f);
		//What is the max velocity in each axis for this body?
		//tempPhsBody.SetMaxVelo(60.f);
		//Is this body affected by gravity?
		//tempPhsBody.SetGravity(false);



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::PlayerBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Jack");

		m_playerEntity = entity;
	}



	{
	//Loading Animation File
	auto animation = File::LoadJSON("Slash.json");

	//Creates Entity
	auto SlashEntity = ECS::CreateEntity();


	//Add Components
	ECS::AttachComponent<Sprite>(SlashEntity);
	ECS::AttachComponent<Transform>(SlashEntity);
	ECS::AttachComponent<AnimationController>(SlashEntity);
	

	//Sets up components
	std::string fileName = "slash.png";
	auto& animSlash = ECS::GetComponent<AnimationController>(SlashEntity);
	animSlash.InitUVs(fileName);

	//Adds animations
	animSlash.AddAnimation(animation["slash"]); // anim 0


	//Sets active animation
	animSlash.SetActiveAnim(0);

	ECS::GetComponent<Sprite>(SlashEntity).LoadSprite(fileName, 50, 50, true, &animSlash); //Sets Size of Sprites

	
	//ECS::GetComponent<Transform>(SlashEntity).SetPosition(vec3(10.f, 80.f, 60.f));  //Sets Location
	
	

	//Sets up the Identifier
	unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
	ECS::SetUpIdentifier(SlashEntity, bitHolder, "Slash");

	m_slashEntity = SlashEntity;
	}

	{
		//Loading Animation File
		auto animation = File::LoadJSON("hearts.json");

		//Creates Entity
		auto HeartsEntity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(HeartsEntity);
		ECS::AttachComponent<Transform>(HeartsEntity);
		ECS::AttachComponent<AnimationController>(HeartsEntity);


		//Sets up components
		std::string fileName = "Hearts1.png";
		auto& animHearts = ECS::GetComponent<AnimationController>(HeartsEntity);
		animHearts.InitUVs(fileName);

		//Adds animations
		animHearts.AddAnimation(animation["3"]); // anim 0
		animHearts.AddAnimation(animation["2.5"]); // anim 1
		animHearts.AddAnimation(animation["2"]); // anim 2
		animHearts.AddAnimation(animation["1.5"]); // anim 3
		animHearts.AddAnimation(animation["1"]); // anim 4
		animHearts.AddAnimation(animation["0.5"]); // anim 5
		animHearts.AddAnimation(animation["0"]); // anim 6


		//Sets active animation
		animHearts.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(HeartsEntity).LoadSprite(fileName, 40, 15, true, &animHearts); //Sets Size of Sprites


		ECS::GetComponent<Transform>(HeartsEntity).SetPosition(vec3(23.f, 25.f, 65.f));  //Sets Location



		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(HeartsEntity, bitHolder, "Hearts");

		m_heartEntity = HeartsEntity;


	}




	/*
	//Setup Enemy Splicer1 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("SplicerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "SplicerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["WalkDown"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["WalkUp"]);
		animController.GetAnimation(1).SetRepeating(true);
		

		//Sets active animation
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-11.f, 273.f, 40.f));  //Sets Location


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-11.f), float32(273.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*) entity);

		tempPhsBody.SetBodyID(-1);
		tempPhsBody.SetCollideID(-1);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, 0.f), false);

		tempBody->SetFixedRotation(true);


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::EnemyBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Splicer");

		m_trainerEntity = entity;
	}

	//Setup Enemy Splicer2 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("SplicerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "SplicerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["WalkDown"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["WalkUp"]);
		animController.GetAnimation(1).SetRepeating(true);


		//Sets active animation
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-11.f, 273.f, 40.f));  //Sets Location


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-70.f), float32(-15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);

		tempPhsBody.SetBodyID(-1);
		tempPhsBody.SetCollideID(-1);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, 0.f), false);

		tempBody->SetFixedRotation(true);


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::EnemyBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Splicer");

		m_splicer2Entity = entity;
	}

	//Setup Enemy Splicer3 Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("SplicerAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "SplicerSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["WalkDown"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["WalkUp"]);
		animController.GetAnimation(1).SetRepeating(true);


		//Sets active animation
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-11.f, 273.f, 40.f));  //Sets Location


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(275.f), float32(-245.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);

		tempPhsBody.SetBodyID(-1);
		tempPhsBody.SetCollideID(-1);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, 0.f), false);

		tempBody->SetFixedRotation(true);


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::EnemyBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Splicer");

		m_splicer3Entity = entity;
	}


	//Setup Enemy Boss Entity
	{
		//Loading Animation File
		auto animation = File::LoadJSON("BossAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "BossSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["WalkDown"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["WalkUp"]);
		animController.GetAnimation(1).SetRepeating(true);


		//Sets active animation
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 60, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-11.f, 273.f, 44.f));  //Sets Location


		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		float shrinkX = tempSpr.GetWidth() / 2.f;
		float shrinkY = tempSpr.GetWidth() / 2.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(310.f), float32(350.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);

		tempPhsBody.SetBodyID(-1);
		tempPhsBody.SetCollideID(-1);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY),
			vec2(0.f, 0.f), false);

		tempBody->SetFixedRotation(true);


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::EnemyBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Boss");

		m_splicer4Entity = entity;
	}

	
	//Setup Bolt Entity
	{
		
		//Loading Animation File
		auto animation = File::LoadJSON("BoltAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();

		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);


		//Sets up components
		std::string fileName = "BoltsSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Vertical"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["Horizontal"]);
		animController.GetAnimation(1).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 20, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(80.f, 100.f, 56.f);  //Sets Location

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//Physics body covers the entire sprite
		//ID type is player
		//Collides with Enemies and Environments
		//Moves
		tempPhsBody.SetBodyID(2);
		tempPhsBody.SetCollideID(2);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(1000.f), float32(100.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempBody->SetUserData((void*)entity);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), false);

		tempBody->SetFixedRotation(true);
		tempBody->SetBullet(true);




		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit() | EntityIdentifier::AttackBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Bolt");

		m_boltEntity = entity;
		
		
		
		
	}

	//Setup Blood Entity1
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "BloodSprite.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1050.f, 1000.f, 32.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Blood");

		m_bloodEntity1 = entity;
	}

	//Setup Blood Entity2
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "BloodSprite.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1080.f, 1000.f, 16.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Blood");

		m_bloodEntity2 = entity;
	}

	//Setup Blood Entity3
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "BloodSprite.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1080.f, 1000.f, 18.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Blood");

		m_bloodEntity3 = entity;
	}

	//Setup Blood Entity4
	{
		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "BloodSprite.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 64.f), vec2(64.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1080.f, 1000.f, 19.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Blood");

		m_bloodEntity4 = entity;
	}

	//Setup Hearts
	{
		//Loading Animation File
		auto animation = File::LoadJSON("HeartAnimations.json");

		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "HeartSpriteSheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);

		//Adds first animation
		animController.AddAnimation(animation["Heart6"]);
		animController.GetAnimation(0).SetRepeating(true);
		animController.AddAnimation(animation["Heart5"]);
		animController.GetAnimation(1).SetRepeating(true);
		animController.AddAnimation(animation["Heart4"]);
		animController.GetAnimation(2).SetRepeating(true);
		animController.AddAnimation(animation["Heart3"]);
		animController.GetAnimation(3).SetRepeating(true);
		animController.AddAnimation(animation["Heart2"]);
		animController.GetAnimation(4).SetRepeating(true);
		animController.AddAnimation(animation["Heart1"]);
		animController.GetAnimation(5).SetRepeating(true);

		//Sets active animation
		animController.SetActiveAnim(0);


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1050.f, 1000.f, 88.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Hearts");

		m_heartEntity = entity;
	}


	//Setup Interact Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Interact.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 800.f), vec2(1200.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 100, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-390.f, 180.f, 38.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Interact");

		m_interactEntity = entity;

	}

	//Setup Win Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Win.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 800.f), vec2(1200.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 400, 400, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1000.f, -2000.f, 99.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Interact");

		m_winEntity = entity;

	}

	//Setup Lose Entity
	{

		//Creates Entity
		auto entity = ECS::CreateEntity();


		//Add Components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up components
		std::string fileName = "Lose.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		//Adds first animation
		animController.AddAnimation(Animation());
		//Sets active animation
		animController.SetActiveAnim(0);
		//Gets first animation
		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(0.f, 800.f), vec2(1200.f, 0.f));

		//Make it repeat
		anim.SetRepeating(true);
		//Sets the time between frames
		anim.SetSecPerFrame(0.1667f); //Same is 60fps


		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 400, 400, true, &animController); //Sets Size of Sprites

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-1000.f, -2000.f, 98.f));  //Sets Location


		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Interact");

		m_loseEntity = entity;

	}
	*/

	//Makes the camera focus on the main player
	//We do this at the very bottom so we get the most accurate pointer to our Transform
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));;

}

//Get Player Entity
int BioshockScene::GetPlayer()
{
	return m_playerEntity;
}

int BioshockScene::GetSlash()
{
	return m_slashEntity;
}

int BioshockScene::GetHealth1()
{
	return m_heartEntity;
}


//Get Audio Diary Entity
int BioshockScene::GetDiary()
{
	return m_diaryEntity;
}

int BioshockScene::GetSpeech()
{
	return m_speechEntity;
}

int BioshockScene::GetInteract()
{
	return m_interactEntity;
}

int BioshockScene::GetWin()
{
	return m_winEntity;
}

int BioshockScene::GetLose()
{
	return m_loseEntity;
}



//Get Enemy trainer Entity
int BioshockScene::GetTrainer()
{
	return m_trainerEntity;
}

//Get Bolt Entity
int BioshockScene::GetBolt()
{
	return m_boltEntity;
}

int BioshockScene::GetBlood1()
{
	return m_bloodEntity1;
}

int BioshockScene::GetBlood2()
{
	return m_bloodEntity2;
}

int BioshockScene::GetBlood3()
{
	return m_bloodEntity3;
}

int BioshockScene::GetBlood4()
{
	return m_bloodEntity4;
}


int BioshockScene::GetHeart()
{
	return m_heartEntity;
}


int BioshockScene::GetSplicer2()
{
	return m_splicer2Entity;
}

int BioshockScene::GetSplicer3()
{
	return m_splicer3Entity;
}

int BioshockScene::GetSplicer4()
{
	return m_splicer4Entity;
}

