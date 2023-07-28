#include "GameDev2D.h"


namespace GameDev2D
{
    void Run(std::function<void()> aInitCallback, std::function<void()> aShutdownCallback, std::function<void(double)> aUpdateCallback, std::function<void()> aDrawCallback)
    {
        //Create the Window's PlatformLayer and initialize it and set the Root Scene
        Application application = Application(WINDOW_TITLE, TARGET_FPS, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_IS_FULLSCREEN);

        //Initialize the application with the callback functions
        application.Init(aInitCallback, aShutdownCallback, aUpdateCallback, aDrawCallback);

        //Message handling
        MSG message;
        do
        {
            if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        } while (application.Loop() == true);
    }

    unsigned int GetScreenWidth()
    {
        return Services::GetApplication()->GetWindow()->GetWidth();
    }

    unsigned int GetScreenHeight()
    {
        return Services::GetApplication()->GetWindow()->GetHeight();
    }

    void LoadTexture(const std::string& aFilename)
    {
        Services::GetResourceManager()->LoadTexture(aFilename);
    }

    void UnloadTexture(const std::string& aFilename)
    {
        Services::GetResourceManager()->UnloadTexture(aFilename);
    }

	Texture* GetTexture(const std::string& aFilename)
	{
		return Services::GetResourceManager()->GetTexture(aFilename);
	}

	void LoadFont(const std::string& aFilename, const std::string& aExtension, unsigned int aFontSize)
	{
		Services::GetResourceManager()->LoadFont(aFilename, aExtension, aFontSize);
	}

	void UnloadFont(const std::string& aFilename, const std::string& aExtension, unsigned int aFontSize)
	{
		Services::GetResourceManager()->UnloadFont(aFilename, aExtension, aFontSize);
	}

	Font* GetFont(const std::string& aFilename, const std::string& aExtension, unsigned int aFontSize)
	{
		return Services::GetResourceManager()->GetFont(aFilename, aExtension, aFontSize);
	}

    unsigned int GetTextureWidth(const std::string& aFilename)
    {
        if (Services::GetResourceManager()->IsTextureLoaded(aFilename) == true)
        {
            Texture* texture = Services::GetResourceManager()->GetTexture(aFilename);
            return texture->GetWidth();
        }
        return 0;
    }

    unsigned int GetTextureHeight(const std::string& aFilename)
    {
        if (Services::GetResourceManager()->IsTextureLoaded(aFilename) == true)
        {
            Texture* texture = Services::GetResourceManager()->GetTexture(aFilename);
            return texture->GetHeight();
        }
        return 0;
    }

	void DrawTexture(const std::string& aFilename, float aX, float aY, float aAngle, float aAlpha)
	{
		Texture* texture = Services::GetResourceManager()->GetTexture(aFilename);
		Services::GetGraphics()->DrawTexture(texture, Vector2(aX, aY), aAngle, aAlpha);
	}

    void DrawString(const std::string& aText, float aX, float aY, const Color& aColor)
    {
        Font* font = Services::GetResourceManager()->GetDefaultFont();
        Services::GetGraphics()->DrawFont(font, aText, Vector2(aX, aY), aColor);
    }

    void DrawRectangle(float aX, float aY, float aWidth, float aHeight, float aAngle, const Color& aColor, bool aIsFilled)
    {
        Services::GetGraphics()->DrawRectangle(Vector2(aX, aY), Vector2(aWidth, aHeight), aAngle, Vector2(0.0f, 0.0f), aColor, aIsFilled);
    }

    void DrawCircle(float aX, float aY, float aRadius, const Color& aColor, bool aIsFilled)
    {
        Services::GetGraphics()->DrawCircle(Vector2(aX, aY), aRadius, Vector2(0.0f, 0.0f), aColor, aIsFilled);
    }

    void DrawLine(float aStartX, float aStartY, float aEndX, float aEndY, const Color& aColor)
    {
        Services::GetGraphics()->DrawLine(Vector2(aStartX, aStartY), Vector2(aEndX, aEndY), aColor);
    }

	void RegisterKeyPressedCallback(std::function<void(Keyboard::Key)> aKeyPressedCallback)
	{
		Services::GetInputManager()->RegisterKeyPressedCallback(aKeyPressedCallback);
	}

	void RegisterLeftMouseClickCallback(std::function<void(float, float)> aLeftMouseClickCallback)
	{
		Services::GetInputManager()->RegisterLeftMouseClickCallback(aLeftMouseClickCallback);
	}

	void RegisterRightMouseClickCallback(std::function<void(float, float)> aRightMouseClickCallback)
	{
		Services::GetInputManager()->RegisterRightMouseClickCallback(aRightMouseClickCallback);
	}

    bool IsKeyUp(unsigned int aKey)
    {
        return Services::GetInputManager()->GetKeyboard()->GetState(static_cast<Keyboard::Key>(aKey)).keyState == Keyboard::KeyUp;
    }

    bool IsKeyDown(unsigned int aKey)
    {
        Keyboard::State keyboardState = Services::GetInputManager()->GetKeyboard()->GetState(static_cast<Keyboard::Key>(aKey));
        return keyboardState.keyState == Keyboard::KeyDown || keyboardState.keyState == Keyboard::KeyRepeat;
    }

    bool IsLeftMouseButtonUp()
    {
        return Services::GetInputManager()->GetMouse()->IsButtonPressed(Mouse::Left) == false;
    }

    bool IsLeftMouseButtonDown()
    {
        return Services::GetInputManager()->GetMouse()->IsButtonPressed(Mouse::Left) == true;
    }

    bool IsRightMouseButtonUp()
    {
        return Services::GetInputManager()->GetMouse()->IsButtonPressed(Mouse::Right) == false;
    }

    bool IsRightMouseButtonDown()
    {
        return Services::GetInputManager()->GetMouse()->IsButtonPressed(Mouse::Right) == true;
    }

    Vector2 GetMousePosition()
    {
        return Services::GetInputManager()->GetMouse()->GetPosition();
    }

	bool IsGamePadConnected(GamePad::Port aPort)
	{
		return Services::GetInputManager()->GetGamePad(aPort)->IsConnected();
	}

    Vector2 GetGamePadLeftThumbStick(GamePad::Port aPort)
    {
        if (Services::GetInputManager()->GetGamePad(aPort)->IsConnected() == true)
        {
            return Services::GetInputManager()->GetGamePad(aPort)->GetLeftThumbStick();
        }
        return Vector2::Zero;
    }

    Vector2 GetGamePadRightThumbStick(GamePad::Port aPort)
    {
        if (Services::GetInputManager()->GetGamePad(aPort)->IsConnected() == true)
        {
            return Services::GetInputManager()->GetGamePad(aPort)->GetRightThumbStick();
        }
        return Vector2::Zero;
    }

    float GetGamePadLeftTrigger(GamePad::Port aPort)
    {
        if (Services::GetInputManager()->GetGamePad(aPort)->IsConnected() == true)
        {
            return Services::GetInputManager()->GetGamePad(aPort)->GetLeftTrigger();
        }
        return 0.0f;
    }

    float GetGamePadRightTrigger(GamePad::Port aPort)
    {
        if (Services::GetInputManager()->GetGamePad(aPort)->IsConnected() == true)
        {
            return Services::GetInputManager()->GetGamePad(aPort)->GetRightTrigger();
        }
        return 0.0f;
    }

    bool IsGamePadButtonPressed(GamePad::Button aButton, GamePad::Port aPort)
    {
        if (Services::GetInputManager()->GetGamePad(aPort)->IsConnected() == true)
        {
            return Services::GetInputManager()->GetGamePad(aPort)->IsButtonPressed(aButton);
        }
        return false;
    }
}