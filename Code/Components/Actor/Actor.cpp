#include "StdAfx.h"
#include "Actor.h"


uint64 CActor::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CActor::ProcessEvent(const SEntityEvent& event)
{
	switch(event.event)
	{
	case ENTITY_EVENT_UPDATE:
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);
	}
}

void CActor::Update(float fFrameTime)
{
	gEnv->pAuxGeomRenderer->DrawSphere(GetEntity()->GetWorldPos(), 0.5f, ColorF(1, 1, 0,.5));


}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActor>)
