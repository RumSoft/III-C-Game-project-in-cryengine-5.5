#pragma once
#include "IActor.h"
#include "GamePlugin.h"

class CActor : public IActor
{
#pragma region base
public:
	// Provide a virtual destructor, ensuring correct destruction of IEntityComponent members
	virtual ~CActor() = default;
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
};

