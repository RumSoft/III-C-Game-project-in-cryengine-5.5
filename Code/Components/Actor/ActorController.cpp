#include "StdAfx.h"
#include "ActorController.h"
#include <CryAISystem/IMovementSystem.h>

CActorController::~CActorController()
{
	Logger::Get().Log(GetEntity()->GetName(), "");
}

void CActorController::Initialize()
{
	//create components
	m_pAnimation = GetEntity()->GetOrCreateComponent<CAdvancedAnimationComponent>();
	m_pCharacterController = GetEntity()->GetOrCreateComponent<CCharacterControllerComponent>();
	m_pPathfindingComponent = GetEntity()->GetOrCreateComponent<CPathfindingComponent>();
	m_pNavigation = GetEntity()->GetOrCreateComponent<IEntityNavigationComponent>();

	//CAdvancedAnimationComponent
	m_pAnimation->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
	m_pAnimation->SetCharacterFile("Objects/Characters/SampleCharacter/thirdperson.cdf");
	m_pAnimation->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimation->SetDefaultScopeContextName("FirstPersonCharacter");
	m_pAnimation->SetDefaultFragmentName("Idle");
	m_pAnimation->SetAnimationDrivenMotion(false);
	m_pAnimation->LoadFromDisk();
	m_pAnimation->ResetCharacter();

	m_idleFragmentId = m_pAnimation->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimation->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimation->GetTagId("Rotate");

	//CCharacterControllerComponent
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));

	m_pPathfindingComponent->SetMovementRecommendationCallback([this](const Vec3& recommendedVelocity)	{
		m_pCharacterController->ChangeVelocity(recommendedVelocity,
		                                       CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);
	});
	
	//IEntityNavigationComponent
	m_pNavigation->SetCollisionAvoidanceProperties(IEntityNavigationComponent::SCollisionAvoidanceProperties{
		1.f, IEntityNavigationComponent::SCollisionAvoidanceProperties::EType::Active
		});
	m_pNavigation->SetMovementProperties(IEntityNavigationComponent::SMovementProperties{
		m_movementSpeed, m_movementSpeed, m_movementSpeed, m_movementSpeed, m_movementSpeed, 2, true
		});
	m_pNavigation->SetStateUpdatedCallback([this](const Vec3& recommendedVelocity) {
		m_pCharacterController->ChangeVelocity(recommendedVelocity,
		                                       Cry::DefaultComponents::CCharacterControllerComponent::
		                                       EChangeVelocityMode::SetAsTarget);
	});
	m_pNavigation->SetNavigationAgentType("MediumSizedCharacters");

}

uint64 CActorController::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_RESET)
		| ENTITY_EVENT_BIT(Cry::Entity::EEvent::TransformChangeFinishedInEditor);
}

void CActorController::ProcessEvent(const SEntityEvent& event)
{
	switch(event.event)
	{
	case ENTITY_EVENT_START_GAME:
	case ENTITY_EVENT_RESET:
	case Cry::Entity::EEvent::TransformChangeFinishedInEditor:
		Revive();
		break;
	case ENTITY_EVENT_UPDATE:
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);
		break;
	}
}

void CActorController::Update(float fFrameTime)
{
	UpdateAnimation(fFrameTime);
}

void CActorController::UpdateAnimation(float fFrameTime)
{

	// Update active fragment
	const auto& desiredFragmentId = m_pCharacterController->IsWalking() ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId)
	{
		m_activeFragmentId = desiredFragmentId;
		m_pAnimation->QueueFragmentWithId(m_activeFragmentId);
	}


	//// Send updated transform to the entity, only orientation changes
	//GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), 
	//	GetEntity()->GetWorldRotation(), 
	//	Vec3(1, 1, 1));
	IEntity* target = nullptr;
	if (target == nullptr) {
		auto dir = m_pCharacterController->GetMoveDirection();
		if(m_pCharacterController->IsWalking())
			GetEntity()->SetRotation(Quat::CreateRotationZ(-atan2(dir.y,dir.x)));
	}
}



void CActorController::Revive()
{
	Snackbar::Get().Log(GetEntity()->GetName());
	GetEntity()->Hide(false);
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));
	m_pCharacterController->SetVelocity(ZERO);
	m_pCharacterController->Physicalize();
	m_pAnimation->ResetCharacter();
	
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActorController>)
