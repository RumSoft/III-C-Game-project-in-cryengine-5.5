#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include <DefaultComponents/Cameras/CameraComponent.h>
#include "GamePlugin.h"
#include "ICameraMode.h"


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
		desc.SetGUID(CCameraManagerGUID);
		desc.SetEditorCategory("Actor");
		desc.SetLabel("--Camera Manager");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ EFlags::Transform, EFlags::Singleton});
	}
#pragma endregion
private:

public:
	void SetCameraMode(ECameraMode mode, const char* reason){}
private:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

	ICameraMode* m_cameraModes[eCameraMode_Last];
	ECameraMode m_cameraMode = eCameraMode_NoCamera;

	float m_zoomDelta{ 0.0f };
};
