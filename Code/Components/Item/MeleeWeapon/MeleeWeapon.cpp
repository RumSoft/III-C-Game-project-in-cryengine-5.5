#include "StdAfx.h"
#include "MeleeWeapon.h"


CMeleeWeaponComponent::CMeleeWeaponComponent()
{
}

void CMeleeWeaponComponent::Initialize()
{
	SItemComponent::Initialize();
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<SItemComponent>)
