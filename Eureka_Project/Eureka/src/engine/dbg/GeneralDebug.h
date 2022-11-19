#pragma once

// Use LOG to print out useful information
#define LOG(message) std::cout << message << std::endl;

// Use VERIFY when the function doesn't work if the condition specified is false
#define VERIFY_STATE(condition) if (!condition) throw invalid_state("ERROR: Unexpected state!")
#define VERIFY_ARG(condition) if (!condition) throw std::invalid_argument("ERROR: Unexpected input!")

// Use WARNING when the function works if the condition specified is false, but the behaviour is probably not intended
#define LOG_WARNING(message) std::cerr << "WARNING: " << message << std::endl