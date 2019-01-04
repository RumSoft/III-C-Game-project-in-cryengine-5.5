#include "StdAfx.h"
#include "IActor.h"

void IActor::ProcessEvent(const SEntityEvent& event)
{
	if (event.event == ENTITY_EVENT_UPDATE)
	{
		const float frameTime = event.fParam[0];
	}
}

uint64 IActor::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}
