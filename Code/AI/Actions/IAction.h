#pragma once
#include "Components/Actor/ActorController.h"

class IAction
{
public:
	virtual ~IAction() = default;
	virtual void Process(CActorController* controller) = 0;
};

class MoveToAction : IAction
{
public:
	void Process(CActorController* controller) override
	{
		
	}
};

class MoveBy : IAction
{
public:
	void Process(CActorController* controller) override
	{
		
	}
};