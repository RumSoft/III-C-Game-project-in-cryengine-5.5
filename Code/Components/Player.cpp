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
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	// Get the input component, wraps access to action mapping so we can easily get callbacks when inputs are triggered
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	// Register an action, and the callback that will be sent when it's triggered
	m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveLeft, activationMode);  });
	// Bind the 'A' key the "moveleft" action
	m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse, EKeyId::eKI_A);

	m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveRight, activationMode);  });
	m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);

	m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveForward, activationMode);  });
	m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

	m_pInputComponent->RegisterAction("player", "moveback", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveBack, activationMode);  });
	m_pInputComponent->BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

	m_pInputComponent->RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) { m_mouseDeltaRotation.x -= value; });
	m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

	m_pInputComponent->RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) { m_mouseDeltaRotation.y -= value; });
	m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);


	m_pInputComponent->RegisterAction("player", "simulateexplosion", [this](int activationMode, float value) { if(aimingMode)CreateExplosion();  });
	m_pInputComponent->BindAction("player", "simulateexplosion", eAID_KeyboardMouse, EKeyId::eKI_Mouse1);

	m_pInputComponent->RegisterAction("player", "simulatepulse", [this](int activationMode, float value) { if(aimingMode)CreatePulse();  });
	m_pInputComponent->BindAction("player", "simulatepulse", eAID_KeyboardMouse, EKeyId::eKI_Mouse2);

	m_pInputComponent->RegisterAction("player", "toggleeditorattack", [this](int activationMode, float value) { aimingMode = !aimingMode; });
	m_pInputComponent->BindAction("player", "toggleeditorattack", eAID_KeyboardMouse, EKeyId::eKI_Mouse3);

	Revive();
}

uint64 CPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE);
}

void CPlayerComponent::Update(float fFrameTime)
{
	if (aimingMode)
	{
		auto h = gEnv->pRenderer->GetHeight();
		auto w = gEnv->pRenderer->GetWidth();
		Vec3 vPos0(0, 0, 0);
		gEnv->pRenderer->UnProjectFromScreen(w/2.f, h/2.f, 0, &vPos0.x, &vPos0.y, &vPos0.z);

		Vec3 vPos1(0, 0, 0);
		gEnv->pRenderer->UnProjectFromScreen(w/2.f, h/2.f, 1, &vPos1.x, &vPos1.y, &vPos1.z);

		Vec3 vDir = vPos1 - vPos0;
		vDir.Normalize();

		const auto rayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
		ray_hit hit;

		if (gEnv->pPhysicalWorld->RayWorldIntersection(vPos0, vDir * gEnv->p3DEngine->GetMaxViewDistance(), ent_all, rayFlags, &hit, 1))
			aimingPoint = hit.pt;
		gEnv->pAuxGeomRenderer->DrawSphere(aimingPoint, 0.1f, ColorF(1, 0, 0));
	}

	Logger::Get().Log("player position", Vec3ToString(GetEntity()->GetWorldPos()));
	Logger::Get().Log("player orientantion", QuatToString(GetEntity()->GetWorldRotation()));

	gEnv->pAuxGeomRenderer->Draw2dLabel(10, 10, 1.75, ColorF(1, 1, 1), false, Logger::Get().ReadLog());
	gEnv->pAuxGeomRenderer->Draw2dLabel(10, 100,
		1.75, ColorF(1, 1, 1), false, Snackbar::Get().ReadLog());
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_START_GAME:
	{
		// Revive the entity when gameplay starts
		Revive();
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		const float moveSpeed = 20.5f;
		Vec3 velocity = ZERO;

		// Check input to calculate local space velocity
		if (m_inputFlags & (TInputFlags)EInputFlag::MoveLeft)
			velocity.x -= moveSpeed * pCtx->fFrameTime;
		if (m_inputFlags & (TInputFlags)EInputFlag::MoveRight)
			velocity.x += moveSpeed * pCtx->fFrameTime;
		if (m_inputFlags & (TInputFlags)EInputFlag::MoveForward)
			velocity.y += moveSpeed * pCtx->fFrameTime;
		if (m_inputFlags & (TInputFlags)EInputFlag::MoveBack)
			velocity.y -= moveSpeed * pCtx->fFrameTime;

		// Update the player's transformation
		Matrix34 transformation = m_pEntity->GetWorldTM();
		transformation.AddTranslation(transformation.TransformVector(velocity));

		// Update entity rotation based on latest input
		Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(transformation));

		const float rotationSpeed = 0.002f;
		ypr.x += m_mouseDeltaRotation.x * rotationSpeed;
		ypr.y += m_mouseDeltaRotation.y * rotationSpeed;

		// Disable roll
		ypr.z = 0;

		transformation.SetRotation33(CCamera::CreateOrientationYPR(ypr));

		// Reset the mouse delta since we "used" it this frame
		m_mouseDeltaRotation = ZERO;

		// Apply set position and rotation to the entity
		m_pEntity->SetWorldTM(transformation);

		Update(pCtx->fFrameTime);
	}
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

		// Position the player in the center of the map
		const float heightOffset = 20.f;
		float terrainCenter = gEnv->p3DEngine->GetTerrainSize() / 2.f;
		float height = gEnv->p3DEngine->GetTerrainZ(terrainCenter, terrainCenter);
		Vec3 playerPosition = Vec3(terrainCenter, terrainCenter, height + heightOffset);

		m_pEntity->SetWorldTM(Matrix34::Create(playerScale, playerRotation, playerPosition));
	}

	// Unhide the entity in case hidden by the Editor
	GetEntity()->Hide(false);

	// Reset input now that the player respawned
	m_inputFlags = 0;
	m_mouseDeltaRotation = ZERO;
	aimingMode = false;
}

void CPlayerComponent::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type)
{
	switch (type)
	{
	case EInputFlagType::Hold:
	{
		if (activationMode == eIS_Released)
		{
			m_inputFlags &= ~flags;
		}
		else
		{
			m_inputFlags |= flags;
		}
	}
	break;
	case EInputFlagType::Toggle:
	{
		if (activationMode == eIS_Released)
		{
			// Toggle the bit(s)
			m_inputFlags ^= flags;
		}
	}
	break;
	}
}

void CPlayerComponent::CreatePulse()
{
	Snackbar::Get().Log("created pulse");
	pe_action_impulse actionImpulse;
	actionImpulse.impulse = Vec3(0, 0, 1);
	actionImpulse.angImpulse = Vec3(0, 0, 1);
	int isThreadSafe = 0;
	auto ent = gEnv->pPhysicalWorld->CreatePhysicalEntity(pe_type::PE_STATIC);
	ent->Action(&actionImpulse);

}

void CPlayerComponent::CreateExplosion()
{
	Snackbar::Get().Log("created explosion");

	gEnv->pPhysicalWorld->SimulateExplosion(aimingPoint, Vec3(1, 1, 1), 0.5, 1, 0.75, 1);
}
