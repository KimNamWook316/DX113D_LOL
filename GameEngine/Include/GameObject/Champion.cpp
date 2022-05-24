
#include "Champion.h"

CChampion::CChampion()
{
	SetTypeID<CChampion>();
}

CChampion::CChampion(const CChampion& obj)	:
	CMovingObject(obj)
{
}

CChampion::~CChampion()
{
}

void CChampion::Start()
{
	CMovingObject::Start();
}

bool CChampion::Init()
{
	CMovingObject::Init();

	return true;
}

void CChampion::Update(float DeltaTime)
{
	CMovingObject::Update(DeltaTime);
}

void CChampion::PostUpdate(float DeltaTime)
{
	CMovingObject::PostUpdate(DeltaTime);
}

void CChampion::PrevRender()
{
	CMovingObject::PrevRender();
}

void CChampion::Render()
{
	CMovingObject::Render();
}

void CChampion::PostRender()
{
	CMovingObject::PostRender();
}

CChampion* CChampion::Clone()
{
	return new CChampion(*this);
}
