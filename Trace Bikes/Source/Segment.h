#pragma once

#include <GameDev2D.h>

namespace GameDev2D
{
    class Segment : public Sprite
	{
	public:
		Segment(const Vector2& position, const string& SegmentColor);
		~Segment();

		bool DoesIntersect(Segment* segment);

		void SetNext(Segment* next);
		void LinkNext(Segment* next);

		Segment* GetNext();

	private:
		Segment* m_Next;
	};
}