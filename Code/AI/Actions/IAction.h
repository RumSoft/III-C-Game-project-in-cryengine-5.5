#pragma once
#include "Components/Actor/ActorController.h"

class IActorAction
{
public:
	virtual ~IActorAction() = default;
	virtual void Process(CActorController* controller) = 0;
};

class MoveToAction : IActorAction
{
public:
	void Process(CActorController* controller) override
	{
		
	}
};

class MoveBy : IActorAction
{
public:
	void Process(CActorController* controller) override
	{
		
	}
};