
#include "TowerComponent.h"

CTowerComponent::CTowerComponent()	:
	m_TowerOrder(1)
{
	m_TowerLine = LoLLine::Top;
	SetTypeID<CTowerComponent>();
}

CTowerComponent::CTowerComponent(const CTowerComponent& com)	:
	CSceneComponent(com)
{
}

CTowerComponent::~CTowerComponent()
{
}

bool CTowerComponent::Save(FILE* File)
{
	fwrite(&m_TowerLine, sizeof(LoLLine), 1, File);
	fwrite(&m_TowerOrder, sizeof(int), 1, File);

	CSceneComponent::Save(File);

	return true;
}

bool CTowerComponent::Load(FILE* File)
{
	fread(&m_TowerLine, sizeof(LoLLine), 1, File);
	fread(&m_TowerOrder, sizeof(int), 1, File);

	CSceneComponent::Load(File);

	return true;
}

bool CTowerComponent::SaveOnly(FILE* File)
{
	return true;
}

bool CTowerComponent::LoadOnly(FILE* File)
{
	return true;
}
