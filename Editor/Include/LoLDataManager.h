#pragma once

#include "GameInfo.h"

// csv에서 정보들을 읽어오는 Manager
class CLoLDataManager
{

private:
	std::unordered_map <std::string, class CExcelData*> m_mapLoLData;

public:
	void Init();

private:
	void ReadChampionNotify();
	void ReadChampionSkillInfo();
	void ReadMonsterNotify();

public:
	void SetChampionNotify(class CAnimationSequenceInstance* Instance, const std::string& ChampionName);
	void SetChampionInfo(class CGameObject* Object, const std::string& ChampionName);

public:
	class CExcelData* FindLoLData(const std::string& Name);

	DECLARE_SINGLE(CLoLDataManager)
};

