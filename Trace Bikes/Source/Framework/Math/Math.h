#ifndef __GameDev2D_Math_h__
#define __GameDev2D_Math_h__


namespace GameDev2D
{
    namespace Math
    {
        float DegreesToRadians(float degrees);
        float RadiansToDegrees(float radians);

        float Clamp(float value, float min, float max);

        float Random();                         //Returns a float value between 0.0 and 1.0
        float Random(float max);                //Returns a float value between 0.0 and a max value
        float Random(float min, float max);     //Returns a float value between a min value and a max value
    }
}

#endif