#pragma once

#include <GameDev2D.h>
#include "Timer.h"
#include <string.h>

namespace GameDev2D
{
	const int RED_BIKE_START_ROT = 180;
	const int BLUE_BIKE_START_ROT = 0;

	const float BIKE_SPEED = 250.0f;
	const float MIN_SEGMENT_DISTANCE = 8.0f; // Minimum distance between segments place by bikes.
	const float NEW_SEGMENT_IGNORE_COUNT = 10;

	const string RED_SEGMENT = "RedSegment";
	const string BLUE_SEGMENT = "BlueSegment";
	const string RED_BIKE = "RedBike";
	const string BLUE_BIKE = "BlueBike";

	const Vector2 RED_BIKE_START_POS = Vector2(23.0f, 9.0f);
	const Vector2 BLUE_BIKE_START_POS = Vector2(WINDOW_WIDTH - 23.0f, WINDOW_HEIGHT - 9.0f);

	enum class RoundState
	{
		Unknown,
		Starting,
		Running,
		GameOver,
	};

	class Segment;

	class Game
	{
	public:
		Game();
		~Game();

		void Update(double delta);
		void Draw();
		void Reset();

		void StartRound();
		void EndRound();

		void Turn(const Vector2& direction, Segment* bike, Vector2& bikevelocity, const string& segmentcolor, float angle);

		void AddInitialSegments();
		void AddSegment(Segment* bike, const string& segmentcolor);
		void DeleteBikeSegments(Segment* bike);

		void CheckBikesIntersections();

		bool IsBikeAddingSegment(Segment* bike);

		void HandleLeftMouseClick(float mouseX, float mouseY);
		void HandleRightMouseClick(float mouseX, float mouseY);
		void HandleKeyPress(Keyboard::Key key);
	private:
		bool m_RedAlive;
		bool m_BlueAlive;
		bool m_ShouldClearRender;

		RoundState m_RoundState;

		Timer m_timer;

		Segment* m_RedBike;
		Segment* m_BlueBike;

		Vector2 m_RedVelocity;
		Vector2 m_BlueVelocity;

		Sprite* m_RedVisual;
		Sprite* m_BlueVisual;

		Label* m_Notification;

		Texture* m_Canvas;
		Texture* m_CanvasPath;

		RenderTarget* m_RenderTarget;
		RenderTarget* m_RenderPath;
	};
}
