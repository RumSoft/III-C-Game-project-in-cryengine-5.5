#pragma once
#include "ICameraMode.h"

class CInventoryCameraMode : public ICameraMode
{
public:
	CInventoryCameraMode() {};
	~CInventoryCameraMode() {};
	void Update(Matrix34 playerTM)
	{
		auto position = playerTM.GetTranslation();
		m_cameraMatrix = Matrix34(Vec3(1, 1, 1), IDENTITY, position + Vec3(4, 0, 1));
	};
	void OnActivate() {};
	void OnDeactivate() {};
};