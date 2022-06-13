#pragma once

#include "GameInfo.h"

class CEngineUtil
{
public:
	static std::string LightTypeToString(Light_Type Type);
	static Light_Type StringToLightType(const std::string& TypeString);

	// String Related
	// 해당 Dir 경로에, 해당 Name 으로 된 파일이 존재하는지 판단해주는 함수
	static bool IsFileExistInDir(const std::string& Path, const std::string& FileName);
	// 풀경로를 넣어주면 파일 이름만 걸러서 리턴해줌
	static std::string FilterFileName(const std::string& FullPath);
};

