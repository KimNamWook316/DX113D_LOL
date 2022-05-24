
#include "MovingObject.h"

CMovingObject::CMovingObject()
{
	SetTypeID<CMovingObject>();
}

CMovingObject::CMovingObject(const CMovingObject& obj)	:
	CGameObject(obj)
{
}

CMovingObject::~CMovingObject()
{
}

void CMovingObject::Start()
{
	CGameObject::Start();
}

bool CMovingObject::Init()
{
	CGameObject::Init();

	return true;
}

void CMovingObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMovingObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CMovingObject::PrevRender()
{
	CGameObject::PrevRender();
}

void CMovingObject::Render()
{
	CGameObject::Render();
}

void CMovingObject::PostRender()
{
	CGameObject::PostRender();
}

CMovingObject* CMovingObject::Clone()
{
	return new CMovingObject(*this);
}
