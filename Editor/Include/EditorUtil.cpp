
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
#include "Component/StateComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
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

void CEditorUtil::FilterSpecificNameIncludedFilePath(std::vector<std::string>& InputVecFullPath, std::vector<std::string>& ReturnVecFullPath, 
	const char* IncludeName)
{
	ReturnVecFullPath.clear();
	ReturnVecFullPath.reserve(InputVecFullPath.size());

	size_t InputSize = InputVecFullPath.size();
	
	for (size_t i = 0; i < InputSize; ++i)
	{
		// Common Name 을 포함하고 있지 않다면 Skips
		size_t nPos = InputVecFullPath[i].find(IncludeName);

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

void CEditorUtil::ShowDemo()
{
	static bool Open = false;

	ImGui::ShowDemoWindow(&Open);
}

std::string CEditorUtil::LoLObjectToString(LoLObject Object)
{
	switch (Object)
	{
	case LoLObject::GameObject:
		return "GameObject";
	case LoLObject::MovingObject:
		return "MovingObject";
	case LoLObject::MapObject:
		return "MapObject";
	case LoLObject::Champion:
		return "Champion";
	case LoLObject::Minion:
		return "Minion";
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
	case ObjectComponent3DType::StateComponent:
		return "StateComponent";
	case ObjectComponent3DType::NavAgent:
		return "NavAgent";
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

size_t CEditorUtil::ObjectComponentTypeIndexToTypeid(int TypeIndex)
{
	switch (TypeIndex)
	{
	case 0:
		return typeid(CPaperBurnComponent).hash_code();
	case 1:
		return typeid(CStateComponent).hash_code();
	case 2:
		return typeid(CNavAgent).hash_code();
	//case 3:
	//	return typeid(CArm).hash_code();
	//case 4:
	//	return typeid(CLightComponent).hash_code();
	//case 5:
	//	return typeid(CSceneComponent).hash_code();
	}

	return -1;
}

bool CEditorUtil::CompareExt(const char* FullPath, const char ExtFilter[_MAX_EXT])
{
	char FullPathExt[_MAX_EXT] = {};
	char FilterBuf[_MAX_EXT] = {};

	strcpy_s(FilterBuf, ExtFilter);
	_strupr_s(FilterBuf);

	int Len = strlen(FullPath);

	strncpy_s(FullPathExt, FullPath + Len - 4, 4);
	_strupr_s(FullPathExt);

	return (strcmp(FilterBuf, FullPathExt) == 0);
}

std::string LoLObjectToString(LoLObject Object)
{
	switch (Object)
	{
	case LoLObject::GameObject:
		return "GameObject";
	case LoLObject::MovingObject:
		return "MovingObject";
	case LoLObject::MapObject:
		return "MapObject";
	case LoLObject::Champion:
		return "Champion";
	case LoLObject::Minion:
		return "Minion";
	}

	return "";
}
