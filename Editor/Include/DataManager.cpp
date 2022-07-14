
#include "DataManager.h"
#include "Resource/ResourceManager.h"
#include "Component/State/StateManager.h"
#include "Scene/SceneManager.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ObjectDataComponent.h"

#include <sstream>

CDataManager::CDataManager()
{
}

CDataManager::~CDataManager()
{

}

void CDataManager::Init()
{
	ReadObjectData();
	ReadNotifyData();

	//CSceneManager::GetInst()->SetObjectDataSetFunction(this, &CDataManager::SetObjectData);
}

void CDataManager::ReadObjectData()
{
	CResourceManager::GetInst()->LoadCSV("ObjectData.csv");

	CExcelData* Data = CResourceManager::GetInst()->FindCSV("ObjectData");

	if (Data)
	{
		m_mapData.insert(std::make_pair("ObjectData", Data));
	}
}

void CDataManager::ReadNotifyData()
{

}

void CDataManager::SetObjectData(CGameObject* Object)
{
	Object_Type ObjectType = Object->GetObjectType();
	CExcelData* Data = FindData("ObjectData");
	CObjectDataComponent* Comp = Object->FindComponentFromType<CObjectDataComponent>();

	if (!Comp)
		return;

	switch (ObjectType)
	{
		case Object_Type::Player:
			Row* row = Data->GetRow("Player");

			size_t Count = row->size();

			for (size_t i = 0; i < Count; ++i)
			{
				std::stringstream ss;

				ss << (*row)[i];

				int Frame = 0;

				ss >> Frame;

				if (i == 0)
					Comp->SetHP(Frame);

				if (i == 1)
					Comp->SetMP(Frame);

					if (i == 2)
						Comp->SetMoveSpeed((float)Frame);

				if (i == 3)
					Comp->SetAttack(Frame);
			}

		break;
	}
}

void CDataManager::SetNotify(CGameObject* Object, const std::string& ObjectName)
{
}


CExcelData* CDataManager::FindData(const std::string& Name)
{
	auto iter = m_mapData.find(Name);

	if (iter == m_mapData.end())
		return nullptr;

	return iter->second;
}
