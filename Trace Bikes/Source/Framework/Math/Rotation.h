#ifndef __GameDev2D_Rotation_h__
#define __GameDev2D_Rotation_h__

#include "Vector2.h"


namespace GameDev2D
{
    class Rotation
    {
    public:
        Rotation();
        Rotation(float angle);
        Rotation(Vector2 direction);

        void SetAngle(float angle);
        float GetAngle();

        void SetDirection(Vector2 direction);
        Vector2 GetDirection();

    private:
        float m_Angle;
        Vector2 m_Direction;
    };
}

#endif
