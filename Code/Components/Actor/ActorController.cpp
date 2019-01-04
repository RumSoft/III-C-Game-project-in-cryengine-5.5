#include "StdAfx.h"
#include "ActorController.h"

uint64 CActorController::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CActorController::ProcessEvent(const SEntityEvent& event)
{

}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActorController>)