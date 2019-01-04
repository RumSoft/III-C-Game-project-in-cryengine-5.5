#pragma once

class CActorController : public IEntityComponent
{
#pragma region base
public:
	CActorController() = default;
	virtual ~CActorController() = default;

	friend CGamePlugin;
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
#pragma endregion

};