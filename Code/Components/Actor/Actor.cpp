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

CActor::~CActor()
{
	delete _healthAttribute;
}

void CActor::Initialize()
{
	Logger::Get().Log(GetEntity()->GetName(), "initialized");
	m_pActorController = GetEntity()->GetOrCreateComponentClass<CActorController>();
	_healthAttribute = new CAttribute(100.f, 10.f, 2.f);
	_healthAttribute->SetAttributeEmptyCallback([this]() {
		Logger::Get().Log(GetEntity()->GetName(), "killed");
	});
}

void CActor::Revive()
{
	GetEntity()->Hide(false);
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));
	
}


void CActor::Update(float fFrameTime)
{
	gEnv->pAuxGeomRenderer->DrawSphere(GetEntity()->GetWorldPos(), 0.25f, ColorF(1, 1, 0, .5));
	
	_healthAttribute->Update(fFrameTime);

}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CActor>)
