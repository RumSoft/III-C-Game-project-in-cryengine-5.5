#pragma once
#include "GamePlugin.h"

struct IActor : IEntityComponent
{
#pragma region base
public:
	IActor() = default;
	virtual ~IActor() = default;

protected:
	friend CGamePlugin;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<IActor>& desc)
	{
		desc.SetGUID(IActorGUID);
		desc.SetEditorCategory("Actors");
		desc.SetLabel("Actor");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}
#pragma endregion
};
