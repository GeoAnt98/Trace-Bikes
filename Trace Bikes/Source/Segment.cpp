#include "Segment.h"
#include <string.h>

namespace GameDev2D
{
	Segment::Segment(const Vector2& position, const string& SegmentColor) : Sprite(SegmentColor)
	{
		m_Next = nullptr;

		SetPosition(position);
		SetAnchor(Vector2(0.5f, 0.5f));
	}

	Segment::~Segment()
	{

	}

    bool Segment::DoesIntersect(Segment* segment)
    {
        float Left = GetPosition().x - GetWidth() * 0.5f;
        float Right = GetPosition().x + GetWidth() * 0.5f;
        float Top = GetPosition().y + GetHeight() * 0.5f;
        float Bottom = GetPosition().y - GetHeight() * 0.5f;

        float SegmentLeft = segment->GetPosition().x - segment->GetWidth() * 0.5f;
        float SegmentRight = segment->GetPosition().x + segment->GetWidth() * 0.5f;
        float SegmentTop = segment->GetPosition().y + segment->GetHeight() * 0.5f;
        float SegmentBottom = segment->GetPosition().y - segment->GetHeight() * 0.5f;

		return !(SegmentLeft > Right || SegmentRight < Left || SegmentBottom > Top || SegmentTop < Bottom);
    }

	void Segment::SetNext(Segment* next)
	{
		m_Next = next;
	}

	void Segment::LinkNext(Segment* next)
	{
		if (m_Next != nullptr)
			next->SetNext(m_Next);

		m_Next = next;
	}

	Segment* Segment::GetNext()
	{
		return m_Next;
	}
}