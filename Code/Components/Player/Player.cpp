#include "StdAfx.h"
#include "Player.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryEntitySystem/IEntity.h>
#include <CryAction/IActionMapManager.h>
#include "Utils/Logger.h"
#include "Utils/StringConversions.h"
#include <CryInput/IHardwareMouse.h>
#include "Components/Item/Item.h"

CPlayerComponent::~CPlayerComponent()
{

}

void CPlayerComponent::Initialize()
{
	m_pCameraManager = GetEntity()->GetOrCreateComponent<CCameraManager>();
	m_pInputComponent = m_pEntity->GetOrCreateComponent<CInputComponent>();
	m_pCursorComponent = m_pEntity->GetOrCreateComponent<CProjectorLightComponent>();
	m_pActor = GetEntity()->GetOrCreateComponent<CActor>();

	m_pInputComponent->RegisterAction("player", "walkto", [this](const int activationMode, float value)
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

void CPlayerComponent::Update(const float fFrameTime)
{
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
	case ENTITY_EVENT_DONE:
		break;
	case ENTITY_EVENT_UPDATE:
		const auto pCtx = reinterpret_cast<SEntityUpdateContext*>(event.nParam[0]);
		Update(pCtx->fFrameTime);

		break;
	}
}

void CPlayerComponent::Revive()
{

	// Set player transformation, but skip this in the Editor
	if (!gEnv->IsEditor())
	{
		const Vec3 playerScale = Vec3(1.f);
		const Quat playerRotation = IDENTITY;

		// Offset the player in the center of the map
		const float heightOffset = 20.f;
		const float terrainCenter = gEnv->p3DEngine->GetTerrainSize() / 2.f;
		const float height = gEnv->p3DEngine->GetTerrainZ(terrainCenter, terrainCenter);
		const Vec3 playerPosition = Vec3(terrainCenter, terrainCenter, height + heightOffset);

		m_pEntity->SetWorldTM(Matrix34::Create(playerScale, playerRotation, playerPosition));


	}
	GetEntity()->Hide(false);
}

void CPlayerComponent::UpdateMouse(float fFrameTime)
{

	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&_mouseScreen.x, &_mouseScreen.y);

	// Invert mouse Y
	_mouseScreen.y = gEnv->pRenderer->GetHeight() - _mouseScreen.y;

	Vec3 p1(0, 0, 0), p2(0, 0, 0);
	gEnv->pRenderer->UnProjectFromScreen(_mouseScreen.x, _mouseScreen.y, 0, &p1.x, &p1.y, &p1.z);
	gEnv->pRenderer->UnProjectFromScreen(_mouseScreen.x, _mouseScreen.y, 1, &p2.x, &p2.y, &p2.z);

	Vec3 dir = p2 - p1;
	dir.Normalize();

	if (gEnv->pPhysicalWorld->RayWorldIntersection(p1, dir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all,
		rwi_stop_at_pierceable | rwi_colltype_any, &_mouseRaycastHit, 1))
	{
		_mousePos = _mouseRaycastHit.pt;

		gEnv->pAuxGeomRenderer->DrawSphere(_mousePos, 0.1, ColorB(255, 0, 255), false);
		UpdateCursor();

		if(_mouseRaycastHit.pCollider)
		{
			if (const auto hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(_mouseRaycastHit.pCollider)) {
				if(auto item = hitEntity->GetComponent<SItemComponent>())
				{
					IRenderAuxText::DrawLabel(hitEntity->GetWorldPos(), 2, hitEntity->GetName());
				}
			}
		}
	}
}

void CPlayerComponent::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type)
{

}


void CPlayerComponent::UpdateCursor()
{
	const auto offset = GetEntity()->GetWorldPos() - _mousePos;
	const auto dir = GetEntity()->GetWorldRotation();
	m_pCursorComponent->SetTransformMatrix(
		Matrix34(Vec3(1, 1, 1),
			Quat::CreateRotationY(DEG2RAD(90)) ,
			Vec3(0, 0, 1) - offset * dir));
}

