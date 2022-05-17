
#include "AnimationMesh.h"
#include "FBXLoader.h"
#include "../Animation/Skeleton.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CAnimationMesh::CAnimationMesh()
{
	SetTypeID<CAnimationMesh>();
	m_MeshType = Mesh_Type::Animation;
}

CAnimationMesh::~CAnimationMesh()
{
}

// fbx
bool CAnimationMesh::Init()
{
	return true;
}

void CAnimationMesh::SetSkeleton(CSkeleton* Skeleton)
{
	m_Skeleton = Skeleton;
}

void CAnimationMesh::SetSkeleton(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Skeleton = new CSkeleton;

	char	FileNameMultibyte[MAX_PATH] = {};

	int Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FileName, -1, FileNameMultibyte, Length, 0, 0);

	m_Skeleton->LoadSkeleton(m_Scene, Name, FileNameMultibyte, PathName);
}

bool CAnimationMesh::LoadMeshFullPathMultibyte(const char* FullPath)
{
	char    Ext[_MAX_EXT] = {};

	_splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);
	_strupr_s(Ext);

	if (strcmp(Ext, ".FBX") == 0)
	{
		CFBXLoader  Loader;

		if (!Loader.LoadFBX(FullPath, false))
			return false;

		return ConvertFBX(&Loader, FullPath);
	}

	return LoadMeshFile(FullPath);
}

bool CAnimationMesh::ConvertFBXLocalFileMultiByte(const char* FullPath)
{
	char Ext[_MAX_EXT] = {};

	_splitpath_s(FullPath, 0, 0, 0, 0, 0, 0, Ext, _MAX_EXT);
	_strupr_s(Ext);

	// FBX 파일이 아닌 경우
	if (strcmp(Ext, ".FBX") != 0)
	{
		return false;
	}

	CFBXLoader  Loader;

	if (!Loader.LoadFBX(FullPath, false))
	{
		return false;
	}

	return ConvertFBXReleaseSequence(&Loader, FullPath);
}

bool CAnimationMesh::ConvertFBX(CFBXLoader* Loader, const char* FullPath)
{
	if (!CMesh::ConvertFBX(Loader, FullPath))
		return false;

    // 애니메이션 처리
	const std::vector<PFBXBONE>* pvecBone = Loader->GetBones();

	if (pvecBone->empty())
		return true;

	std::string	strSkeletonName = m_Name + "_Skeleton";

	m_Skeleton = new CSkeleton;

	// 본 수만큼 반복한다.
	std::vector<PFBXBONE>::const_iterator	iterB;
	std::vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

	for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
	{
		Bone* pBone = new Bone;

		pBone->strName = (*iterB)->strName;
		pBone->iDepth = (*iterB)->iDepth;
		pBone->iParentIndex = (*iterB)->iParentIndex;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				pBone->matBone[i][j] = (float)(*iterB)->matBone.mData[i].mData[j];
			}
		}

		m_Skeleton->AddBone(pBone);
	}

	// 애니메이션 클립을 추가한다.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = Loader->GetClips();

	// 파일 이름을 제외한 경로를 만들어둔다.
	char	strAnimPath[MAX_PATH] = {};
	strcpy_s(strAnimPath, FullPath);

	int	iLength = (int)strlen(strAnimPath);
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
		if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
		{
			memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// 클립을 읽어온다.
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		m_Scene->GetResource()->LoadAnimationSequence((*iterC)->strName, false, *iterC);

		CAnimationSequence* pSequence = m_Scene->GetResource()->FindAnimationSequence((*iterC)->strName);

		if (!pSequence)
			continue;

		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, FullPath);
		memcpy(&strAnimFullPath[iLength - 3], "sqc", 3);

		pSequence->SaveFullPathMultibyte(strAnimFullPath);
	}

	if (m_Skeleton)
	{
		char	SkeletonPath[MAX_PATH] = {};
		strcpy_s(SkeletonPath, FullPath);
		memcpy(&SkeletonPath[iLength - 3], "bne", 3);
		m_Skeleton->SaveSkeletonFullPath(SkeletonPath);
	}

	return true;
}

bool CAnimationMesh::SaveMesh(FILE* File)
{
	if (!CMesh::SaveMesh(File))
		return false;

	return true;
}

bool CAnimationMesh::LoadMesh(FILE* File)
{
	if (!CMesh::LoadMesh(File))
		return false;

	return true;
}

bool CAnimationMesh::ConvertFBXReleaseSequence(CFBXLoader* Loader, const char* FullPath)
{
	if (!CMesh::ConvertFBX(Loader, FullPath))
		return false;

    // 애니메이션 처리
	const std::vector<PFBXBONE>* pvecBone = Loader->GetBones();

	// Static Mesh의 변환도 Animation Mesh를 만들어서 처리하기 때문에,
	// Bone이나 Clip이 없는 경우에도 리턴하지 않고 진행한다.
	if (!pvecBone->empty())
	{
		std::string	strSkeletonName = m_Name + "_Skeleton";

		m_Skeleton = new CSkeleton;

		// 본 수만큼 반복한다.
		std::vector<PFBXBONE>::const_iterator	iterB;
		std::vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

		for (iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB)
		{
			Bone* pBone = new Bone;

			pBone->strName = (*iterB)->strName;
			pBone->iDepth = (*iterB)->iDepth;
			pBone->iParentIndex = (*iterB)->iParentIndex;

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					pBone->matOffset[i][j] = (float)(*iterB)->matOffset.mData[i].mData[j];
				}
			}

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					pBone->matBone[i][j] = (float)(*iterB)->matBone.mData[i].mData[j];
				}
			}

			m_Skeleton->AddBone(pBone);
		}
	}

	// 애니메이션 클립을 추가한다.
	const std::vector<PFBXANIMATIONCLIP>* pvecClip = Loader->GetClips();

	// 파일 이름을 제외한 경로를 만들어둔다.
	char	strAnimPath[MAX_PATH] = {};
	strcpy_s(strAnimPath, FullPath);

	int	iLength = (int)strlen(strAnimPath);
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb.exe 9개, 2번인덱스 3 ~ 8번까지 제거
		if (strAnimPath[i] == '/' || strAnimPath[i] == '\\')
		{
			memset(&strAnimPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			break;
		}
	}

	// 클립을 읽어온다.
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	std::vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for (iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC)
	{
		CResourceManager::GetInst()->LoadAnimationSequenceConvertFBX((*iterC)->strName, false, *iterC);

		CAnimationSequence* pSequence = CResourceManager::GetInst()->FindAnimationSequence((*iterC)->strName);

		if (!pSequence)
			continue;

		char	strAnimFullPath[MAX_PATH] = {};
		strcpy_s(strAnimFullPath, FullPath);
		iLength = (int)strlen(strAnimFullPath);
		memcpy(&strAnimFullPath[iLength - 3], "sqc", 3);

		pSequence->SaveFullPathMultibyte(strAnimFullPath);

		// 로드 한 후 Release 처리
		CResourceManager::GetInst()->ReleaseAnimationSequence3D((*iterC)->strName);
	}

	if (m_Skeleton)
	{
		char	SkeletonPath[MAX_PATH] = {};
		strcpy_s(SkeletonPath, FullPath);
		iLength = (int)strlen(SkeletonPath);
		memcpy(&SkeletonPath[iLength - 3], "bne", 3);
		m_Skeleton->SaveSkeletonFullPath(SkeletonPath);
	}

	return true;
}
