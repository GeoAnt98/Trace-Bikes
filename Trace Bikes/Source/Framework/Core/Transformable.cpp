#include "Transformable.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    Transformable::Transformable() :
        m_Parent(nullptr),
        m_Transform(Matrix::Identity()),
        m_Rotation(),
        m_Position(0.0f, 0.0f),
        m_Scale(1.0f, 1.0f),
        m_IsTransformMatrixDirty(false)
    {

    }

    void Transformable::AttachTo(Transformable* aParent)
    {
        m_Parent = aParent;
    }

    Transformable* Transformable::GetParent()
    {
        return m_Parent;
    }

    Matrix Transformable::GetTransformMatrix()
    {
        if (m_IsTransformMatrixDirty == true)
        {
            Matrix translationMatrix = Matrix::MakeTranslation(m_Position);
            Matrix rotationMatrix = Matrix::MakeRotation(m_Rotation.GetAngle());
            Matrix scaleMatrix = Matrix::MakeScale(m_Scale);

            m_Transform = translationMatrix * rotationMatrix * scaleMatrix;
            m_IsTransformMatrixDirty = false;
        }

        if (m_Parent != nullptr)
        {
            return m_Parent->GetTransformMatrix() * m_Transform;
        }

        return m_Transform;
    }

    Vector2 Transformable::GetPosition()
    {
        return m_Position;
    }

    void Transformable::SetPosition(Vector2 aPosition)
    {
        m_Position = aPosition;
        m_IsTransformMatrixDirty = true;
    }

    void Transformable::Translate(Vector2 aOffset)
    {
        m_Position += aOffset;
        m_IsTransformMatrixDirty = true;
    }

    float Transformable::GetAngle()
    {
        return m_Rotation.GetAngle();
    }

    void Transformable::SetAngle(float aAngle)
    {
        m_Rotation.SetAngle(aAngle);
        m_IsTransformMatrixDirty = true;
    }

    void Transformable::Rotate(float aAngle)
    {
        m_Rotation.SetAngle(m_Rotation.GetAngle() + aAngle);
        m_IsTransformMatrixDirty = true;
    }

    Vector2 Transformable::GetDirection()
    {
        return m_Rotation.GetDirection();
    }

    void Transformable::SetDirection(Vector2 aDirection)
    {
        m_Rotation.SetDirection(aDirection);
        m_IsTransformMatrixDirty = true;
    }

    Vector2 Transformable::GetScale()
    {
        return m_Scale;
    }

    void Transformable::SetScale(Vector2 aScale)
    {
        m_Scale = aScale;
        m_IsTransformMatrixDirty = true;
    }

    void Transformable::Log()
    {
        Log::Message(this, "Transformable", Log::Verbosity_Debug, "Position(%f, %f), Rotation(%f), Scale(%f, %f)", m_Position.x, m_Position.y, m_Rotation.GetAngle(), m_Scale.x, m_Scale.y);
    }
}