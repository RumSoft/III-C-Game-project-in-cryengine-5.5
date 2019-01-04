#include "StdAfx.h"
#include "Actor.h"

uint64 CActor::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CActor::ProcessEvent(const SEntityEvent& event)
{
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActor>)