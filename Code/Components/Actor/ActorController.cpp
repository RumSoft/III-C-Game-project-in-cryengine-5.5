#include "StdAfx.h"
#include "ActorController.h"

void CActorController::Initialize()
{
	//CAdvancedAnimationComponent
	m_pAdvancedAnimationComponent	= GetEntity()->GetOrCreateComponent<CAdvancedAnimationComponent>();
	
	//CCharacterControllerComponent
	m_pCharacterControllerComponent	= GetEntity()->GetOrCreateComponent<CCharacterControllerComponent>();

	//CPathfindingComponent
	m_pPathfindingComponent			= GetEntity()->GetOrCreateComponent<CPathfindingComponent>();

	//IEntityBehaviorTreeComponent
	m_pBehaviorTreeComponent		= GetEntity()->GetOrCreateComponent<IEntityBehaviorTreeComponent>();

	//IEntityNavigationComponent
	m_pNavigationComponent			= GetEntity()->GetOrCreateComponent<IEntityNavigationComponent>();

	//IEntityCoverUserComponent
	m_pCoverUserComponent			= GetEntity()->GetOrCreateComponent<IEntityCoverUserComponent>();

}

uint64 CActorController::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CActorController::ProcessEvent(const SEntityEvent& event)
{

}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActorController>)