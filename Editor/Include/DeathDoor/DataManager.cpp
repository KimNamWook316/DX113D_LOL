
#include "DataManager.h"
#include "Resource/ResourceManager.h"
#include "Component/State/StateManager.h"
#include "Scene/SceneManager.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ObjectDataComponent.h"

#include <sstream>

DEFINITION_SINGLE(CDataManager)

CDataManager::CDataManager()
{
}

CDataManager::~CDataManager()
{

}

void CDataManager::Init()
{
	ReadObjectData();

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

// TODO : 데이터 추가될 때마다 함수 업데이트 할 것
const ObjectData& CDataManager::GetObjectData(const std::string& Key)
{
	ObjectData Data = {};

	CExcelData* Excel = FindData("ObjectData");

	Row* row = Excel->GetRow(Key);

	if (!row)
	{
		return Data;
	}

	size_t Count = row->size();

	for (size_t i = 0; i < Count; ++i)
	{
		std::stringstream ss;

		ss << (*row)[i];

		int Frame = 0;

		ss >> Frame;

		if (i == 0)
		{
			Data.HP = Frame;
		}
		else if (i == 1)
		{
			Data.DetectRange = (float)Frame;
		}
		else if (i == 2)
		{
			Data.MoveSpeed = (float)Frame;
		}
		else if (i == 3)
		{
			Data.Attack = (int)Frame;
		}
		else if (i == 4)
		{
			Data.MeleeAttackRange = (float)Frame;
		}
		else if (i == 5)
		{
			Data.JumpAttackRange = (float)Frame;
		}
		else if (i == 6)
		{
			Data.RotateSpeedPerSec = (float)Frame;
		}
	}

	return Data;
}

CExcelData* CDataManager::FindData(const std::string& Name)
{
	auto iter = m_mapData.find(Name);

	if (iter == m_mapData.end())
		return nullptr;

	return iter->second;
}
