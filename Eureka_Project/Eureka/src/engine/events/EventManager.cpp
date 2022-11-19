#include "pch.h"
#include "EventManager.h"

std::list<std::weak_ptr<EventSubscriber>> EventManager::m_Subscribers;

void EventManager::Start()
{
	for (auto& subscriber : m_Subscribers) {
		if(auto sharedSubscriber = subscriber.lock()) 
		{
			sharedSubscriber->Start();
		}		
	}
}

void EventManager::Update()
{
	for (auto& subscriber : m_Subscribers) {
		if (auto sharedSubscriber = subscriber.lock())
		{
			sharedSubscriber->Update();
		}
	}
}

void EventManager::Close()
{
	m_Subscribers.clear();
}

void EventManager::AddSubscriber(std::shared_ptr<EventSubscriber> subscriber)
{
	m_Subscribers.push_back(subscriber);
}

void EventManager::RemoveSubscriber(std::shared_ptr<EventSubscriber> subscriber)
{
	// Credit to user mkaes on stackoverflow
	m_Subscribers.remove_if([subscriber](std::weak_ptr<EventSubscriber> weakActiveSubscriber) { 
		std::shared_ptr<EventSubscriber> activeSubscriber = weakActiveSubscriber.lock();
		if (subscriber && activeSubscriber)
			return subscriber == activeSubscriber;
		return false;
	});
}
