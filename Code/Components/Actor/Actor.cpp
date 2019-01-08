#include "StdAfx.h"
#include "Actor.h"
#include <CryMath/Random.h>
#include "AI/Actions/IAction.h"


uint64 CActor::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_RESET);
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
	m_pActorController = GetEntity()->GetOrCreateComponentClass<CActorController>();
}


void CActor::Revive()
{
	GetEntity()->Hide(false);
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));
}


void CActor::Update(float fFrameTime)
{
	gEnv->pAuxGeomRenderer->DrawSphere(GetEntity()->GetWorldPos(), 0.25f, ColorF(1, 1, 0, .5));
	slowupdate += fFrameTime;
	if(slowupdate >= 5)
	{
		slowupdate = 0;
		ActorAction* x = nullptr;

		x = new HideInCoverAction();
		x->Process(GetController());
		delete x;
		//GetController()->GetNavigationComponent()->NavigateTo(GetEntity()->GetWorldPos() + Vec3(4, 4, 0));
		//GetController()->GetPathfindingComponent()->RequestMoveTo(GetEntity()->GetWorldPos() + Vec3(4, 4, 0));
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActor>)
