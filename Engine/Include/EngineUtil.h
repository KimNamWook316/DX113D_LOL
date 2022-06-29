#pragma once

#include "GameInfo.h"

class CEngineUtil
{
public:
	// Enum To String
	static std::string LightTypeToString(Light_Type Type);
	static Light_Type StringToLightType(const std::string& TypeString);

	static std::string FogTypeToString(Fog_Type Type);
	static Fog_Type StringToFogType(const std::string& TypeString);

	// String Related
	// 해당 Dir 경로에, 해당 Name 으로 된 파일이 존재하는지 판단해주는 함수 + 존재할 시 FullPath 경로 리턴
	static std::optional<std::string> CheckAndExtractFullPathOfTargetFile(std::string_view Path, std::string_view FileName);

	// (OBJ : 혹시나 사용하고 있는 사람이 있나하여, 기존 함수 남겨둠) 해당 Dir 경로에, 해당 Name 으로 된 파일이 존재하는지 판단해주는 함수 
	static bool IsFileExistInDir(const std::string& Path, const std::string& FileName);

	// 특정 파일을, 현재 디렉토리에서, 다른 디렉토리로 복사하기
	// FileName 에는 확장자 정보까지 들어가 있어야 한다.
	static bool CopyFileToOtherDirectory(const struct PathInfo* CurrentPathInfo, const struct PathInfo* TargetPathInfo, const std::string& FileName);

	// 해당 디렉토리가 존재하는지 확인하고, 존재하지 않는다면 해당 디렉토리를 만들어준다. (파일이 아니라, 폴더를 만들어주는 것)
	static void CheckAndMakeDirectory(const struct PathInfo* Info);

	// 풀경로를 넣어주면 파일 이름만 걸러서 리턴해줌
	static std::string FilterFileName(const std::string& FullPath);

	// Collision Profile
	static std::string CollisionChannelToString(Collision_Channel eChannnel);
	static Collision_Channel StringToCollisionChannel(const std::string& String);
	static Collision_Interaction StringToCollisionInteraction(const std::string& String);

	// String
	static std::string BoolToString(bool Bool);
};

