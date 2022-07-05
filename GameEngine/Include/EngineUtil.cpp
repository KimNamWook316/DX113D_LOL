#include "EngineUtil.h"
#include "PathManager.h"
#include <filesystem>

namespace fs = std::filesystem;

std::string CEngineUtil::LightTypeToString(Light_Type Type)
{
	std::string ret = "";
	switch (Type)
	{
	case Light_Type::Dir:
		ret = "Dir";
		break;
	case Light_Type::Point:
		ret = "Point";
		break;
	case Light_Type::Spot:
		ret = "Spot";
		break;
	}
	return ret;
}

Light_Type CEngineUtil::StringToLightType(const std::string& TypeString)
{
	if (TypeString == "Dir")
	{
		return Light_Type::Dir;
	}
	else if (TypeString == "Point")
	{
		return Light_Type::Point;
	}
	else if (TypeString == "Spot")
	{
		return Light_Type::Spot;
	}
	return (Light_Type)(-1);
}

std::string CEngineUtil::FogTypeToString(Fog_Type Type)
{
	std::string ret = "";
	switch (Type)
	{
	case Fog_Type::Linear:
		ret = "Linear";
		break;
	case Fog_Type::Exp:
		ret = "Exp";
		break;
	case Fog_Type::Exp2:
		ret = "Exp2";
		break;
	}
	return ret;
}

Fog_Type CEngineUtil::StringToFogType(const std::string& TypeString)
{
	if (TypeString == "Linear")
	{
		return Fog_Type::Linear;
	}
	else if (TypeString == "Exp")
	{
		return Fog_Type::Exp;
	}
	else if (TypeString == "Exp2")
	{
		return Fog_Type::Exp2;
	}
	return (Fog_Type)(-1);
}

std::string CEngineUtil::ToonShaderTypeToString(ToonShaderType Type)
{
	std::string ret = "";
	switch (Type)
	{
	case ToonShaderType::Default:
		ret = "Default";
		break;
	case ToonShaderType::Easy:
		ret = "Easy";
		break;
	case ToonShaderType::Light:
		ret = "Light";
		break;
	case ToonShaderType::Warm:
		ret = "Warm";
		break;
	}
	return ret;
}

ToonShaderType CEngineUtil::StringToToonShaderType(const std::string& TypeString)
{
	if (TypeString == "Default")
	{
		return ToonShaderType::Default;
	}
	else if (TypeString == "Easy")
	{
		return ToonShaderType::Easy;
	}
	else if (TypeString == "Light")
	{
		return ToonShaderType::Light;
	}
	else if (TypeString == "Warm")
	{
		return ToonShaderType::Warm;
	}
	return (ToonShaderType)(-1);
}

std::optional<std::string> CEngineUtil::CheckAndExtractFullPathOfTargetFile(std::string_view PathName, std::string_view TargetFileName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName.data());

	if (!Info)
		return  std::nullopt;

	char FolderName[MAX_PATH] = {};

	strcpy_s(FolderName, Info->PathMultibyte);

	fs::path TargetFolder(FolderName);

	for (const fs::directory_entry& entry :
		fs::recursive_directory_iterator(TargetFolder))
	{
		const std::string& FileName = FilterFileName(entry.path().u8string());

		if (strcmp(FileName.c_str(), TargetFileName.data()) == 0)
		{
			return entry.path().u8string();
		}
	}

	return std::nullopt;
}

bool CEngineUtil::IsFileExistInDir(const std::string& PathName, const std::string& TargetFileName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (!Info)
		return  false;

	char FolderName[MAX_PATH] = {};

	strcpy_s(FolderName, Info->PathMultibyte);

	fs::path TargetFolder(FolderName);

	for (const fs::directory_entry& entry :
		fs::recursive_directory_iterator(TargetFolder))
	{
		const std::string& FileName = FilterFileName(entry.path().u8string());

		if (strcmp(FileName.c_str(), TargetFileName.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}

bool CEngineUtil::CopyFileToOtherDirectory(const PathInfo* CurrentPathInfo, const PathInfo* TargetPathInfo, const std::string& FileName)
{
	if (!CurrentPathInfo || !TargetPathInfo)
		return false;

	// 현재 디렉토리 정보를 만든다.
	std::string CurrentPath = CurrentPathInfo->PathMultibyte;
	CurrentPath.append(FileName);

	std::string TargetPath = TargetPathInfo->PathMultibyte;
	TargetPath.append(FileName);

	fs::path FromFilePath(CurrentPath);
	fs::path ToFilePath(TargetPath);

	// 둘중 하나라도 파일이 아니라면 X (둘중 하나라도 Directory라면)
	if (fs::is_directory(FromFilePath) || fs::is_directory(ToFilePath))
		return false;

	fs::copy(FromFilePath, ToFilePath);

	return true;
}

bool CEngineUtil::GetPathInfoBeforeFileName(const std::string& FilePath, std::string& ExtractedPathInfo)
{
	int FilePathLength = (int)FilePath.size();

	for (int i = FilePathLength - 1; i >= 0; --i)
	{
		if (FilePath[i] == '\\')
		{
			ExtractedPathInfo = FilePath.substr(0, (size_t)i + 1);
			return true;
		}
	}

	ExtractedPathInfo = FilePath;

	return true;
}

bool CEngineUtil::GetFileNameAfterSlash(const std::string& FilePath, std::string& ExtractedFileName)
{
	int FilePathLength = (int)FilePath.size();

	for (int i = FilePathLength - 1; i >= 0; --i)
	{
		if (FilePath[i] == '\\')
		{
			ExtractedFileName = FilePath.substr((size_t)i + 1, FilePath.size());
			return true;
		}
	}

	ExtractedFileName = FilePath;

	return true;
}

bool CEngineUtil::GetFileExt(const std::string& FileName, std::string& ExtractedExt)
{
	size_t i = FileName.find('.');

	if (i != std::string::npos)
	{
		ExtractedExt = FileName.substr(i + 1, FileName.length() - i);
		return true;
	}

	return false;
}

bool CEngineUtil::GetFileNameOnly(const std::string& FullFileName, std::string& ExtractedFileName)
{
	size_t i = FullFileName.find('.');

	if (i != std::string::npos)
	{
		ExtractedFileName = FullFileName.substr(0, i);
		return true;
	}

	return false;
}


bool CEngineUtil::CopyFileToOtherDirectory(const std::string& OriginFullPath, const std::string& TargetFullPath,
	std::string& FinalCopyPath, bool Recursvie)
{
	fs::path originalPath = OriginFullPath;
	fs::path copiedPath = TargetFullPath;

	// 같은 경로라면, _(num) 을 붙여가면서, 해당 경로에 같은 이름이 없을 때까지 반복해가며 새로운 이름으로 만들어낸다.
	// if (originalPath == copiedPath)
	if (fs::exists(copiedPath))
	{
		std::string FolderPathStr;
		std::string FileNameStr;
		std::string FileNameOnlyStr;
		std::string FileExtStr;

		GetPathInfoBeforeFileName(OriginFullPath, FolderPathStr);
		GetFileNameAfterSlash(OriginFullPath, FileNameStr);
		GetFileExt(FileNameStr, FileExtStr);
		GetFileNameOnly(FileNameStr, FileNameOnlyStr);

		fs::path FolderPathInfo = FolderPathStr;

		int ExtraNameNum = 0;

		while (true)
		{
			std::string SavedCopyPath;
			SavedCopyPath.reserve(FolderPathStr.length() * 2);

			SavedCopyPath.append(FolderPathStr);
			SavedCopyPath.append("\\");
			SavedCopyPath.append(FileNameOnlyStr);
			SavedCopyPath.append("_" + std::to_string(ExtraNameNum));
			SavedCopyPath.append("." + FileExtStr);

			// 만약 현재 폴더 경로에 해당 이름으로 된 파일이 없다면
			if (fs::exists(SavedCopyPath) == false)
			{
				copiedPath = SavedCopyPath;
				break;
			}

			++ExtraNameNum;
		}
	}

	if (!Recursvie)
	{
		fs::copy(originalPath, copiedPath);
	}
	else
	{
		fs::copy(originalPath, copiedPath, fs::copy_options::recursive);
	}

	FinalCopyPath = copiedPath.string();

	return true;
}

void CEngineUtil::CheckAndMakeDirectory(const PathInfo* Info)
{
	if (!Info)
		return;

	fs::path dir(Info->PathMultibyte);

	// 이미 존재하면 return
	if (fs::exists(dir))
		return;

	// 해당 디렉토리까지 타고 들어가는, 부모 디렉토리들까지 한번에 만들어준다.
	fs::create_directories(dir);
}

std::string CEngineUtil::FilterFileName(const std::string& FullPath)
{
	size_t len = FullPath.length();

	std::string FileName;

	for (size_t i = len - 1; i >= 0; --i)
	{
		if (FullPath[i] == '\\' || FullPath[i] == '/')
		{
			FileName = FullPath.substr(i + 1);
			return FileName;
		}
	}

	return FileName;
}

std::string CEngineUtil::CollisionChannelToString(Collision_Channel eChannnel)
{
	std::string Out;

	switch (eChannnel)
	{
	case Collision_Channel::Object:
		Out = "Object";
		break;
	case Collision_Channel::Player:
		Out = "Player";
		break;
	case Collision_Channel::Monster:
		Out = "Monster";
		break;
	case Collision_Channel::PlayerAttack:
		Out = "PlayerAttack";
		break;
	case Collision_Channel::MonsterAttack:
		Out = "MonsterAttack";
		break;
	case Collision_Channel::Custom1:
		Out = "Custom1";
		break;
	case Collision_Channel::Custom2:
		Out = "Custom2";
		break;
	case Collision_Channel::Custom3:
		Out = "Custom3";
		break;
	case Collision_Channel::Custom4:
		Out = "Custom4";
		break;
	case Collision_Channel::Custom5:
		Out = "Custom5";
		break;
	case Collision_Channel::Custom6:
		Out = "Custom6";
		break;
	case Collision_Channel::Custom7:
		Out = "Custom7";
		break;
	case Collision_Channel::Custom8:
		Out = "Custom8";
		break;
	case Collision_Channel::Custom9:
		Out = "Custom9";
		break;
	case Collision_Channel::Custom10:
		Out = "Custom10";
		break;
	case Collision_Channel::Custom11:
		Out = "Custom11";
		break;
	case Collision_Channel::Custom12:
		Out = "Custom12";
		break;
	case Collision_Channel::Custom13:
		Out = "Custom13";
		break;
	case Collision_Channel::Custom14:
		Out = "Custom14";
		break;
	case Collision_Channel::Custom15:
		Out = "Custom15";
		break;
	case Collision_Channel::Custom16:
		Out = "Custom16";
		break;
	default:
		assert(false);
		break;
	}

	return Out;
}

Collision_Channel CEngineUtil::StringToCollisionChannel(const std::string& String)
{
	if (String == "Object")
	{
		return Collision_Channel::Object;
	}
	else if (String == "Player")
	{
		return Collision_Channel::Player;
	}
	else if (String == "PlayerAttack")
	{
		return Collision_Channel::PlayerAttack;
	}
	else if (String == "Monster")
	{
		return Collision_Channel::Monster;
	}
	else if (String == "MonsterAttack")
	{
		return Collision_Channel::MonsterAttack;
	}
	else if (String == "Custom1")
	{
		return Collision_Channel::Custom1;
	}
	else if (String == "Custom2")
	{
		return Collision_Channel::Custom2;
	}
	else if (String == "Custom3")
	{
		return Collision_Channel::Custom3;
	}
	else if (String == "Custom4")
	{
		return Collision_Channel::Custom4;
	}
	else if (String == "Custom5")
	{
		return Collision_Channel::Custom5;
	}
	else if (String == "Custom6")
	{
		return Collision_Channel::Custom6;
	}
	else if (String == "Custom7")
	{
		return Collision_Channel::Custom7;
	}
	else if (String == "Custom8")
	{
		return Collision_Channel::Custom8;
	}
	else if (String == "Custom9")
	{
		return Collision_Channel::Custom9;
	}
	else if (String == "Custom10")
	{
		return Collision_Channel::Custom10;
	}
	else if (String == "Custom11")
	{
		return Collision_Channel::Custom11;
	}
	else if (String == "Custom12")
	{
		return Collision_Channel::Custom12;
	}
	else if (String == "Custom13")
	{
		return Collision_Channel::Custom14;
	}
	else if (String == "Custom14")
	{
		return Collision_Channel::Custom14;
	}
	else if (String == "Custom15")
	{
		return Collision_Channel::Custom15;
	}
	else if (String == "Custom16")
	{
		return Collision_Channel::Custom16;
	}
	return (Collision_Channel)(-1);
}

Collision_Interaction CEngineUtil::StringToCollisionInteraction(const std::string& String)
{
	if (String == "TRUE")
	{
		return Collision_Interaction::Collision;
	}
	else
	{
		return Collision_Interaction::Ignore;
	}
}

std::string CEngineUtil::BoolToString(bool Bool)
{
	std::string Ret = Bool ? "TRUE" : "FALSE";
	return Ret;
}
