#pragma once
#include "GamePlugin.h"
#include <CryEntitySystem/IEntitySystem.h>
#include "../Item.h"

struct CMeleeWeaponComponent final : SItemComponent
{
public:
	CMeleeWeaponComponent();
	virtual ~CMeleeWeaponComponent() = default;

	void Initialize() override;
	static void ReflectType(Schematyc::CTypeDesc<CMeleeWeaponComponent>& desc)
	{
		desc.SetGUID(CMeleeWeaponGUID);
		desc.SetEditorCategory("Items");
		desc.SetLabel("MeleeWeaponComponent");
		desc.AddBase<SItemComponent>();
		desc.AddMember(&CMeleeWeaponComponent::sItemProperties, 'itep', "ItemProperties", "Item settings", "All properties of this item.", SItemProperties());
	}

	virtual void ProcessEvent(const SEntityEvent& event) override
	{
		SItemComponent::ProcessEvent(event);
	}
};
