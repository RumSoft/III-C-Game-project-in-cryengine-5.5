#pragma once
#include <CryMath/Cry_Math.h>
#include <CryRenderer/Tarray.h>
#include <CrySystem/VR/IHMDDevice.h>
#include <CrySystem/VR/IHMDManager.h>
#include "DefaultComponents/Audio/ListenerComponent.h"

class ICameraMode
{
public:
	virtual ~ICameraMode() = default;
	virtual void UpdateView() = 0;
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;

	CCamera& GetCamera() { return m_camera; }

protected:
	bool IsZoomEnabled = true;
	float m_zoomDistance = 10;
	float m_fov = 65;
	Matrix34 m_cameraMatrix{ ZERO, IDENTITY };
	CCamera m_camera;
};