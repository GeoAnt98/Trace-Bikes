#include "MouseScrollWheelEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    MouseScrollWheelEvent::MouseScrollWheelEvent(float aDelta) : Event(MOUSE_WHEEL_EVENT),
        m_Delta(aDelta)
    {

    }
        
    float MouseScrollWheelEvent::GetDetla()
    {
        return m_Delta;
    }
    
    void MouseScrollWheelEvent::LogEvent()
    {
        Log::Message(this, "MouseScrollWheelEvent", Log::Verbosity_Input_Mouse, "Delta: %f", m_Delta);
    }
}