#include "pch.h"
#include "Exceptions.h"

invalid_state::invalid_state(char const* const message) throw()
	: std::runtime_error(message)
{
}