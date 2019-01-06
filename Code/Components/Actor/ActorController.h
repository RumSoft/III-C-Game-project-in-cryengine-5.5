#pragma once

#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/AI/PathfindingComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <CryAISystem/Components/IEntityBehaviorTreeComponent.h>
#include <CryAISystem/Components/IEntityFactionComponent.h>
#include <CryAISystem/Components/IEntityCoverUserComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>

using namespace Cry::DefaultComponents;

class CActorController : public IEntityComponent
{
#pragma region base
public:
	CActorController() = default;
	virtual ~CActorController() = default;

	friend CGamePlugin;
	void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CActorController>& desc)
	{
		desc.SetGUID(CActorControllerGUID);
		desc.SetEditorCategory("Actors");
		desc.SetLabel("Actor Controller");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}

public:
	CAdvancedAnimationComponent*	GetAnimationComponent()		{ return m_pAdvancedAnimationComponent; }
	CCharacterControllerComponent*	GetCharacterController()	{ return m_pCharacterControllerComponent; }
	CPathfindingComponent*			GetPathfindingComponent()	{ return m_pPathfindingComponent; }
	IEntityBehaviorTreeComponent*	GetBehaviorTreeComponent()	{ return m_pBehaviorTreeComponent; }
	IEntityNavigationComponent*		GetNavigationComponent()	{ return m_pNavigationComponent; }
	IEntityCoverUserComponent*		GetCoverUserComponent()		{ return m_pCoverUserComponent; }

private:
	CAdvancedAnimationComponent*	m_pAdvancedAnimationComponent	= nullptr;
	CCharacterControllerComponent*	m_pCharacterControllerComponent = nullptr;
	CPathfindingComponent*			m_pPathfindingComponent			= nullptr;
	IEntityBehaviorTreeComponent*	m_pBehaviorTreeComponent		= nullptr;
	IEntityNavigationComponent*		m_pNavigationComponent			= nullptr;
	IEntityCoverUserComponent*		m_pCoverUserComponent			= nullptr;

#pragma endregion

};