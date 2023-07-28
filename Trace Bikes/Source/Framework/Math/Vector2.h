#ifndef __GameDev2D_Vector2_h__
#define __GameDev2D_Vector2_h__

namespace GameDev2D
{
    struct Vector2
    {
        Vector2(float x, float y);
        Vector2(const Vector2& vector2);
        Vector2();

        //2D zero vector constant (0,0)
        static const Vector2 Zero;

        //2D unit vector constant (1,1)
        static const Vector2 Unit;

        //Calculates the angle of the vector
        float Angle();

        //Returns the Length (magnitude) of the vector
        float Length();

        //Returns the Length squared of the vector
        float LengthSquared();

        //Normalizes the current vector
        void Normalize();

        //Returns a normalized copy of the current vector
        Vector2 Normalized();

        //Calculates the distance between the current vector and the supplied vector
        float Distance(const Vector2& vector2);

        //Calculates the distance squared between the current vector and the supplied vector
        float DistanceSquared(const Vector2& vector2);

        //Calculates the dot product between the current vector and the supplied vector
        float DotProduct(const Vector2& vector2);

        Vector2 operator+(const Vector2& vector2) const;
        void operator+=(const Vector2& vector2);

        Vector2 operator-(const Vector2& vector2) const;
        void operator-=(const Vector2& vector2);

        Vector2 operator*(const Vector2& vector2) const;
        void operator*=(const Vector2& vector2);

        Vector2 operator*(const float& scale) const;
        void operator*=(const float& scale);

        Vector2 operator/(const Vector2& vector2) const;
        void operator/=(const Vector2& vector2);

        Vector2 operator/(const float& scale) const;
        void operator/=(const float& scale);

        Vector2 operator-() const;

        bool operator==(const Vector2& vector2) const;
        bool operator!=(const Vector2& vector2) const;

        bool operator<(const Vector2& vector2) const;
        bool operator<=(const Vector2& vector2) const;

        bool operator>(const Vector2& vector2) const;
        bool operator>=(const Vector2& vector2) const;

        //Member variables
        union
        {
            struct 
            {
                float x;
                float y;
            };
            float data[2];
        };
    };
}

#endif