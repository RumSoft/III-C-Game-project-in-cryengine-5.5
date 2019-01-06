#include "StdAfx.h"
#include "ActorController.h"

void CActorController::Initialize()
{
	//CAdvancedAnimationComponent
	m_pAnimation = GetEntity()->GetOrCreateComponent<CAdvancedAnimationComponent>();

	m_pAnimation->SetMannequinAnimationDatabaseFile("Animations/Mannequin/ADB/FirstPerson.adb");
	m_pAnimation->SetCharacterFile("Objects/Characters/SampleCharacter/thirdperson.cdf");

	m_pAnimation->SetControllerDefinitionFile("Animations/Mannequin/ADB/FirstPersonControllerDefinition.xml");
	m_pAnimation->SetDefaultScopeContextName("FirstPersonCharacter");
	m_pAnimation->SetDefaultFragmentName("Idle");
	m_pAnimation->SetAnimationDrivenMotion(false);
	m_idleFragmentId = m_pAnimation->GetFragmentId("Idle");
	m_walkFragmentId = m_pAnimation->GetFragmentId("Walk");
	m_rotateTagId = m_pAnimation->GetTagId("Rotate");
	m_pAnimation->LoadFromDisk();

	//CCharacterControllerComponent
	m_pCharacterController	= GetEntity()->GetOrCreateComponent<CCharacterControllerComponent>();
	m_pCharacterController->SetTransformMatrix(Matrix34::Create(Vec3(1.f), IDENTITY, Vec3(0, 0, 1.f)));


	//CPathfindingComponent
	m_pPathfindingComponent	= GetEntity()->GetOrCreateComponent<CPathfindingComponent>();
	m_pPathfindingComponent->SetMaxAcceleration(m_movementSpeed);
	m_pPathfindingComponent->SetMovementRecommendationCallback([this](const Vec3& recommendedVelocity)
	{
		m_pCharacterController->ChangeVelocity(recommendedVelocity, Cry::DefaultComponents::CCharacterControllerComponent::EChangeVelocityMode::SetAsTarget);
	});

	//IEntityBehaviorTreeComponent
	m_pBehaviorTree = GetEntity()->GetOrCreateComponent<IEntityBehaviorTreeComponent>();

	//IEntityNavigationComponent
	m_pNavigation = GetEntity()->GetOrCreateComponent<IEntityNavigationComponent>();

	//IEntityCoverUserComponent
	m_pCoverUser = GetEntity()->GetOrCreateComponent<IEntityCoverUserComponent>();

}

uint64 CActorController::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) 
	| ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) 
	| ENTITY_EVENT_BIT(ENTITY_EVENT_RESET);
}

void CActorController::ProcessEvent(const SEntityEvent& event)
{

}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActorController>)