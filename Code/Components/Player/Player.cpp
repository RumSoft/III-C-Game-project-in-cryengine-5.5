#include "StdAfx.h"
#include "Player.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryEntitySystem/IEntity.h>
#include <CryAction/IActionMapManager.h>
#include "Utils/Logger.h"
#include "Utils/StringConversions.h"
#include <CryInput/IHardwareMouse.h>

void CPlayerComponent::Initialize()
{
	// Create the camera component, will automatically update the viewport every frame
	m_pCameraManager = GetEntity()->GetOrCreateComponent<CCameraManager>();
	// Get the input component, wraps access to action mapping so we can easily get callbacks when inputs are triggered
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	m_pActor = GetEntity()->GetOrCreateComponent<CActor>();

	m_pInputComponent->RegisterAction("player", "walkto", [this](int activationMode, float value)
	{
		if (activationMode == eAAM_OnPress)
		{
			Snackbar::Get().Log("clicked, navigating");
			GetActor()->QueueAction(new MoveToAction(_mousePos));
		}
	});
	m_pInputComponent->BindAction("player", "walkto", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

	Revive();
}

uint64 CPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE)
		| ENTITY_EVENT_BIT(ENTITY_EVENT_RESET);
}

void CPlayerComponent::Update(float fFrameTime)
{
	Logger::Get().Log("player position", Vec3ToString(GetEntity()->GetWorldPos()));
	Logger::Get().Log("player orientantion", QuatToString(GetEntity()->GetWorldRotation()));

	gEnv->pAuxGeomRenderer->Draw2dLabel(10, 10, 1.75, ColorF(1, 1, 1), false, Logger::Get().ReadLog());
	gEnv->pAuxGeomRenderer->Draw2dLabel(30, 300,
		1.75, ColorF(1, 1, 1), false, Snackbar::Get().ReadLog());

	UpdateMouse(fFrameTime);
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_RESET:
		// Check if we're entering game mode
		break;
	case ENTITY_EVENT_START_GAME:
		Revive();
		break;
	case ENTITY_EVENT_UPDATE:
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];
		Update(pCtx->fFrameTime);

		break;
	}
}

void CPlayerComponent::Revive()
{

	// Set player transformation, but skip this in the Editor
	if (!gEnv->IsEditor())
	{
		Vec3 playerScale = Vec3(1.f);
		Quat playerRotation = IDENTITY;

		// Offset the player in the center of the map
		const float heightOffset = 20.f;
		float terrainCenter = gEnv->p3DEngine->GetTerrainSize() / 2.f;
		float height = gEnv->p3DEngine->GetTerrainZ(terrainCenter, terrainCenter);
		Vec3 playerPosition = Vec3(terrainCenter, terrainCenter, height + heightOffset);

		m_pEntity->SetWorldTM(Matrix34::Create(playerScale, playerRotation, playerPosition));
	}



	// Unhide the entity in case hidden by the Editor
	GetEntity()->Hide(false);
}

void CPlayerComponent::UpdateMouse(float fFrameTime)
{
	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&_mouseScreen.x, &_mouseScreen.y);

	// Invert mouse Y
	_mouseScreen.y = gEnv->pRenderer->GetHeight() - _mouseScreen.y;

	Vec3 p1(0, 0, 0), p2(0,0,0);
	gEnv->pRenderer->UnProjectFromScreen(_mouseScreen.x, _mouseScreen.y, 0, &p1.x, &p1.y, &p1.z);
	gEnv->pRenderer->UnProjectFromScreen(_mouseScreen.x, _mouseScreen.y, 1, &p2.x, &p2.y, &p2.z);

	Vec3 dir = p2 - p1;
	dir.Normalize();


	if (gEnv->pPhysicalWorld->RayWorldIntersection(p1, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all,
	                                               rwi_stop_at_pierceable | rwi_colltype_any, &_mouseRaycastHit, 1))
	{
		_mousePos = _mouseRaycastHit.pt;
		gEnv->pAuxGeomRenderer->DrawSphere(_mousePos, 0.1, ColorB(255, 0, 255), false);
	}
}

void CPlayerComponent::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type)
{

}

void CPlayerComponent::CreatePulse()
{

}

void CPlayerComponent::CreateExplosion()
{
	Snackbar::Get().Log("created explosion");

	gEnv->pPhysicalWorld->SimulateExplosion(_mousePos, Vec3(1, 1, 1), 0.5, 1, 0.75, 1);
}
