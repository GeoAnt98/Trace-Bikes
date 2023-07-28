#include "Services.h"


namespace GameDev2D
{
    
    //Initialize the static variables to NULL
    Application* Services::s_Application = nullptr;
    Graphics* Services::s_Graphics = nullptr;
    ResourceManager* Services::s_ResourceManager = nullptr;
    InputManager* Services::s_InputManager = nullptr;
    DebugUI* Services::s_DebugUI = nullptr;
    
    void Services::Init(Application* aApplication)
    {
        s_Application = aApplication;
        s_Graphics = new Graphics();
        s_ResourceManager = new ResourceManager();
        s_InputManager = new InputManager();
        s_DebugUI = new DebugUI();
    }
    
    void Services::Cleanup()
    {
        if (s_DebugUI != nullptr)
        {
            delete s_DebugUI;
            s_DebugUI = nullptr;
        }

        if (s_InputManager != nullptr)
        {
            delete s_InputManager;
            s_InputManager = nullptr;
        }

        if (s_ResourceManager != nullptr)
        {
            delete s_ResourceManager;
            s_ResourceManager = nullptr;
        }

        if (s_Graphics != nullptr)
        {
            delete s_Graphics;
            s_Graphics = nullptr;
        }
    }

    Application* Services::GetApplication()
    {
        return s_Application;
    }
    
    Graphics* Services::GetGraphics()
    {
        assert(s_Graphics != nullptr);
        return s_Graphics;
    }
    
    InputManager* Services::GetInputManager()
    {
        assert(s_InputManager != nullptr);
        return s_InputManager;
    }

    DebugUI* Services::GetDebugUI()
    {
        assert(s_DebugUI != nullptr);
        return s_DebugUI;
    }
    
    ResourceManager* Services::GetResourceManager()
    {
        assert(s_ResourceManager != nullptr);
        return s_ResourceManager;
    }
}
