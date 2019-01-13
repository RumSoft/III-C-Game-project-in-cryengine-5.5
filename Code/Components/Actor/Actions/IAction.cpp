#include "StdAfx.h"
#include "IAction.h"
#include "Components/Actor/Actor.h"

//1 - navigation component
//2 - pathfinding component
#define navType 2

#if navType == 2
#define nav GetNavigationComponent()
#define moveTo NavigateTo
#else
#define nav GetPathfindingComponent()
#define moveTo RequestMoveTo
#endif

#define controller GetController()
#define entityPos GetEntity()->GetWorldPos()

void MoveToAction::Process(CActor* actor)
{
	actor->controller->nav->moveTo(_pos);
}

void MoveByAction::Process(CActor* actor)
{
	actor->controller->nav->moveTo(actor->entityPos + _offset);
}


