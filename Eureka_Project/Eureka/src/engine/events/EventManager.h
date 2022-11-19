#pragma once

#include "engine/glCore.h"

#include "EventSubscriber.h"
#include "engine/world/Camera.h"

class EventManager
{
private:
	static std::list<std::weak_ptr<EventSubscriber>> m_Subscribers;
public:
	static void Start();
	static void Update();
	static void Close();

	static void AddSubscriber(std::shared_ptr<EventSubscriber> subscriber);
	static void RemoveSubscriber(std::shared_ptr<EventSubscriber> subscriber);
};