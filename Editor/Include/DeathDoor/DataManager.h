#pragma once

#include "GameInfo.h"

// csv에서 정보들을 읽어오는 Manager
class CDataManager
{
private:
	std::unordered_map <std::string, class CExcelData*> m_mapData;

public:
	virtual void Init();

private:
	void ReadObjectData();
	//void ReadPoolData();

public:
	const ObjectData& GetObjectData(const std::string& Key);
	//int GetObjectPoolData(const std::string& Key);
	class CExcelData* FindData(const std::string& Name);

	DECLARE_SINGLE(CDataManager)
};

