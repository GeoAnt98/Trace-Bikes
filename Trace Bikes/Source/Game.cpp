#include "Game.h"
#include "Segment.h"

#include <GameDev2D.h>
#include <string.h>

namespace GameDev2D
{
	Game::Game() :
		m_timer(3.0f),
		m_ShouldClearRender(false),
		m_RedAlive(false),
		m_BlueAlive(false),
		m_RoundState(RoundState::Starting)
	{
		// Load resources.
		LoadTexture(RED_SEGMENT);
		LoadTexture(BLUE_SEGMENT);
		LoadTexture(RED_BIKE);
		LoadTexture(BLUE_BIKE);
		LoadFont("Harting_plain", "ttf", 72);

		// Create the bikes.
		m_RedBike = new Segment(RED_BIKE_START_POS, RED_SEGMENT);
		m_RedBike->SetAngle(RED_BIKE_START_ROT);
		m_RedVisual = new Sprite(RED_BIKE);
		m_RedVisual->SetAnchor(Vector2(0.5f, 0.5f));
		m_RedVelocity = Vector2::Zero;

		m_BlueBike = new Segment(BLUE_BIKE_START_POS, BLUE_SEGMENT);
		m_BlueBike->SetAngle(BLUE_BIKE_START_ROT);
		m_BlueVisual = new Sprite(BLUE_BIKE);
		m_BlueVisual->SetAnchor(Vector2(0.5f, 0.5f));
		m_BlueVelocity = Vector2::Zero;

		// Notification label setup.
		m_Notification = new Label(GetFont("Harting_plain", "ttf", 72));
		m_Notification->SetAnchor(Vector2(0.5f, 0.5f));
		m_Notification->SetPosition(Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2));

		// Create the texture canvas.
		PixelFormat PixelFormat(PixelFormat::RGBA, PixelFormat::UnsignedByte);
		ImageData ImageData(PixelFormat, GetScreenWidth(), GetScreenHeight());
		m_Canvas = new Texture(ImageData);
		m_RenderTarget = new RenderTarget(m_Canvas);

		AddInitialSegments();
		m_timer.Start();
	}

	Game::~Game()
	{
		DeleteBikeSegments(m_RedBike);
		DeleteBikeSegments(m_BlueBike);

		if (m_BlueBike != nullptr)
		{
			delete m_BlueBike;
			m_BlueBike = nullptr;
		}

		if (m_RedVisual != nullptr)
		{
			delete m_RedVisual;
			m_RedVisual = nullptr;
		}

		if (m_BlueVisual != nullptr)
		{
			delete m_BlueVisual;
			m_BlueVisual = nullptr;
		}

		if (m_Notification != nullptr)
		{
			delete m_Notification;
			m_Notification = nullptr;
		}

		if (m_Canvas != nullptr)
		{
			delete m_Canvas;
			m_Canvas = nullptr;
		}

		if (m_RenderTarget != nullptr)
		{
			delete m_RenderTarget;
			m_RenderTarget = nullptr;
		}

		// Unload textures.
		UnloadTexture(RED_SEGMENT);
		UnloadTexture(BLUE_SEGMENT);
		UnloadTexture(RED_BIKE);
		UnloadTexture(BLUE_BIKE);
		UnloadFont("Harting_plain", "ttf", 72);
	}

	void Game::Update(double delta)
	{
		if (m_timer.IsRunning())
		{
			m_timer.Update(delta);
			m_Notification->SetText(to_string((int)(4 - m_timer.GetPercentage() * 3)));

			if (m_RoundState == RoundState::Starting && m_timer.GetPercentage() >= 1)
				StartRound();
		}

		m_RedVisual->SetPosition(m_RedBike->GetPosition());
		m_BlueVisual->SetPosition(m_BlueBike->GetPosition());
		m_RedVisual->SetAngle(m_RedBike->GetAngle());
		m_BlueVisual->SetAngle(m_BlueBike->GetAngle());

		if (m_RoundState != RoundState::Running) return;

		// Move the bikes.
		Vector2 redposition = m_RedBike->GetPosition() + Vector2(m_RedVelocity.x * delta, m_RedVelocity.y * delta);
		Vector2 blueposition = m_BlueBike->GetPosition() + Vector2(m_BlueVelocity.x * delta, m_BlueVelocity.y * delta);

		m_RedBike->SetPosition(redposition);
		m_BlueBike->SetPosition(blueposition);

		// Get the next segment, after the head segment
		Segment* next = m_RedBike->GetNext();

		//If the difference is big enough add another segment
		if (IsBikeAddingSegment(m_RedBike))
			AddSegment(m_RedBike, RED_SEGMENT);

		if (IsBikeAddingSegment(m_BlueBike))
			AddSegment(m_BlueBike, BLUE_SEGMENT);

		CheckBikesIntersections();

		if (!m_RedAlive || !m_BlueAlive)
			EndRound();
	}

	void Game::Draw()
	{
		m_RenderTarget->Begin(m_ShouldClearRender);
		m_RedBike->GetNext()->Draw();
		m_BlueBike->GetNext()->Draw();
		m_RenderTarget->End();

		if (m_ShouldClearRender)
			m_ShouldClearRender = false;

		Services::GetGraphics()->DrawTexture(m_Canvas, Vector2(0.0f, 0.0f), 0.0f, 1.0f);

		m_RedBike->Draw();
		m_BlueBike->Draw();
		m_RedVisual->Draw();
		m_BlueVisual->Draw();

		if (m_RoundState == RoundState::GameOver || m_RoundState == RoundState::Starting)
			m_Notification->Draw();
	}

	void Game::Reset()
	{
		m_RoundState = RoundState::Starting;

		m_ShouldClearRender = true;
		m_RedAlive = false;
		m_BlueAlive = false;

		m_RedBike->SetPosition(RED_BIKE_START_POS);
		m_RedBike->SetAngle(RED_BIKE_START_ROT);
		m_RedVelocity = Vector2::Zero;

		m_BlueBike->SetPosition(BLUE_BIKE_START_POS);
		m_BlueBike->SetAngle(BLUE_BIKE_START_ROT);
		m_BlueVelocity = Vector2::Zero;

		m_Notification->SetColor(Color::WhiteColor());

		DeleteBikeSegments(m_RedBike);
		DeleteBikeSegments(m_BlueBike);
		AddInitialSegments();

		m_timer.Reset();
		m_timer.Start();
	}

	void Game::StartRound()
	{
		m_RoundState = RoundState::Running;

		m_RedAlive = true;
		m_BlueAlive = true;

		m_timer.Stop();

		Turn(Vector2(1.0f, 0.0f), m_RedBike, m_RedVelocity, RED_SEGMENT, 0);
		Turn(Vector2(-1.0f, 0.0f), m_BlueBike, m_BlueVelocity, BLUE_SEGMENT, 0);
	}

	void Game::EndRound()
	{
		m_RoundState = RoundState::GameOver;

		if (!m_RedAlive && !m_BlueAlive)
		{
			m_Notification->SetColor(Color::WhiteColor());
			m_Notification->SetText("Tie!");
		}
		else if (!m_RedAlive)
		{
			m_Notification->SetColor(Color::CyanColor());
			m_Notification->SetText("Blue Bike Wins!");
		}
		else if (!m_BlueAlive)
		{
			m_Notification->SetColor(Color::RedColor());
			m_Notification->SetText("Red Bike Wins");
		}
	}

	void Game::Turn(const Vector2& direction, Segment* bike, Vector2& bikevelocity, const string& segmentcolor, float angle)
	{
		if (bikevelocity.DotProduct(direction) == 0.0f)
		{
			// Set the velocity
			bikevelocity = direction * BIKE_SPEED;
			bike->SetAngle(angle);

			AddSegment(bike, segmentcolor);
		}
	}

	void Game::AddInitialSegments()
	{
		AddSegment(m_RedBike, RED_SEGMENT);
		AddSegment(m_BlueBike, BLUE_SEGMENT);
	}

	void Game::AddSegment(Segment* bike, const string& segmentcolor)
	{
		bike->LinkNext(new Segment(bike->GetPosition(), segmentcolor));
	}

	void Game::DeleteBikeSegments(Segment* bike)
	{
		Segment* segment = bike->GetNext();

		while (segment != nullptr)
		{
			Segment* next = segment->GetNext();
			delete segment;
			segment = next;
		}

		bike->SetNext(nullptr);
	}

	void Game::CheckBikesIntersections()
	{
		// Check if both bikes intersect.
		if (m_RedBike->DoesIntersect(m_BlueBike))
		{
			m_RedAlive = false;
			m_BlueAlive = false;
			return;
		}

		// Check if both bikes are within screen boundaries.
		Vector2 redpos = m_RedBike->GetPosition();
		Vector2 bluepos = m_BlueBike->GetPosition();

		if (redpos.x < 0 || redpos.x > GetScreenWidth() || redpos.y < 0 || redpos.y > GetScreenHeight())
			m_RedAlive = false;
		if (bluepos.x < 0 || bluepos.x > GetScreenWidth() || bluepos.y < 0 || bluepos.y > GetScreenHeight())
			m_BlueAlive = false;

		if (m_RedAlive)
		{
			// Check if the red bike intersects with any blue segments.
			Segment* segment = m_BlueBike->GetNext();

			while (segment != nullptr)
			{
				if (m_RedBike->DoesIntersect(segment))
				{
					m_RedAlive = false;
					break;
				}
				segment = segment->GetNext();
			}

			if (m_RedAlive)
			{
				// Check if the red bike intersects with its own segments.
				int ignorecount = 0;
				segment = m_RedBike->GetNext();

				while (segment != nullptr)
				{
					ignorecount++;

					if (ignorecount >= NEW_SEGMENT_IGNORE_COUNT && m_RedBike->DoesIntersect(segment))
					{
						m_RedAlive = false;
						break;
					}
					segment = segment->GetNext();
				}
			}
		}

		if (m_BlueAlive)
		{
			// Check if the blue bike intersects with any red segments.
			Segment* segment = m_RedBike->GetNext();

			while (segment != nullptr)
			{
				if (m_BlueBike->DoesIntersect(segment))
				{
					m_BlueAlive = false;
					break;
				}
				segment = segment->GetNext();
			}

			if (m_BlueAlive)
			{
				// Check if the blue bike intersects with its own segments.
				int ignorecount = 0;
				segment = m_BlueBike->GetNext();

				while (segment != nullptr)
				{
					ignorecount++;

					if (ignorecount >= NEW_SEGMENT_IGNORE_COUNT && m_BlueBike->DoesIntersect(segment))
					{
						m_BlueAlive = false;
						break;
					}
					segment = segment->GetNext();
				}
			}
		}
	}

	bool Game::IsBikeAddingSegment(Segment* bike)
	{
		Vector2 bikepos = bike->GetPosition();
		Vector2 nextpos = bike->GetNext()->GetPosition();
		Vector2 difference = Vector2(fabsf(bikepos.x - nextpos.x), fabsf(bikepos.y - nextpos.y));

		return difference.x >= MIN_SEGMENT_DISTANCE || difference.y >= MIN_SEGMENT_DISTANCE;
	}

	void Game::HandleLeftMouseClick(float mouseX, float mouseY) { }

	void Game::HandleRightMouseClick(float mouseX, float mouseY) { }

	void Game::HandleKeyPress(Keyboard::Key key)
	{
		if (m_RoundState == RoundState::Running)
		{
			if (key == Keyboard::A)
				Turn(Vector2(-1.0f, 0.0f), m_RedBike, m_RedVelocity, RED_SEGMENT, 0);
			else if (key == Keyboard::D)
				Turn(Vector2(1.0f, 0.0f), m_RedBike, m_RedVelocity, RED_SEGMENT, 180);
			else if (key == Keyboard::W)
				Turn(Vector2(0.0f, 1.0f), m_RedBike, m_RedVelocity, RED_SEGMENT, -90);
			else if (key == Keyboard::S)
				Turn(Vector2(0.0f, -1.0f), m_RedBike, m_RedVelocity, RED_SEGMENT, 90);

			if (key == Keyboard::Left)
				Turn(Vector2(-1.0f, 0.0f), m_BlueBike, m_BlueVelocity, BLUE_SEGMENT, 0);
			else if (key == Keyboard::Right)
				Turn(Vector2(1.0f, 0.0f), m_BlueBike, m_BlueVelocity, BLUE_SEGMENT, 180);
			else if (key == Keyboard::Up)
				Turn(Vector2(0.0f, 1.0f), m_BlueBike, m_BlueVelocity, BLUE_SEGMENT, -90);
			else if (key == Keyboard::Down)
				Turn(Vector2(0.0f, -1.0f), m_BlueBike, m_BlueVelocity, BLUE_SEGMENT, 90);
		}
		else if (m_RoundState == RoundState::GameOver && key == Keyboard::R)
			Reset();
	}
}