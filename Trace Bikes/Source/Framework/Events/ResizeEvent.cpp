#include "ResizeEvent.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    ResizeEvent::ResizeEvent(Vector2 aSize) : Event(RESIZE_EVENT),
        m_Size(aSize)
    {
    }

    Vector2 ResizeEvent::GetSize()
    {
        return m_Size;
    }
    
    void ResizeEvent::LogEvent()
    {
        Log::Message(this, "ResizeEvent", Log::Verbosity_Application, "Size: (%f, %f)", m_Size.x, m_Size.y);
    }
}