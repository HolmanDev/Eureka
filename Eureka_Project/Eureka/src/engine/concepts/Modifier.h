#pragma once

#include "engine/events/EventSubscriber.h"
#include "Attribute.h"

class GameObject;

class Modifier: public Attribute, public EventSubscriber
{
public:
	Modifier();
	~Modifier();
};