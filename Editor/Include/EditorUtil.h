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

	// 디렉토리 이름을 m_Name으로 가지고 있는 Tree를 첫번째 인자로 주면 Root부터 그 디렉토리 경로까지 모든 디렉토리를 뽑아준다
	// (ex.Bin\\Texture\\Monster 경로면 list에 Bin, Texture, Monster를 넣어준다)
	static void GetFullPathDirectory(class CIMGUITree* CurrentDir, std::list<std::string>& Output);

	// 디렉토리 이름들을 list로 넘겨주면 Root Full경로와 더해서 최종 풀경로를 만들어준다
	// (ex. list에 Texture, Monster라는 이름의 TreeNode가 있으면, [Editor 프로젝트까지 로컬 풀경로]\Editor\Bin\Texture\Monster 를 만들어준다)
	static std::string MergeFullPath(const std::list<std::string> DirNames);

	// Imgui Demo Window를 띄워주는 함수
	static void ShowDemo();

	static std::string LoLObjectToString(LoLObject Object);

	static std::string SceneComponent3DTypeToString(SceneComponent3DType Type);
	static std::string ObjectComponent3DTypeToString(ObjectComponent3DType Type);

	static size_t ObjectTypeIndexToTypeid(int TypeIndex);
	static int ObjectTypeIdToTypeIndex(size_t ObjectTypeID);

	static size_t SceneComponentTypeIndexToTypeid(int TypeIndex);
	static size_t ObjectComponentTypeIndexToTypeid(int TypeIndex);

public:
	static bool CompareExt(const char* FullPath, const char ExtFilter[_MAX_EXT]);

};

namespace AnimationClipInfoKeys
{
	const std::string FrameRange = "Frame Range";
	const std::string FrameLength = "Frame Length";
	const std::string FrameMode = "Frame Mode"; // 24, 30, 60
	const std::string PlayTime = "Play Time";
	const std::string FrameTime = "Frame Time";
	const std::string PlayScale = "Play Scale";
	const std::string AnimSeqKey = "Seq Key";
	const std::string AnimSeqFileName = "Seq File Name";
};