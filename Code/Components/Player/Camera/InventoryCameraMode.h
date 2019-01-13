#pragma once
#include "ICameraMode.h"

class CInventoryCameraMode : public ICameraMode
{
public:
	CInventoryCameraMode()
	{
		IsZoomEnabled = false;
	};
	~CInventoryCameraMode() override {};
	void Update(Matrix34 playerTM) override
	{
		auto position = playerTM.GetTranslation();
		auto rot = Quat(playerTM);
		auto offset = Vec3(0, -5, 0);
		m_cameraMatrix = Matrix34(Vec3(1, 1, 1), rot, position + offset*rot);
	};
	void OnActivate() override
	{
		gEnv->pConsole->ExecuteString("t_scale 0.1");
	};
	void OnDeactivate() override
	{
		gEnv->pConsole->ExecuteString("t_scale 1");
	};

private:

};