#include "BioshockContactListener.h"
#include "BioshockScene.h"
#include "ECS.h"

BioshockContactListener::BioshockContactListener(entt::registry* reg)
{
	m_register = reg;
}

void BioshockContactListener::BeginContact(b2Contact* contact)
{


	auto fixtureA = contact->GetFixtureA();
	auto bodyA = fixtureA->GetBody();
	unsigned int entityA = (unsigned int)bodyA->GetUserData();

	if (!m_register->valid(entityA))
		return;


	auto identifierA = m_register->get<EntityIdentifier>(entityA).GetIdentifier();


	auto fixtureB = contact->GetFixtureB();
	auto bodyB = fixtureB->GetBody();
	unsigned int entityB = (unsigned int)bodyB->GetUserData();

	if (!m_register->valid(entityB))
		return;


	auto identifierB = m_register->get<EntityIdentifier>(entityB).GetIdentifier();

	//Check Body A
	if (identifierA & EntityIdentifier::EnemyBit())
	{
		//m_register->destroy(entityA);
		if (identifierB & EntityIdentifier::PlayerBit())
		{
			if (bodyB->GetLinearVelocity().Length() > 10.f)
			{
				//m_register->destroy(entityA);
			}
		}

		
	}

	//Check Body B
	if (identifierB & EntityIdentifier::EnemyBit())
	{
		//m_register->destroy(entityB);
		if (identifierA & EntityIdentifier::PlayerBit())
		{
			if (bodyA->GetLinearVelocity().Length() > 10.f)
			{
				//m_register->destroy(entityB);
			}
		}

		
	}
}
