#pragma once

class EventSubscriber
{
public:
	virtual ~EventSubscriber();
	virtual void Start();
	virtual void Update();
};