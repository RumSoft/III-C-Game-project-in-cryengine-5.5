#pragma once
#include "IActor.h"
#include "GamePlugin.h"
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>

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
protected:
	void Update(float fFrameTime) override;

private:
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	FragmentID m_activeFragmentId;
	TagID m_rotateTagId;

};

