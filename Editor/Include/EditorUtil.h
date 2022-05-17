#pragma once

#include "GameInfo.h"
#include "Flag.h"
#include "EditorInfo.h"

class CEditorUtil
{
public:
	// 풀경로를 넣어주면 파일 이름만 걸러서 리턴해줌
	static std::string FilterFileName(const std::string& FullPath);

	// 해당 경로에 존재하는 모든 파일 이름과 디렉토리 이름들을 가져옴
	static void GetAllFilenames(const std::string& PathName, std::vector<std::string>& vecFileNames,
		std::vector<std::string>& vecDirNames);
	static void GetAllFilenamesFullPath(const std::string& FullPath, std::vector<std::string>& vecFileNames,
		std::vector<std::string>& vecDirNames);

	// 해당 디렉토리에 존재하고, 확장자와 일치하는 파일들의 풀 경로를 리턴함
	static void GetAllFileFullPathInDir(const char* TargetDir, std::vector<std::string>& OutVecFullPath,
		const char* ExtFilter = nullptr);

	// Imgui Demo Window를 띄워주는 함수
	static void ShowDemo();

	static std::string LoLObjectToString(LoLObject Object);

	static std::string SceneComponent3DTypeToString(SceneComponent3DType Type);
	static std::string ObjectComponent3DTypeToString(ObjectComponent3DType Type);

	static size_t ObjectTypeIndexToTypeid(int TypeIndex);
	static size_t SceneComponentTypeIndexToTypeid(int TypeIndex);
	static size_t ObjectComponentTypeIndexToTypeid(int TypeIndex);

private:
	static bool CompareExt(const char* FullPath, const char ExtFilter[_MAX_EXT]);

};