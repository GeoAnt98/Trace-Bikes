#pragma once

//Include statements
#include "Animation/Animator.h"
#include "Animation/Easing.h"
#include "Audio/Audio.h"
#include "Core/Drawable.h"
#include "Core/Transformable.h"
#include "Debug/Log.h"
#include "Debug/Profile.h"
#include "Events/Event.h"
#include "Events/EventDispatcher.h"
#include "Events/EventHandler.h"
#include "Events/FullscreenEvent.h"
#include "Events/GamePadButtonDownEvent.h"
#include "Events/GamePadButtonUpEvent.h"
#include "Events/GamePadConnectedEvent.h"
#include "Events/GamePadDisconnectedEvent.h"
#include "Events/GamePadLeftThumbStickEvent.h"
#include "Events/GamePadLeftTriggerEvent.h"
#include "Events/GamePadRightThumbStickEvent.h"
#include "Events/GamePadRightTriggerEvent.h"
#include "Events/KeyUpEvent.h"
#include "Events/KeyDownEvent.h"
#include "Events/KeyRepeatEvent.h"
#include "Events/MouseButtonDownEvent.h"
#include "Events/MouseButtonUpEvent.h"
#include "Events/MouseMovementEvent.h"
#include "Events/MouseScrollWheelEvent.h"
#include "Events/ResizeEvent.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Camera.h"
#include "Graphics/Color.h"
#include "Graphics/Font.h"
#include "Graphics/GraphicTypes.h"
#include "Graphics/Label.h"
#include "Graphics/Polygon.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Shader.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteAtlas.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexData.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "IO/File.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Rotation.h"
#include "Math/Vector2.h"
#include "Services/Services.h"
#include "Services/DebugUI/DebugUI.h"
#include "Services/Graphics/Graphics.h"
#include "Services/InputManager/InputManager.h"
#include "Services/ResourceManager/ResourceManager.h"
#include "Utils/Png/Png.h"
#include "Utils/Text/Text.h"
#include "Utils/TrueType/TrueType.h"
#include "Windows/Application.h"
#include "Windows/GameLoop.h"
#include "Windows/GameWindow.h"
#include <Windows.h>
#include <functional>

//Application constants
#define TARGET_FPS 60
#define LIMIT_FPS true
#define WINDOW_TITLE "GameDev2D"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_IS_FULLSCREEN false
#define BACKGROUND_CLEAR_COLOR Color::BlackColor()
#define DEBUG_DRAW_FPS 1
#define DEBUG_DRAW_DELTA_TIME 0
#define DEBUG_DRAW_ELAPSED_TIME 0
#define DEBUG_DRAW_ALLOCATED_TEXTURE_MEMORY 0
#define DEBUG_DRAW_SPRITE_RECT 0
#define THROW_EXCEPTION_ON_ERROR 1
#define LOG_TO_FILE 0


namespace GameDev2D
{
    // The Run() function is the entry point into GameDev2D, you must provide four (4) functions to the Run() function
    //
    // 1. The first function, is an initialization function, that has a return type of void and take zero parameters.
    //   The initialization function will only be called once, shortly after the GameWindow is setup.
    //
    // 2. The second function, is an shutdown function, that has a return type of void and take zero parameters. The 
    //   shutdown function will only be called once, at the end of the application's lifecycle to ensure all 
    //   resources all properly deleted/unloaded.
    //   
    // 3. The third function, is an update function, that has a return type of void and take one parameter of type double,
    //   the update function is called once a frame, you should perform ALL game logic in the update function
    //
    // 4. The fourth function, is an draw function, that has a return type of void and takes zero parameters, the draw 
    //   function is called once a frame, you should perform ALL rendering and ONLY rendering in the draw function.
    //   Drawing anything outside of the draw function will not work.
    void Run(std::function<void()> initCallback, std::function<void()> shutdownCallback, std::function<void(double)> updateCallback, std::function<void()> drawCallback);


    // Returns the width of the Window
    unsigned int GetScreenWidth();


    // Returns the height of the Window
    unsigned int GetScreenHeight();


    // Loads a Texture from a file. All Texture files MUST be of type png. If a Texture file doesn't exist a default 'checkboard'
    // Texture will be loaded in its place.
    void LoadTexture(const std::string& filename);


    // Unloads an already loaded Texture. You must make sure to unload any Texture that you load, if you don't you are properly 
    // releasing precious memory back to the operating system
    void UnloadTexture(const std::string& filename);


	// Returns an already loaded texture. If the Texture hasn't been loaded, a default 'checkboard'
    // Texture will be returned in its place
	Texture* GetTexture(const std::string& filename);


	// Loads a Font from a file. Font files can be of type ttf or otf. If a Font file doesn't exist a default 'font'
	// will be loaded in its place.
	void LoadFont(const std::string& filename, const std::string& extension, unsigned int fontSize);


	// Unloads an already loaded Font. You must make sure to unload any Font that you load, if you don't you are properly 
	// releasing precious memory back to the operating system
	void UnloadFont(const std::string& filename, const std::string& extension, unsigned int fontSize);


	// Returns an already loaded Font. If a Font file isn't already loaded a default 'font'
	// will be loaded in its place.
	Font* GetFont(const std::string& filename, const std::string& extension, unsigned int fontSize);


    // Returns the width an already loaded Texture, if the Texture isn't loaded, then zero will be returned
    unsigned int GetTextureWidth(const std::string& filename);


    // Returns the height an already loaded Texture, if the Texture isn't loaded, then zero will be returned
    unsigned int GetTextureHeight(const std::string& filename);


    // Draw a Texture to the screen, must be called inside the Draw function. You must specify position and rotation values.
    void DrawTexture(const std::string& filename, float x, float y, float angle, float alpha = 1.0f);


    // Draw a string of text to the screen using GameDev2D's default Font, must be called inside the Draw function. You 
    // must specify position and color values.
    void DrawString(const std::string& text, float x, float y, const Color& color);


    // Draw a rectange to the screen, must be called inside the Draw function. You must specify position, size, angle, color and fill values.
    void DrawRectangle(float x, float y, float width, float height, float angle, const Color& color, bool isFilled);


    // Draw a circle to the screen, must be called inside the Draw function. You must specify position, radius, color and fill values.
    void DrawCircle(float x, float y, float radius, const Color& color, bool isFilled);


    // Draw a line to the screen, must be called inside the Draw function. You must specify a start and end point values, plus a color value
    void DrawLine(float startX, float startY, float endX, float endY, const Color& color);


	// Registers a callback function to be called whenever the key is pressed (down then up)
	void RegisterKeyPressedCallback(std::function<void(Keyboard::Key)> keyPressedCallback);


	// Registers a callback function to be called whenever the left mouse button is clicked
	void RegisterLeftMouseClickCallback(std::function<void(float, float)> leftMouseClickCallback);


	// Registers a callback function to be called whenever the right mouse button is clicked
	void RegisterRightMouseClickCallback(std::function<void(float, float)> rightMouseClickCallback);


    // Returns wether a keyboard key is Up or not
    bool IsKeyUp(unsigned int key);


    // Returns wether a keyboard key is Down or not
    bool IsKeyDown(unsigned int key);


    // Returns wether the left mouse button is Up or not
    bool IsLeftMouseButtonUp();


    // Returns wether the left mouse button is Down or not
    bool IsLeftMouseButtonDown();


    // Returns wether the right mouse button is Up or not
    bool IsRightMouseButtonUp();


    // Returns wether the right mouse button is Down or not
    bool IsRightMouseButtonDown();


    // Returns the current position of the mouse
    Vector2 GetMousePosition();


    // Returns wether a GamePad is connected, by default it will check Port 1, but you 
    // can specify any of the 4 ports available
    bool IsGamePadConnected(GamePad::Port port = GamePad::Port_1);
    
    
    //  Returns the Left Thumb stick vector for the GamePad connected on Port 1, but you
    // can specify any of the 4 ports available. If the GamePad is NOT connected, then 
    // Vector2::Zero is returned
    Vector2 GetGamePadLeftThumbStick(GamePad::Port port = GamePad::Port_1);


    //  Returns the Right Thumb stick vector for the GamePad connected on Port 1, but you
    // can specify any of the 4 ports available. If the GamePad is NOT connected, then 
    // Vector2::Zero is returned
    Vector2 GetGamePadRightThumbStick(GamePad::Port port = GamePad::Port_1);


    // Returns the Left analog trigger for the GamePad connected on Port 1, but you can
    // specify any of the 4 ports available. If the GamePad is NOT connected, then 
    // zero is returned
    float GetGamePadLeftTrigger(GamePad::Port port = GamePad::Port_1);


    // Returns the Right analog trigger for the GamePad connected on Port 1, but you can
    // specify any of the 4 ports available. If the GamePad is NOT connected, then 
    // zero is returned
    float GetGamePadRightTrigger(GamePad::Port port = GamePad::Port_1);

    // Returns wether a button is pressed or not for the GamePad connected on Port 1, but 
    // you can specify any of the 4 ports available. If the GamePad is NOT connected, then 
    // false is returned
    bool IsGamePadButtonPressed(GamePad::Button button, GamePad::Port port = GamePad::Port_1);
}