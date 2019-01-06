#include "StdAfx.h"
#include "Actor.h"
#include <CryMath/Random.h>


uint64 CActor::GetEventMask() const
{
	 return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CActor::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_INIT:
	case ENTITY_EVENT_START_GAME:
	case ENTITY_EVENT_RESET:
		Revive();
		break;
	case ENTITY_EVENT_UPDATE:
		const auto pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);
		break;
	}
}


void CActor::Initialize()
{
	Logger::Get().Log(GetEntity()->GetName(), "initialized");

	m_pCharacterController = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	m_pAnimationComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CAdvancedAnimationComponent>();
	m_pAnimationComponent->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
	m_pAnimationComponent->SetCharacterFile("Objects/Characters/SampleCharacter/thirdperson.cdf");

	m_pAnimationComponent->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimationComponent->SetDefaultScopeContextName("FirstPersonCharacter");
	m_pAnimationComponent->SetDefaultFragmentName("Idle");
	m_pAnimationComponent->SetAnimationDrivenMotion(false);

	m_pAnimationComponent->LoadFromDisk();

	m_idleFragmentId = m_pAnimationComponent->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimationComponent->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimationComponent->GetTagId("Rotate");

	m_pPathfindingComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CPathfindingComponent>();
	m_pPathfindingComponent->SetMaxAcceleration(m_movementSpeed);

	m_pNavigationComponent = m_pEntity->GetOrCreateComponent<IEntityNavigationComponent>();

	IEntityNavigationComponent::SMovementProperties movementProperties;
	movementProperties.normalSpeed = m_movementSpeed;
	movementProperties.maxSpeed = m_movementSpeed;
	movementProperties.maxAcceleration = m_movementSpeed;
	m_pNavigationComponent->SetMovementProperties(movementProperties);
	m_pNavigationComponent->SetNavigationAgentType("MediumSizedCharacters");

	m_pNavigationComponent->SetStateUpdatedCallback([this](const Vec3& recommendedVelocity)
	{
		m_pCharacterController->ChangeVelocity(recommendedVelocity, Cry::DefaultComponents::CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);
	});

	m_pCoverUserComponent = GetEntity()->GetOrCreateComponent<IEntityCoverUserComponent>();


	m_pObservableComponent = GetEntity()->GetOrCreateComponent<IEntityObservableComponent>();

	//m_pBehaviorTreeComponent = GetEntity()->GetOrCreateComponent<IEntityBehaviorTreeComponent>();
	//m_pBehaviorTreeComponent->SetBBKeyValue()

	m_pFactionComponent = GetEntity()->GetOrCreateComponent<IEntityFactionComponent>();
	m_pFactionComponent->SetFactionId(cry_random_uint32() % 10);

	m_pListenerComponent = GetEntity()->GetOrCreateComponent<IEntityListenerComponent>();



}


void CActor::Revive()
{
	Health = 100;
	this->enemyQueue.clear();
	this->actorState = NO_STATE;

	GetEntity()->Hide(false);
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	m_pAnimationComponent->ResetCharacter();
	m_pCharacterController->Physicalize();
	
	m_activeFragmentId = FRAGMENT_ID_INVALID;
}

void CActor::UpdateAI(float fFrameTime)
{
	switch(this->actorState)
	{
	case NO_STATE:
		actorState = Idle;

		break;
	case Idle:
		Logger::Get().Log(GetEntity()->GetName(), "idle");
		CanSeeEnemy();
		if(enemyQueue.size())
			actorState = Attacking;
		break;
	case Alarmed:
		Logger::Get().Log(GetEntity()->GetName(), "Alarmed");
		CanSeeEnemy();
		if (enemyQueue.size())
			actorState = Attacking;
		break;
	case Attacking: 
		Logger::Get().Log(GetEntity()->GetName(), "Attacking");
		if(enemyQueue.size() > 0)
			AttackActor(enemyQueue[0]);
		break;
	case Fleeing:
		Logger::Get().Log(GetEntity()->GetName(), "Fleeing");
		Hide();
		break;
	case Killed: 
		Logger::Get().Log(GetEntity()->GetName(), "Killed");

		break;
	default: ;
	}
}

void CActor::Update(float fFrameTime)
{
	gEnv->pAuxGeomRenderer->DrawSphere(GetEntity()->GetWorldPos(), 0.25f, ColorF(1, 1, 0, .5));

	m_aiupdatetime += fFrameTime;
	if (m_aiupdatetime > 1) {
		UpdateAI(fFrameTime);
		m_aiupdatetime = 0;
		Snackbar::Get().Log(GetEntity()->GetName(), 0.1);
	}

}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActor>)
