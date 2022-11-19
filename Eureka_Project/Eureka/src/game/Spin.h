#pragma once

#include "engine/concepts/Modifier.h"

class Spin : public Modifier
{
public:
	Spin();
	~Spin();

	std::string Identify() override;

	void Start() override;
	void Update() override;
};