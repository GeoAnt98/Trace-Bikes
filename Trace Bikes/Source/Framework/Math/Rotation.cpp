#include "Rotation.h"
#include "Math.h"
#include <math.h>


namespace GameDev2D
{
    Rotation::Rotation()
    {
        SetAngle(0.0f);
    }

    Rotation::Rotation(float aAngle)
    {
        SetAngle(aAngle);
    }

    Rotation::Rotation(Vector2 aDirection)
    {
        SetDirection(aDirection);
    }

    void Rotation::SetAngle(float aAngle)
    {
        //Set the angle in degrees
        m_Angle = aAngle;

        //Bounds check the angle
        if (m_Angle < 0.0f)
        {
            m_Angle += 360.0f;
        }
        else if (m_Angle > 360.0f)
        {
            m_Angle -= 360.0f;
        }

        //Convert to radians
        float angleInRadians = Math::DegreesToRadians(m_Angle);

        //Calculate the direction vector
        m_Direction = Vector2(cosf(angleInRadians), sinf(angleInRadians));
    }

    float Rotation::GetAngle()
    {
        return m_Angle;
    }

    void Rotation::SetDirection(Vector2 aDirection)
    {
        //Set the direction
        m_Direction = aDirection;

        //Calculate the angle in radians
        float angleInRadians = atan2f(m_Direction.y, m_Direction.x);

        //Convert to degrees
        m_Angle = Math::RadiansToDegrees(angleInRadians);
    }

    Vector2 Rotation::GetDirection()
    {
        return m_Direction;
    }
}