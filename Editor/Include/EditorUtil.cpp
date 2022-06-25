
#include "EditorUtil.h"
#include "PathManager.h"
#include "GameObject/Minion.h"
#include "GameObject/Champion.h"
#include "GameObject/MapObject.h"
#include "Component/LightComponent.h"
#include "Component/Arm.h"
#include "Component/LandScape.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/PaperBurnComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/GameStateComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Component/GameDataComponent.h"
#include "Component/PlayerDataComponent.h"
#include "IMGUITree.h"
#include "Flag.h"


#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::string CEditorUtil::FilterFileName(const std::string& FullPath)
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

void CEditorUtil::GetAllFilenames(const std::string& PathName, std::vector<std::string>& vecFileNames, std::vector<std::string>& vecDirNames)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (!Info)
		return;

	fs::path p = Info->Path;

	fs::directory_iterator itr(p);

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		fs::path iterpath = entry.path();

		bool isDirectory = fs::is_directory(iterpath);

		std::string fullPath = entry.path().string();

		std::string fileName = FilterFileName(fullPath);

		if (isDirectory)
			vecDirNames.push_back(fileName);

		else
			vecFileNames.push_back(fileName);

		++itr;
	}
}

void CEditorUtil::GetAllFilenamesFullPath(const std::string& FullPath, std::vector<std::string>& vecFileNames, std::vector<std::string>& vecDirNames)
{
	fs::path p = FullPath;

	fs::directory_iterator itr(p);

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		fs::path iterpath = entry.path();

		bool isDirectory = fs::is_directory(iterpath);

		std::string fullPath = entry.path().string();

		std::string fileName = FilterFileName(fullPath);

		if (isDirectory)
			vecDirNames.push_back(fileName);

		else
			vecFileNames.push_back(fileName);

		++itr;
	}
}

void CEditorUtil::GetAllFileFullPathInDir(const char* TargetDir, std::vector<std::string>& OutVecFullPath, const char* ExtFilter)
{
	fs::directory_iterator itr(TargetDir);

	char Ext[_MAX_EXT] = {};

	if (ExtFilter)
	{
		strcpy_s(Ext, ExtFilter);
		_strupr_s(Ext);
	}

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		fs::path iterpath = entry.path();

		bool isDirectory = fs::is_directory(iterpath);

		if (isDirectory)
		{
			++itr;
			continue;
		}

		std::string fullPath = entry.path().string();

		if (ExtFilter)
		{
			if (!CompareExt(fullPath.c_str(), Ext))
			{
				++itr;
				continue;
			}
		}

		OutVecFullPath.push_back(fullPath);

		++itr;
	}
}

void CEditorUtil::GetAllFileNamesInDir(const char* TargetDir, std::vector<std::string>& OutVecFileName, const char* ExtFilter)
{
	fs::directory_iterator itr(TargetDir);

	char Ext[_MAX_EXT] = {};

	if (ExtFilter)
	{
		strcpy_s(Ext, ExtFilter);
		_strupr_s(Ext);
	}

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;
		fs::path iterpath = entry.path();
		bool isDirectory = fs::is_directory(iterpath);
		if (isDirectory)
		{
			++itr;
			continue;
		}

		std::string fileName = entry.path().filename().string();

		if (ExtFilter)
		{
			if (!CompareExt(fileName.c_str(), Ext))
			{
				++itr;
				continue;
			}
		}

		OutVecFileName.push_back(fileName);

		++itr;
	}
}

void CEditorUtil::FilterSpecificNameIncludedFilePath(std::vector<std::string>& InputVecFullPath, std::vector<std::string>& ReturnVecFullPath, 
	const char* IncludeName)
{
	ReturnVecFullPath.clear();
	ReturnVecFullPath.reserve(InputVecFullPath.size());

	size_t InputSize = InputVecFullPath.size();
	
	for (size_t i = 0; i < InputSize; ++i)
	{
		// Common Name 을 포함하고 있지 않다면 Skips
		// size_t nPos = InputVecFullPath[i].find(IncludeName);
		// FullPath 에서 중간 경로 들이 아니라, 오로지 FileName 이 IncludeName Str 을 포함하고 있는지를 조사해야 한다.
		// 따라서 폴더 경로 제외, FileName 을 중간에 추출해야 한다.
		std::string CurFileName;
		GetFileNameAfterSlash(InputVecFullPath[i], CurFileName);
		size_t nPos = CurFileName.find(IncludeName);

		if (nPos == std::string::npos)
			continue;

		ReturnVecFullPath.push_back(InputVecFullPath[i]);
	}
}

void CEditorUtil::GetFullPathDirectory(CIMGUITree* CurrentDir, std::list<std::string>& Output)
{
	while (CurrentDir->GetParent() != nullptr)
	{
		Output.push_front(CurrentDir->GetName());
		CurrentDir = CurrentDir->GetParent();
	}
}

std::string CEditorUtil::MergeFullPath(const std::list<std::string> DirNames)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	std::string FullPath = Info->PathMultibyte;

	auto iter = DirNames.begin();
	auto iterEnd = DirNames.end();

	for (; iter != iterEnd; ++iter)
	{
		FullPath += (*iter);
		FullPath += "\\";
	}

	return FullPath;
}

std::optional<std::string> CEditorUtil::GetFullPathOfTargetFileNameInDir(const std::string& PathName,
	const std::string& TargetFileName, std::string& FileNameOnly)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (!Info)
		return  std::nullopt;

	char FolderName[MAX_PATH] = {};

	strcpy_s(FolderName, Info->PathMultibyte);

	fs::path TargetFolder(FolderName);
	
	for (const fs::directory_entry& entry :
		fs::recursive_directory_iterator(TargetFolder))
	{
		const std::string& FileName = FilterFileName(entry.path().u8string());
	
		if (strcmp(FileName.c_str(), TargetFileName.c_str()) == 0)
		{
			GetFileNameOnly(fs::path(entry.path()).filename().u8string(), FileNameOnly);

			return entry.path().u8string();
		}
	}

	return std::nullopt;
}

bool CEditorUtil::GetFileExt(const std::string& FileName, std::string& ExtractedExt)
{
	size_t i = FileName.find('.');

	if (i != std::string::npos)
	{
		ExtractedExt = FileName.substr(i+1, FileName.length() - i);
		return true;
	}

	return false;
}

bool CEditorUtil::GetFileNameOnly(const std::string& FullFileName, std::string& ExtractedFileName)
{
	size_t i = FullFileName.find('.');

	if (i != std::string::npos)
	{
		ExtractedFileName = FullFileName.substr(0, i);
		return true;
	}

	return false;
}

bool CEditorUtil::GetFileNameAfterSlash(const std::string& FilePath, std::string& ExtractedFileName)
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

bool CEditorUtil::GetPathInfoBeforeFileName(const std::string& FilePath, std::string& ExtractedPathInfo)
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

const char* CEditorUtil::ChangeTCHARTextToMultibyte(TCHAR* TCHARText)
{
	char FilePathMultibyte[MAX_PATH] = {};

	int ConvertLength = WideCharToMultiByte(CP_ACP, 0, TCHARText, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, TCHARText, -1, FilePathMultibyte, ConvertLength, 0, 0);

	return FilePathMultibyte;
}

// void CEditorUtil::ChangeMultibyteTextToTCHAR(const std::string& MText, std::wstring& TCHARTextResult)
const TCHAR* CEditorUtil::ChangeMultibyteTextToTCHAR(const std::string& MText)
{
	TCHAR TCHARTextResult[MAX_PATH] = {};

	int ConvertLength = MultiByteToWideChar(CP_ACP, 0, MText.c_str(), -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, MText.c_str(), -1, TCHARTextResult, ConvertLength);

	return TCHARTextResult;
}

void CEditorUtil::ExtractFileNameAndExtFromPath(const std::string& FullPath, std::string& FileNameResult, std::string& ExtResult)
{
	char FileName[MAX_PATH] = {};
	char FileExt[_MAX_EXT] = {};

	_splitpath_s(FullPath.c_str(), nullptr, 0, nullptr, 0, FileName, MAX_PATH, FileExt, _MAX_EXT);

	FileNameResult = FileName;
	ExtResult = FileExt;
}

void CEditorUtil::ExtractFileNameAndExtFromPath(const std::string& FullPath, char* FileName, char* FileExt)
{
	_splitpath_s(FullPath.c_str(), nullptr, 0, nullptr, 0, FileName, MAX_PATH, FileExt, _MAX_EXT);
}

bool CEditorUtil::IsFileExistInDir(const std::string& PathName, const std::string& TargetFileName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (!Info)
	{
		assert(false);
		return  false;
	}

	char FolderName[MAX_PATH] = {};

	strcpy_s(FolderName, Info->PathMultibyte);

	fs::path TargetFolder(FolderName);

	for (const fs::directory_entry& entry :
		fs::recursive_directory_iterator(TargetFolder))
	{
		const std::string& FileName = FilterFileName(entry.path().u8string());

		if (strcmp(TargetFileName.c_str(), FileName.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}

void CEditorUtil::GetAllKindsOfTransformedStringVersions(std::string OriginalString, 
	std::vector<std::string>& vecEachToLower, 
	std::vector<std::string>& vecEachToUpper, 
	std::string& strAllUpper, std::string& strAllLower)
{
	// 모두 대문자
	strAllUpper.resize(OriginalString.size());
	std::transform(OriginalString.begin(), OriginalString.end(), strAllUpper.begin(), ::toupper);

	// 모두 소문자 
	strAllLower.resize(OriginalString.size());
	std::transform(OriginalString.begin(), OriginalString.end(), strAllLower.begin(), ::tolower);

	// 원본 모두 소문자로 바꾸기
	std::transform(OriginalString.begin(), OriginalString.end(), OriginalString.begin(), ::tolower);

	vecEachToUpper.clear();

	// 하나씩 대문자 
	{
		size_t  StrSize = OriginalString.size();

		std::string CpyString = OriginalString;

		for (size_t Index = 0; Index < StrSize; ++Index)
			vecEachToUpper.push_back(CpyString);

		for (size_t Index = 0; Index < StrSize; ++Index)
		{
			// abcd
			// - Abcd
			// - aBcd
			// - abCd
			// - abcD
			vecEachToUpper[Index][Index] = toupper(vecEachToUpper[Index][Index]);
		}
	}

	// 원본 모두 대문자로 바꾸기
	std::transform(OriginalString.begin(), OriginalString.end(), OriginalString.begin(), ::toupper);

	vecEachToLower.clear();

	// 하나씩 소문자
	{
		size_t  StrSize = OriginalString.size();

		std::string CpyString = OriginalString;

		for (size_t Index = 0; Index < StrSize; ++Index)
			vecEachToLower.push_back(CpyString);

		for (size_t Index = 0; Index < StrSize; ++Index)
		{
			// ABCD
			// - aBCD
			// - AbCD
			// - ABcD
			// - ABCd
			vecEachToLower[Index][Index] = tolower(vecEachToLower[Index][Index]);
		}
	}
}

void CEditorUtil::ShowDemo()
{
	static bool Open = false;

	ImGui::ShowDemoWindow(&Open);
}

std::string CEditorUtil::ObjectToString(Object_Type Object)
{
	switch (Object)
	{
	case Object_Type::Player:
		return "Player";
	case Object_Type::Monster:
		return "Monster";
	case Object_Type::MapObject:
		return "MapObject";
	}

	return "";
}

std::string CEditorUtil::SceneComponent3DTypeToString(SceneComponent3DType Type)
{
	switch (Type)
	{
	case SceneComponent3DType::AnimationMeshComponent:
		return "AnimationMeshComponent";
	case SceneComponent3DType::StaticMeshComponent:
		return "StaticMeshComponent";
	case SceneComponent3DType::LandScapeComponent:
		return "LandScapeComponent";
	case SceneComponent3DType::ArmComponent:
		return "ArmComponent";
	case SceneComponent3DType::LightComponent:
		return "LightComponent";
	case SceneComponent3DType::SceneComponent:
		return "SceneComponent";
	case SceneComponent3DType::ParticleComponent:
		return "ParticleComponent";
	case SceneComponent3DType::ColliderBox3D:
		return "ColliderBox3D";
	case SceneComponent3DType::ColliderSphere:
		return "ColliderSphere";
	}

	return "";
}

std::string CEditorUtil::ObjectComponent3DTypeToString(ObjectComponent3DType Type)
{
	switch (Type)
	{
	case ObjectComponent3DType::PaperBurnComponent:
		return "PaperBurnComponent";
	case ObjectComponent3DType::GameStateComponent:
		return "GameStateComponent";
	case ObjectComponent3DType::NavAgent:
		return "NavAgent";
	case ObjectComponent3DType::GameDataComponent:
		return "GameDataComponent";
	case ObjectComponent3DType::PlayerDataComponent:
		return "PlayerDataComponent";
	}

	return "";
}

size_t CEditorUtil::ObjectTypeIndexToTypeid(int TypeIndex)
{
	switch (TypeIndex)
	{
	case 0:
		return typeid(CGameObject).hash_code();
	case 1:
		return typeid(CMovingObject).hash_code();
	case 2:
		return typeid(CMapObject).hash_code();
	case 3:
		return typeid(CChampion).hash_code();
	case 4:
		return typeid(CMinion).hash_code();
	}

	return -1;
}

int CEditorUtil::ObjectTypeIdToTypeIndex(size_t ObjectTypeID)
{
	if (ObjectTypeID == typeid(CGameObject).hash_code())
		return 0;
	else if (ObjectTypeID == typeid(CMovingObject).hash_code())
		return 1;
	else if (ObjectTypeID == typeid(CMapObject).hash_code())
		return 2;
	else if (ObjectTypeID == typeid(CChampion).hash_code())
		return 3;
	else if (ObjectTypeID == typeid(CMinion).hash_code())
		return 4;

	return -1;
}

size_t CEditorUtil::SceneComponentTypeIndexToTypeid(int TypeIndex)
{
	switch (TypeIndex)
	{
	case 0:
		return typeid(CAnimationMeshComponent).hash_code();
	case 1:
		return typeid(CStaticMeshComponent).hash_code();
	case 2:
		return typeid(CLandScape).hash_code();
	case 3:
		return typeid(CArm).hash_code();
	case 4:
		return typeid(CLightComponent).hash_code();
	case 5:
		return typeid(CSceneComponent).hash_code();
	case 6:
		return typeid(CParticleComponent).hash_code();
	case 7:
		return typeid(CColliderBox3D).hash_code();
	case 8:
		return typeid(CColliderSphere).hash_code();
	}

	return -1;
}

// TODO : ObjectComponent 종류 추가될 때 마다 추가
size_t CEditorUtil::ObjectComponentTypeIndexToTypeid(int TypeIndex)
{
	switch (TypeIndex)
	{
	case 0:
		return typeid(CPaperBurnComponent).hash_code();
	case 1:
		return typeid(CGameStateComponent).hash_code();
	case 2:
		return typeid(CNavAgent).hash_code();
	case 3:
		return typeid(CGameDataComponent).hash_code();
	case 4:
		return typeid(CPlayerDataComponent).hash_code();
	}

	return -1;
}

bool CEditorUtil::CompareExt(const char* FullPath, const char ExtFilter[_MAX_EXT])
{
	char FullPathExt[_MAX_EXT] = {};
	char FilterBuf[_MAX_EXT] = {};

	if (ExtFilter[0] == '.')
	{
		strcpy_s(FilterBuf, ExtFilter + 1);
	}
	else
	{
		strcpy_s(FilterBuf, ExtFilter);
	}
	_strupr_s(FilterBuf);

	int ExtLen = (int)strlen(FilterBuf);
	int Len = (int)strlen(FullPath);

	strncpy_s(FullPathExt, FullPath + Len - ExtLen, 4);
	_strupr_s(FullPathExt);

	return (strcmp(FilterBuf, FullPathExt) == 0);
}

