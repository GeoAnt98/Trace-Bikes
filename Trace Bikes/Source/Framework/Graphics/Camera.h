#ifndef __GameDev2D__Camera__
#define __GameDev2D__Camera__

#include "GraphicTypes.h"
#include "../Core/Transformable.h"
#include "../Math/Matrix.h"


namespace GameDev2D
{
    class Camera : public Transformable
    {
    public:
        Camera();

        //Used to update the Camera's shake
        void Update(double delta);

        //Getter methods for the projection and view matrices
        Matrix GetProjectionMatrix();
        Matrix GetViewMatrix();

        //Set's the width and height of the camera's view
        void SetViewport(const Viewport& viewport);

        //Returns the width and height of the Camera's view
        Viewport GetViewport();

        //Sets the Z-clipping values
        void SetDepthClip(float near, float far);

        //Returns the Z-clipping values
        float GetNearClip();
        float GetFarClip();

        //Shakes the camera for a magnitude over a certain duration
        void Shake(float magnitude, double duration);

    protected:
        //Resets the projection matrix
        void ResetProjectionMatrix();

        //Conveniance method to randomize a camera shake
        float RandomShake(float magnitude);

        //Member variables
        Matrix m_ProjectionMatrix;
        Viewport m_Viewport;
        float m_ClipNear;
        float m_ClipFar;

        //Shake members
        bool m_ShakeEnabled;
        float m_ShakeMagnitude;
        double m_ShakeDuration;
        double m_ShakeTimer;
        Vector2 m_ShakeOffset;
    };
}

#endif
