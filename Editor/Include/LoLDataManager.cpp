
#include "LoLDataManager.h"
#include "Resource/ResourceManager.h"
#include "Component/State/StateManager.h"
#include "Scene/SceneManager.h"
#include "Animation/AnimationSequenceInstance.h"

#include <sstream>

DEFINITION_SINGLE(CLoLDataManager)

CLoLDataManager::CLoLDataManager()
{
}

CLoLDataManager::~CLoLDataManager()
{

}

void CLoLDataManager::Init()
{
	ReadChampionNotify();
	ReadChampionSkillInfo();
}

void CLoLDataManager::ReadChampionNotify()
{
	CResourceManager::GetInst()->LoadCSV("AnimationNotify.csv");

	CExcelData* Data = CResourceManager::GetInst()->FindCSV("AnimationNotify");

	if (Data)
	{
		m_mapLoLData.insert(std::make_pair("AnimationNotify", Data));
	}
}

void CLoLDataManager::ReadChampionSkillInfo()
{
	CResourceManager::GetInst()->LoadCSV("SkillInfo.csv");

	CExcelData* Data = CResourceManager::GetInst()->FindCSV("SkillInfo");

	if (Data)
	{
		m_mapLoLData.insert(std::make_pair("SkillInfo", Data));
	}
}

void CLoLDataManager::SetChampionNotify(CAnimationSequenceInstance* Instance, const std::string& ChampionName)
{
	CExcelData* Data = FindLoLData("AnimationNotify");

	if (!Data)
		return;

	CStateManager* StateManager = CSceneManager::GetInst()->GetStateManager();

	// TODO : 챔피언과 스킬이 추가될때마다 여기에 Notify 추가
	if (ChampionName.find("Alistar") != std::string::npos)
	{
		Row* row = Data->GetRow("Alistar");

		size_t Count = row->size();

		for (size_t i = 0; i < Count; ++i)
		{
			std::stringstream ss;

			ss << (*row)[i];

			int Frame = 0;

			ss >> Frame;

			// Q Skill
			if (i == 0)
			{
				Instance->AddNotifyParam<CStateManager>("Alistar_SkillQ", "AlistarQAirborne", Frame, StateManager, &CStateManager::CheckAirborneTarget);

				std::string StrRange = CResourceManager::GetInst()->FindCSV("SkillInfo")->FindData("Alistar", "QRange");
				int Range = 0;
				ss.clear();

				ss << StrRange;
				ss >> Range;

				Instance->SetNotifyParamRange("Alistar_SkillQ", "AlistarQAirborne", Range);
			}
			// W Skill
			if (i == 1)
				Instance->AddNotifyParam<CStateManager>("Airborne", "AlistarQAirborne", Frame, StateManager, &CStateManager::FindKnockBackTarget);
		}

	}
}

void CLoLDataManager::SetChampionInfo(CGameObject* Object, const std::string& ChampionName)
{
	CExcelData* Data = FindLoLData("LoLChampionInfo");

	if (!Data)
		return;

	CStateManager* StateManager = CSceneManager::GetInst()->GetStateManager();

	// TODO : 챔피언 정보 추가될때마다 여기에 Notify 추가
	if (ChampionName.find("Alistar") != std::string::npos)
	{
		Row* row = Data->GetRow("Alistar");

		size_t Count = row->size();

		for (size_t i = 0; i < Count; ++i)
		{
			std::stringstream ss;

			ss << (*row)[i];

			int Info = 0;

			ss >> Info;

			// TODO : LoLChampionInfo.csv에서 읽어오는 항목 늘어날 때 마다 추가해주기

			// Move Speed

			switch (i)
			{
			case 0:
				Object->SetChampionMoveSpeed(Info);
				break;
			case 1:
				Object->SetChampionAttack(Info);
				break;
			case 2:
				Object->SetChampionAttackSpeed(Info);
				break;
			case 3:
				Object->SetChampionArmor(Info);
				break;
			case 4:
				Object->SetChampionAbilityPower(Info);
				break;
			case 5:
				Object->SetChampionMagicResist(Info);
				break;
			case 6:
				Object->SetChampionNormalAttackRange(Info);
				break;
			case 7:
				Object->SetChampionHP(Info);
				break;
			case 8:
				Object->SetChampionMP(Info);
				break;
			}
		}
	}
}

CExcelData* CLoLDataManager::FindLoLData(const std::string& Name)
{
	auto iter = m_mapLoLData.find(Name);

	if (iter == m_mapLoLData.end())
		return nullptr;

	return iter->second;
}
