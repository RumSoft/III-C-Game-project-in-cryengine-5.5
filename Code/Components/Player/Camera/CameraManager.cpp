#include "StdAfx.h"
#include "CameraManager.h"

void CCameraManager::Initialize()
{

}

uint64 CCameraManager::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CCameraManager::ProcessEvent(const SEntityEvent& event)
{
	const auto pCtx = (SEntityUpdateContext*)event.nParam[0];
	switch(event.event)
	{
	case ENTITY_EVENT_UPDATE:
		break;
	}
}
