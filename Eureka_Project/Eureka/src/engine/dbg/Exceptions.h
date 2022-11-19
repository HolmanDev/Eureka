#pragma once

class invalid_state : public std::runtime_error
{
public:
	invalid_state(char const* const message) throw();
};