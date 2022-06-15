#pragma once

#include "Component/ObjectComponent.h"

class CChampionAIComponent :
	public CObjectComponent
{
	friend class CGameObject;

protected:
	CChampionAIComponent();
	CChampionAIComponent(const CChampionAIComponent& com);
	virtual ~CChampionAIComponent();

private:
	LoLLine m_Line;
	int m_DetectRange;

public:
	void SetLine(const LoLLine& Line)
	{
		m_Line = Line;
	}

	void SetDetectRange(int Range)
	{
		m_DetectRange = Range;
	}

	const LoLLine& GetLine()	const
	{
		return m_Line;
	}

	int GetDetectRange()	const
	{
		return m_DetectRange;
	}
};

