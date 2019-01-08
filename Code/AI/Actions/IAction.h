#pragma once
#include "Components/Actor/ActorController.h"
#include "CryAISystem/ICoverSystem.h"

class IActorAction
{
public:
	virtual ~IActorAction() = default;
	
	/**
	 * \brief xd
	 * \param controller ddd
	 */
	virtual void Process(CActorController* controller) = 0;
};

class ActorAction : public IActorAction
{
public:
	ActorAction() = default;
	void Process(CActorController* controller) override { };
};

class MoveToAction : public ActorAction
{
public:
	MoveToAction() = default;
	Vec3 Position = ZERO;
	void Process(CActorController* controller) override
	{
		controller->GetNavigationComponent()->NavigateTo(Position);
	}
};

class MoveByAction : public ActorAction
{
public:
	MoveByAction() = default;
	Vec3 Position = ZERO;
	void Process(CActorController* controller) override
	{
		controller->GetNavigationComponent()->NavigateTo(controller->GetEntity()->GetWorldPos() + Position);
	}
};

#define coverRange 10

class HideInCoverAction : public ActorAction
{
public:
	HideInCoverAction() = default;
	void Process(CActorController* controller) override
	{
		Vec3* locations = new Vec3();
		gEnv->pAISystem->GetCoverSystem()->GetCover(controller->GetEntity()->GetWorldPos(), coverRange, new Vec3(0, 0, 1.84), 1, 10, locations, 1,1);
		Vec3 l1 = locations[0];
		
	}
};