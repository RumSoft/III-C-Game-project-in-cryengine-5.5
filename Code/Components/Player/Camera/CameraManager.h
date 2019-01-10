#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include "GamePlugin.h"

class CCameraManager : public IEntityComponent
{
	enum ECameraMode
	{
		eCameraMode_NoCamera = -1,
		eCameraMode_Topdown,
		eCameraMode_Inventory,
		eCameraMode_Last
	};

#pragma region base
public:
	CCameraManager() = default;
	virtual ~CCameraManager() = default;

	friend CGamePlugin;
	void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CCameraManager>& desc)
	{
		desc.SetGUID(CEnemyGUID);
		desc.SetEditorCategory("Enemy AI");
		desc.SetLabel("Enemy AI Actor");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform });
	}
#pragma endregion
private:

public:
	void SetCameraMode(ECameraMode mode, const char* reason){}
private:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;


};
