#pragma once

class CActor;

class IActorAction
{
public:
	virtual ~IActorAction() = default;
	
	virtual void Process(CActor* actor) = 0;
};

class ActorAction : public IActorAction
{
public:
	ActorAction() = default;
	void Process(CActor* actor) override { };
};

class MoveToAction : public ActorAction
{
	MoveToAction() = default;
	Vec3 _pos = ZERO;
public:
	MoveToAction(const Vec3 pos) : _pos(pos) {}
	void Process(CActor* actor) override;
};

class MoveByAction : public ActorAction
{		
	MoveByAction() = default;
	Vec3 _offset = ZERO;
public:
	MoveByAction(const Vec3 offset) : _offset(offset) {}
	void Process(CActor* actor) override;
};
