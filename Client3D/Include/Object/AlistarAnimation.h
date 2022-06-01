#pragma once

#include "Animation/AnimationSequenceInstance.h"

class CAlistarAnimation :
    public CAnimationSequenceInstance
{
public:
	CAlistarAnimation();
	CAlistarAnimation(const CAlistarAnimation& Anim);
	~CAlistarAnimation();

protected:
	bool        m_Idle;

public:
	bool GetIdleEnable()	const
	{
		return m_Idle;
	}

	void SetIdleEnable(bool Enable)
	{
		m_Idle = Enable;
	}

public:
	virtual bool Init();
	virtual CAlistarAnimation* Clone();
};

