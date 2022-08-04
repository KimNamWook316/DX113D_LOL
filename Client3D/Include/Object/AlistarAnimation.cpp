
#include "AlistarAnimation.h"

CAlistarAnimation::CAlistarAnimation()
{
	SetTypeID<CAlistarAnimation>();
	m_Idle = true;
}

CAlistarAnimation::CAlistarAnimation(const CAlistarAnimation& Anim) :
	CAnimationSequenceInstance(Anim)
{
}

CAlistarAnimation::~CAlistarAnimation()
{
}

bool CAlistarAnimation::Init()
{
	if (!CAnimationSequenceInstance::Init())
		return false;

	AddAnimation("Alistar_Idle", "Alistar_Idle");
	AddAnimation("Alistar_Run", "Alistar_Run");



	return true;
}

CAlistarAnimation* CAlistarAnimation::Clone()
{
	return new CAlistarAnimation(*this);
}

