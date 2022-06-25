
#include "GameDataComponent.h"
#include "../DataManager.h"
#include "GameObject/GameObject.h"
#include "Resource/Excel/ExcelData.h"
#include "../EditorManager.h"

#include <sstream>

CGameDataComponent::CGameDataComponent()
{
	SetTypeID<CGameDataComponent>();
	m_ComponentType = Component_Type::ObjectComponent;
}

CGameDataComponent::CGameDataComponent(const CGameDataComponent& com) :
	CObjectComponent(com)
{
}

CGameDataComponent::~CGameDataComponent()
{
}

void CGameDataComponent::Start()
{
	CObjectComponent::Start();
}

bool CGameDataComponent::Init()
{


	return true;
}

void CGameDataComponent::Update(float DeltaTime)
{

}

void CGameDataComponent::PostUpdate(float DeltaTime)
{

}

void CGameDataComponent::PrevRender()
{
}

void CGameDataComponent::Render()
{
}

void CGameDataComponent::PostRender()
{
}

CGameDataComponent* CGameDataComponent::Clone()
{
	return new CGameDataComponent(*this);
}

bool CGameDataComponent::Save(FILE* File)
{
	CObjectComponent::Save(File);

	//fwrite(&m_Data, sizeof(GameData), 1, File);

	return true;
}

bool CGameDataComponent::Load(FILE* File)
{
	CObjectComponent::Load(File);

	//fread(&m_Data, sizeof(GameData), 1, File);

	CExcelData* Data = CEditorManager::GetInst()->GetDataManager()->FindData("GameData");
	Row* row = nullptr;

	if (Data)
	{
		if (m_Object->GetObjectType() == Object_Type::Player)
		{

		}

		else if (m_Object->GetObjectType() == Object_Type::Monster)
		{
			
		}

		else if (m_Object->GetObjectType() == Object_Type::MapObject)
		{

		}

		if (row)
		{
			size_t Count = row->size();

			for (size_t i = 0; i < Count; ++i)
			{
				std::stringstream ss;

				ss << (*row)[i];

				int Data = 0;

				ss >> Data;

			}
		}

	}

	return true;
}

bool CGameDataComponent::SaveOnly(FILE* File)
{
	return false;
}

bool CGameDataComponent::LoadOnly(FILE* File)
{
	return false;
}
