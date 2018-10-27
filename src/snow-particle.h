#pragma once

#include <vector>

namespace EternalSnow
{
	struct SnowParticle
	{
		double hvel, vvel; //Velocity.
		double x, y; //Current screen position.
	};

	extern std::vector<SnowParticle> snowvec;
}