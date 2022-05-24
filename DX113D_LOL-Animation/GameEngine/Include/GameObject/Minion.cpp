
#include "Minion.h"

CMinion::CMinion()
{
	SetTypeID<CMinion>();
}

CMinion::CMinion(const CMinion& obj)	:
	CMovingObject(obj)
{
}

CMinion::~CMinion()
{
}

void CMinion::Start()
{
	CMovingObject::Start();
}

bool CMinion::Init()
{
	CMovingObject::Init();

	return true;	
}

void CMinion::Update(float DeltaTime)
{
}

void CMinion::PostUpdate(float DeltaTime)
{
}

void CMinion::PrevRender()
{
}

void CMinion::Render()
{
}

void CMinion::PostRender()
{
}

CMinion* CMinion::Clone()
{
	return nullptr;
}
