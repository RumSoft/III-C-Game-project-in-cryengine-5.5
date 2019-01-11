#pragma once
#include "ICameraMode.h"

class CTopdownCameraMode : public ICameraMode
{
public:
	CTopdownCameraMode() {}
	~CTopdownCameraMode() override {}
	void Update(Matrix34 playerTM) override
	{
		auto position = playerTM.GetTranslation();
		Quat rotation = Quat::CreateRotationXYZ(Ang3(DEG2RAD(-45), 0, DEG2RAD(45)));
		m_cameraMatrix = Matrix34(Vec3(1,1,1), rotation, position + Vec3(-5,-5,10));
	}
	void OnActivate() override {}
	void OnDeactivate() override {}
};