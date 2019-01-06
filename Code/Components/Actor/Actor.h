#pragma once
#include "IActor.h"
#include "GamePlugin.h"
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/AI/PathfindingComponent.h>
#include <CryAISystem/Components/IEntityNavigationComponent.h>
#include <CryAISystem/Components/IEntityCoverUserComponent.h>
#include <CryAISystem/Components/IEntityBehaviorTreeComponent.h>
#include <CryAISystem/Components/IEntityObserverComponent.h>
#include <CryAISystem/Components/IEntityFactionComponent.h>
//#include <DefaultComponents/Effects/ParticleComponent.h>
#include "State/IActorState.h"
#include <DefaultComponents/Input/InputComponent.h>
#include <CryAISystem/ICoverSystem.h>

//class IEntityObserverComponent;

class CActor : public IActor
{
#pragma region base
public:
	// Provide a virtual destructor, ensuring correct destruction of IEntityComponent members
	virtual ~CActor() = default;
	void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CActor>& desc)
	{
		desc.SetGUID(CActorGUID);
		desc.SetEditorCategory("Actors");
		desc.SetLabel("Actor");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}
#pragma endregion base
	void OnKill() override {};
	void OnSpawn() override {};

	void Revive() override;

	void AttackActor(CActor* enemy)
	{
		m_pPathfindingComponent->RequestMoveTo(enemy->GetEntity()->GetWorldPos());
	}
	void MoveTo(Vec3 pos)
	{
		Logger::Get().Log(GetEntity()->GetName(), "moves somewhere");
	}
	void AttackMelee(CActor* enemy) {}
	void Hide() {  }
	bool CanSeeEnemy()
	{
		
		return false;
	}
	int DealDamage(int damage) { Health -= damage; if (Health <= 0) Kill(); }


	void Kill() { actorState = EActorState::Killed;}

	enum EActorState
	{
		NO_STATE, Idle, Alarmed,Attacking, Fleeing, Killed, END
	};

	EActorState actorState;

	std::vector<CActor*> enemyQueue;

	int Faction;

	int Health;
	int MaxHealth;

protected:
	void UpdateAI(float fFrameTime);
	void Update(float fFrameTime) override;
	IActorState* GetState() { return m_pActorState; }
private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;
	Cry::DefaultComponents::CPathfindingComponent* m_pPathfindingComponent = nullptr;
	IEntityCoverUserComponent* m_pCoverUserComponent = nullptr;
	IEntityNavigationComponent* m_pNavigationComponent = nullptr;
	IEntityBehaviorTreeComponent* m_pBehaviorTreeComponent = nullptr;
	IEntityFactionComponent* m_pFactionComponent = nullptr;
	IActorState* m_pActorState = nullptr;
	
	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	FragmentID m_activeFragmentId;
	TagID m_rotateTagId;
	float m_movementSpeed = 10;

	float m_aiupdatetime = 1;
};

