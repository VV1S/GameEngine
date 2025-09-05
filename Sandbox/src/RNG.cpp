#include "enginepch.h"
#include "RNG.h"


std::mt19937 Util::RNG::s_Engine;
std::uniform_int_distribution<std::mt19937::result_type> Util::RNG::s_Dist;


void Util::RNG::Init() { s_Engine.seed(std::random_device{}()); }
float Util::RNG::Float01() {
	return (float)s_Dist(s_Engine) / (float)std::numeric_limits<uint32_t>::max();
}