#pragma once

#include <CryEntitySystem/IEntitySystem.h>
#include "Components/Item/Item.h"

#define INVENTORY_CAPACITY 3

struct SItemComponent;

class CInventory final : public IEntityComponent
{
public:
	CInventory();
	virtual ~CInventory() = default;

	void Initialize() override;
	static void ReflectType(Schematyc::CTypeDesc<CInventory>& desc);
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	bool AddItem(SItemComponent* pNewItem);
	bool AddItem(int slot, SItemComponent* pNewItem);
	void RemoveItem(int slot);
	void RemoveItem(SItemComponent* pNewItem);
	int GetItemSlot(SItemComponent* pItem);
	SItemComponent* GetItem(int slot);
private:
	SItemComponent* pItems[INVENTORY_CAPACITY] = { nullptr };
};
