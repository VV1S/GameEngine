#pragma once
#include <random>
#include <limits>


namespace Util {
	class RNG {
	public:
		static void Init();
		static float Float01(); // [0,1]
	private:
		static std::mt19937 s_Engine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Dist;
	};
}