#pragma once

#include "GameInfo.h"

class CEngineUtil
{
public:
	static std::string LightTypeToString(Light_Type Type);
	static Light_Type StringToLightType(const std::string& TypeString);

	// String Related
	// 해당 Dir 경로에, 해당 Name 으로 된 파일이 존재하는지 판단해주는 함수 + 존재할 시 FullPath 경로 리턴
	static std::optional<std::string> CheckAndExtractFullPathOfTargetFile(std::string_view Path, std::string_view FileName);

	// (OBJ : 혹시나 사용하고 있는 사람이 있나하여, 기존 함수 남겨둠) 해당 Dir 경로에, 해당 Name 으로 된 파일이 존재하는지 판단해주는 함수 
	static bool IsFileExistInDir(const std::string& Path, const std::string& FileName);

	// 풀경로를 넣어주면 파일 이름만 걸러서 리턴해줌
	static std::string FilterFileName(const std::string& FullPath);
	
	// FullPath 가 C::\\Windows\\Project\\Bin\\Hello.txt 이고, 
	// FolderName 이 Project 라면 => Bin\\Hello.txt 라는 Path 정보를 return 해준다.
	// std::string_view GetFilePathAfterSpecificFolder(std::string_view FolderName, std::string_view FullPath);
};

