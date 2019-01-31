#include "StdAfx.h"
#include "Item.h"

void SItemComponent::Initialize()
{
	LoadGeometry();
	SetMaterial();
	Physicalize();
}

uint64 SItemComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED);
}

void SItemComponent::ProcessEvent(const SEntityEvent& event)
{
	if (event.event == ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED)
	{
		if (sItemProperties != sPrevItemProperties)
		{
			sPrevItemProperties = sItemProperties;
			LoadGeometry();
			SetMaterial();
			Physicalize();
		}
	}
}

void SItemComponent::ReflectType(Schematyc::CTypeDesc<SItemComponent>& desc)
{
	desc.SetGUID(SItemGuid);
	desc.SetEditorCategory("Items");
	desc.SetLabel("ItemComponent");
	desc.SetDescription("Base item component");
	desc.AddMember(&SItemComponent::sItemProperties, 'itep', "ItemProperties", "Item settings", "All properties of this item.", SItemProperties());
}

bool SItemComponent::IsPickable()
{
	return !m_pOwnerEntity;
}

void SItemComponent::PickUp(IEntity* pNewOwner)
{
	if (!pNewOwner)
		return;
	m_pOwnerEntity = pNewOwner;
	m_pOwnerEntity->AttachChild(GetEntity());
}

void SItemComponent::Drop()
{

}

void SItemComponent::LoadGeometry()
{
	string sGeomPath = GetProperties()->sRenderProperties.sGeomPath.value.c_str();
	if (sGeomPath.empty())
	{
		m_pEntity->LoadGeometry(GEOMETRY_SLOT, "");
		return;
	}
	m_pEntity->LoadGeometry(GEOMETRY_SLOT, sGeomPath);
}

void SItemComponent::Physicalize()
{

	SEntityPhysicalizeParams physParams;
	physParams.mass = GetProperties()->sPhysicsProperties.fMass;
	physParams.type = PE_RIGID;
	GetEntity()->Physicalize(physParams);
}

void SItemComponent::SetMaterial()
{
	const string sMaterialPath = GetProperties()->sRenderProperties.sMaterial.value.c_str();
	const auto material = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial(sMaterialPath);
	m_pEntity->SetMaterial(material);
}


CRY_STATIC_AUTO_REGISTER_FUNCTION(&registerComponent<SItemComponent>)
