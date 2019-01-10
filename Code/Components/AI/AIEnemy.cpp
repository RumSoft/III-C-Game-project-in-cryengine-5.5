#include "StdAfx.h"
#include "AIEnemy.h"


void CAIEnemy::Initialize()
{
	CAIComponent::Initialize();

}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<CAIEnemy>)
