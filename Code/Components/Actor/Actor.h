#pragma once
#include "IActor.h"
#include "GamePlugin.h"
#include "ActorController.h"
#include "Components/Inventory/Inventory.h"
#include "Components/Attributes/Attributes.h"

class CActor : public IActor
{
#pragma region base
public:
	// Provide a virtual destructor, ensuring correct destruction of IEntityComponent members
	virtual ~CActor();
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


private:
	void Update(float fFrameTime) override;
	void Revive() override;

public:

	CActorController*	GetController() { return m_pActorController; }
	CInventory*			GetInventory()	{ return m_pInventory; }
	IEntityFactionComponent* GetFactionComponent() { return m_pFactionComponent; }

	float GetHealth() { return _healthAttribute->GetValue(); }
	void DamageActor(const float value) { _healthAttribute->ChangeValue(-value, true); }
private:
	CActorController*	m_pActorController = nullptr;
	CInventory*			m_pInventory = nullptr; //equipment: weapon,armour,skills? and consumables
	IEntityFactionComponent* m_pFactionComponent = nullptr;
	float slowupdate = 0;

	CAttribute* _healthAttribute = nullptr;
};

