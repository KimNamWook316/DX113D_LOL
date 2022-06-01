
#include "MonsterAnimation.h"

CMonsterAnimation::CMonsterAnimation()
{
	SetTypeID<CMonsterAnimation>();
	m_Idle = true;
}

CMonsterAnimation::CMonsterAnimation(const CMonsterAnimation& Anim) :
	CAnimationSequenceInstance(Anim)
{
}

CMonsterAnimation::~CMonsterAnimation()
{
}

bool CMonsterAnimation::Init()
{
	if (!CAnimationSequenceInstance::Init())
		return false;

	AddAnimation("PlayerIdle", "Idle");
	AddAnimation("PlayerAttack", "Attack", false);
	AddAnimation("PlayerWalk", "Walk");

	return true;
}

CMonsterAnimation* CMonsterAnimation::Clone()
{
	return new CMonsterAnimation(*this);
}
