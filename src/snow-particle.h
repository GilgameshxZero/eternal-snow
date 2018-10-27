#pragma once

#include <vector>

namespace EternalSnow {
struct SnowParticle {
    double horiVel, vertVel;  //Velocity.
    double x, y;        //Current screen position.
};

extern std::vector<SnowParticle> snowParts;
}  // namespace EternalSnow