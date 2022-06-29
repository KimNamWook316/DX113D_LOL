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
