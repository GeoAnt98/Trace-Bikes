#include "Math.h"
#include <stdlib.h>


namespace GameDev2D
{
    namespace Math
    {
        float DegreesToRadians(float aDegrees)
        {
            return aDegrees * 0.01745329251f;
        }

        float RadiansToDegrees(float aRadians)
        {
            return aRadians * 57.2957795131f;
        }

        float Clamp(float aValue, float aMin, float aMax)
        {
            if (aValue < aMin)
            {
                return aMin;
            }

            if (aValue > aMax)
            {
                return aMax;
            }

            return aValue;
        }

        float Random()
        {
            return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }

        float Random(float aMax)
        {
            return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / aMax));
        }

        float Random(float aMin, float aMax)
        {
            return aMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (aMax - aMin)));
        }
    }
}