
#include "AnimationSequenceInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../Resource/Shader/AnimationUpdateConstantBuffer.h"
#include "../Resource/Shader/AnimationUpdateShader.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Device.h"

CAnimationSequenceInstance::CAnimationSequenceInstance() :
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_PlayAnimation(true),
	m_GlobalTime(0.f),
	m_SequenceProgress(0.f),
	m_ChangeTimeAcc(0.f),
	m_ChangeTime(0.2f),
	m_EditorStopAnimation(false),
	m_EditorStopTargetFrame(-1)
{
	SetTypeID<CAnimationSequenceInstance>();
}

CAnimationSequenceInstance::CAnimationSequenceInstance(const CAnimationSequenceInstance& Anim)
{
	m_PlayAnimation = Anim.m_PlayAnimation;

	m_GlobalTime = 0.f;
	m_SequenceProgress = 0.f;
	m_ChangeTimeAcc = 0.f;
	m_ChangeTime = Anim.m_ChangeTime;

	m_AnimationUpdateShader = Anim.m_AnimationUpdateShader;

	if (Anim.m_AnimationUpdateCBuffer)
		m_AnimationUpdateCBuffer = Anim.m_AnimationUpdateCBuffer->Clone();

	if (Anim.m_OutputBuffer)
		m_OutputBuffer = Anim.m_OutputBuffer->Clone();

	if (Anim.m_BoneBuffer)
		m_BoneBuffer = Anim.m_BoneBuffer->Clone();

	m_Skeleton = Anim.m_Skeleton;

	m_EditorStopAnimation = false;

	m_mapAnimation.clear();

	auto	iter = Anim.m_mapAnimation.begin();
	auto	iterEnd = Anim.m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimationSequenceData* Data = new CAnimationSequenceData;

		Data->m_Sequence = iter->second->m_Sequence;
		Data->m_Name = iter->second->m_Name;
		Data->m_Loop = iter->second->m_Loop;
		Data->m_PlayTime = iter->second->m_PlayTime;
		Data->m_PlayScale = iter->second->m_PlayScale;

		if (Anim.m_CurrentAnimation->m_Name == Data->m_Name)
			m_CurrentAnimation = Data;

		m_mapAnimation.insert(std::make_pair(iter->first, Data));
	}
}

CAnimationSequenceInstance::~CAnimationSequenceInstance()
{
	SAFE_DELETE(m_OutputBuffer);
	SAFE_DELETE(m_BoneBuffer);
	SAFE_DELETE(m_AnimationUpdateCBuffer);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimationSequenceInstance::SetEditorStopTargetFrame(int Frame)
{
	if (!m_CurrentAnimation)
		return;

	// 기존에 Stop 해두던 Animation 을 Play 시킨다.
	if (!m_PlayAnimation)
		m_PlayAnimation = true;

	m_EditorStopTargetFrame = Frame;

	// 해당 Frame 이전으로 세팅해둔다.
	// int PrevFrame = Frame - 1;
	// if (PrevFrame < 0)
	//	PrevFrame = m_CurrentAnimation->GetAnimationSequence()->GetEndFrame();

	// SetCurrentAnimationFrameIdx(PrevFrame);
	SetCurrentAnimationFrameIdx(Frame);

	m_EditorStopAnimation = true;
}

void CAnimationSequenceInstance::SetSkeleton(CSkeleton* Skeleton)
{
	m_Skeleton = Skeleton;
}

void CAnimationSequenceInstance::GetAnimationSequenceNames(std::vector<std::string>& VecSequenceNames)
{
	auto iter =  m_mapAnimation.begin();
	auto iterEnd =  m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		VecSequenceNames.push_back(iter->first);
	}
}

void CAnimationSequenceInstance::SetCurrentAnimationFrameIdx(int Idx)
{
	if (!m_CurrentAnimation)
		return;

	// m_GlobalTime += DeltaTime * m_CurrentAnimation->m_PlayScale;
	// float	AnimationTime = m_GlobalTime + m_CurrentAnimation->m_Sequence->m_StartTime;

	// Global Time 도 해당 위치로 맞춰준다.
	m_GlobalTime = (Idx) * m_CurrentAnimation->m_FrameTime;

	m_CurrentAnimation->GetAnimationSequence()->m_CurrentFrameIdx = Idx;
}

bool CAnimationSequenceInstance::EditCurrentSequenceKeyName(const std::string& NewKey, const std::string& PrevKey)
{
	if (!m_CurrentAnimation)
		return false;

	// 다시 Combo Box 같은 것 Refresch
	auto iter = m_mapAnimation.find(PrevKey);

	if (iter == m_mapAnimation.end())
		return false;

	CAnimationSequenceData* PrevAnim = iter->second;

	CAnimationSequenceData* NewAnim = new CAnimationSequenceData;

	NewAnim->m_Sequence = PrevAnim->GetAnimationSequence();
	NewAnim->m_Name = PrevAnim->GetName();
	NewAnim->m_Loop = PrevAnim->m_Loop;
	NewAnim->m_PlayTime = PrevAnim->m_PlayTime;
	NewAnim->m_PlayScale = PrevAnim->m_PlayScale;
	NewAnim->m_FrameTime = PrevAnim->m_FrameTime;

	// 기존 것은 지워준다.
	m_mapAnimation.erase(iter);

	// 새로운 사항을 추가한다.
	m_mapAnimation.insert(std::make_pair(NewKey, NewAnim));

	return true;
}

void CAnimationSequenceInstance::AddAnimation(const std::string& SequenceName,
	const std::string& Name, bool Loop,
	float PlayTime, float PlayScale)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (Anim)
		return;

	CAnimationSequence* Sequence = nullptr;

	if (m_Scene)
	{
		Sequence = m_Scene->GetResource()->FindAnimationSequence(SequenceName);
	}

	else
	{
		Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	}

	if (!Sequence)
		return;

	Anim = new CAnimationSequenceData;

	Anim->m_Sequence = Sequence;
	Anim->m_Name = Name;
	Anim->m_Loop = Loop;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_FrameTime = PlayTime / Sequence->GetKeyFrameCount();

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Anim;
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));
}

void CAnimationSequenceInstance::AddAnimation(const TCHAR* FileName,
	const std::string& PathName, const std::string& Name,
	bool Loop, float PlayTime, float PlayScale)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (Anim)
		return;

	char	FileNameMultibyte[256] = {};

	int	Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, FileName, -1, FileNameMultibyte, Length, 0, 0);

	CAnimationSequence* Sequence = nullptr;

	if (m_Scene)
	{
		std::string	SequenceName;

		m_Scene->GetResource()->LoadAnimationSequenceMultibyte(Loop, SequenceName, FileNameMultibyte);
		Sequence = m_Scene->GetResource()->FindAnimationSequence(SequenceName);
	}

	else
	{
		std::string	SequenceName;

		CResourceManager::GetInst()->LoadAnimationSequenceMultibyte(Loop, SequenceName, FileNameMultibyte);
		Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);
	}

	if (!Sequence)
		return;

	Anim = new CAnimationSequenceData;

	Anim->m_Sequence = Sequence;
	Anim->m_Name = Name;
	Anim->m_Loop = Loop;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_FrameTime = PlayTime / Sequence->GetKeyFrameCount();

	if (m_mapAnimation.empty())
	{
		m_CurrentAnimation = Anim;
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));
}

void CAnimationSequenceInstance::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayTime = PlayTime;
}

void CAnimationSequenceInstance::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_PlayScale = PlayScale;
}

void CAnimationSequenceInstance::SetLoop(const std::string& Name, bool Loop)
{
	CAnimationSequenceData* Anim = FindAnimation(Name);

	if (!Anim)
		return;

	Anim->m_Loop = Loop;
}

void CAnimationSequenceInstance::SetCurrentAnimation(const std::string& Name)
{
	m_CurrentAnimation = FindAnimation(Name);

	if (!m_CurrentAnimation)
		return;

	m_CurrentAnimation->m_Time = 0.f;

	size_t Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}
}

void CAnimationSequenceInstance::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->m_Name == Name)
		return;

	//m_CurrentAnimation->m_Time = 0.f;

	size_t	Size = m_CurrentAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->m_vecNotify[i]->Call = false;
	}

	m_ChangeAnimation = FindAnimation(Name);
	m_ChangeAnimation->m_Time = 0.f;
}

void CAnimationSequenceInstance::ClearAnimationSequence()
{	
	// 현재 Animation은 null로
	m_CurrentAnimation = nullptr;

	// 나머지는 모두 지워주기
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		// SAFE_DELETE(iter->second);
		SAFE_DELETE(iter->second);
	}
	m_mapAnimation.clear();
}

void CAnimationSequenceInstance::GatherSequenceNames(std::vector<std::string>& vecString)
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		vecString.push_back(iter->first);
	}
}

void CAnimationSequenceInstance::AddAnimationSequenceToSceneResource()
{
	CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		
	}
}

int CAnimationSequenceInstance::GetCurrentAnimationOrder()
{
	if (!m_CurrentAnimation)
		return -1;

	int Idx = 0;

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter->second == m_CurrentAnimation)
			break;
		Idx += 1;
	}

	return Idx;
	return 0;
}

void CAnimationSequenceInstance::Start()
{
	if (m_Scene)
	{
		m_AnimationUpdateShader = (CComputeShader*)m_Scene->GetResource()->FindShader("AnimationUpdateShader");
	}

	else
	{
		m_AnimationUpdateShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("AnimationUpdateShader");
	}

	m_AnimationUpdateCBuffer = new CAnimationUpdateConstantBuffer;

	m_AnimationUpdateCBuffer->Init();

	m_vecBoneMatrix.resize(m_Skeleton->GetBoneCount());

	m_OutputBuffer = new CStructuredBuffer;

	m_OutputBuffer->Init("OutputBone", sizeof(Matrix),
		(unsigned int)m_Skeleton->GetBoneCount(), 0);

	m_BoneBuffer = new CStructuredBuffer;

	m_BoneBuffer->Init("OutputBone", sizeof(Matrix),
		(unsigned int)m_Skeleton->GetBoneCount(), 1,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		D3D11_CPU_ACCESS_READ, false);
}

bool CAnimationSequenceInstance::Init()
{
	return true;
}

void CAnimationSequenceInstance::Update(float DeltaTime)
{
	if (!m_Skeleton || !m_CurrentAnimation || !m_PlayAnimation || m_CurrentAnimation->m_Sequence->GetKeyFrameCount() == 0)
		return;

	m_GlobalTime += DeltaTime * m_CurrentAnimation->m_PlayScale;

	bool	Change = false;
	bool	ChangeEnd = false;
	bool	AnimEnd = false;

	if (m_ChangeAnimation)
	{
		Change = true;
		m_ChangeTimeAcc += DeltaTime;

		if (m_ChangeTimeAcc >= m_ChangeTime)
		{
			m_ChangeTimeAcc = m_ChangeTime;
			ChangeEnd = true;
		}
	}

	else
	{
		m_SequenceProgress = m_GlobalTime / m_CurrentAnimation->m_PlayTime;

		if (m_GlobalTime >= m_CurrentAnimation->m_PlayTime)
		{
			//m_GlobalTime -= m_CurrentAnimation->m_PlayTime;
			m_GlobalTime = m_CurrentAnimation->m_PlayTime;

			AnimEnd = true;
		}

		m_AnimationUpdateCBuffer->SetChangeAnimation(false);
		m_AnimationUpdateCBuffer->SetChangeRatio(0.f);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(0);

		float	AnimationTime = m_GlobalTime + m_CurrentAnimation->m_Sequence->m_StartTime;

		int	StartFrame = 0;
		int	EndFrame = m_CurrentAnimation->m_Sequence->m_FrameLength;

		// 수정 전 코드
		//int	FrameIndex = (int)(AnimationTime / m_CurrentAnimation->m_Sequence->m_FrameTime);
		// 수정자 : 이도경 / 내용 : AnimationSequence가 아닌 AnimationSequenceInstance의 멤버 사용하는 것으로 수정
		// 날짜 : 22.05.02
		int	FrameIndex = (int)(AnimationTime / m_CurrentAnimation->m_FrameTime);

		m_CurrentAnimation->GetAnimationSequence()->m_CurrentFrameIdx = FrameIndex;

		// Editor 상에서 의도한 위치의 Frame 에서 멈추게 한다.
		if (m_EditorStopAnimation && FrameIndex == m_EditorStopTargetFrame)
		{
			m_PlayAnimation = false;
		}

		int	NextFrameIndex = FrameIndex + 1;

		if (FrameIndex >= m_CurrentAnimation->m_Sequence->m_FrameLength)
			FrameIndex = m_CurrentAnimation->m_Sequence->m_FrameLength - 1;

		if (NextFrameIndex >= EndFrame)
			NextFrameIndex = StartFrame;

		// 수정 전 코드
		//float	Ratio = (AnimationTime - m_CurrentAnimation->m_Sequence->m_FrameTime * FrameIndex) / m_CurrentAnimation->m_Sequence->m_FrameTime;
		// 수정자 : 이도경 / 내용 : AnimationSequence가 아닌 AnimationSequenceInstance의 멤버 사용하는 것으로 수정
		// 날짜 : 22.05.02
		float	Ratio = (AnimationTime - m_CurrentAnimation->m_FrameTime * FrameIndex) / m_CurrentAnimation->m_FrameTime;

		// 수정자 : 이도경 / 내용 : 예외처리
		// 날짜 : 22.05.02
		if (NextFrameIndex >= m_CurrentAnimation->m_Sequence->m_FrameLength)
			NextFrameIndex = 0;

		m_AnimationUpdateCBuffer->SetFrameCount(EndFrame);
		m_AnimationUpdateCBuffer->SetCurrentFrame(FrameIndex);
		m_AnimationUpdateCBuffer->SetNextFrame(NextFrameIndex);
		m_AnimationUpdateCBuffer->SetRatio(Ratio);
		m_AnimationUpdateCBuffer->SetBoneCount((int)m_Skeleton->GetBoneCount());

		size_t	Size = m_CurrentAnimation->m_vecNotify.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (!m_CurrentAnimation->m_vecNotify[i]->Call &&
				m_CurrentAnimation->m_vecNotify[i]->Frame == FrameIndex)
			{
				m_CurrentAnimation->m_vecNotify[i]->Call = true;
				m_CurrentAnimation->m_vecNotify[i]->Function();
			}
		}

		if (AnimEnd)
		{
			if (m_CurrentAnimation->m_EndFunction)
				m_CurrentAnimation->m_EndFunction();

			if (m_CurrentAnimation->m_Loop)
			{
				m_GlobalTime = 0.f;
				size_t	Size = m_CurrentAnimation->m_vecNotify.size();

				for (size_t i = 0; i < Size; ++i)
				{
					m_CurrentAnimation->m_vecNotify[i]->Call = false;
				}
			}
		}
	}

	if (Change)
	{
		m_AnimationUpdateCBuffer->SetChangeRatio(m_ChangeTimeAcc / m_ChangeTime);
		m_AnimationUpdateCBuffer->SetChangeAnimation(true);
		m_AnimationUpdateCBuffer->SetChangeFrameCount(m_ChangeAnimation->m_Sequence->m_FrameLength);

		m_ChangeAnimation->m_Sequence->SetChangeShader();
	}

	m_AnimationUpdateCBuffer->SetRowIndex(0);

	m_OutputBuffer->SetShader();

	m_AnimationUpdateCBuffer->UpdateCBuffer();

	m_Skeleton->SetShader();

	m_BoneBuffer->SetShader();

	m_CurrentAnimation->m_Sequence->SetShader();

	unsigned int GroupX = (unsigned int)m_Skeleton->GetBoneCount() / 256 + 1;

	m_AnimationUpdateShader->Excute(GroupX, 1, 1);

	m_BoneBuffer->ResetShader();

	m_CurrentAnimation->m_Sequence->ResetShader();

	m_Skeleton->ResetShader();

	m_OutputBuffer->ResetShader();

	if (Change)
	{
		m_ChangeAnimation->m_Sequence->ResetChangeShader();
	}

	if (ChangeEnd)
	{
		m_CurrentAnimation = m_ChangeAnimation;
		m_ChangeAnimation = nullptr;
		m_ChangeTimeAcc = 0.f;
		m_GlobalTime = 0.f;
	}

 //	// 구조화 버퍼에 있는 본 정보를 DataBuffer로 복사한다.
 //	CDevice::GetInst()->GetContext()->CopyResource(m_BoneDataBuffer, m_BoneBuffer->GetBuffer());

	D3D11_MAPPED_SUBRESOURCE	Map = {};

	// Usage가 Default임에도 불구하고 Cpu에서 Read Access를 가질 수 있는 버그를 이용
	CDevice::GetInst()->GetContext()->Map(m_BoneBuffer->GetBuffer(), 0, D3D11_MAP_READ, 0, &Map);

	memcpy(&m_vecBoneMatrix[0], Map.pData, sizeof(Matrix) * m_vecBoneMatrix.size());;

	CDevice::GetInst()->GetContext()->Unmap(m_BoneBuffer->GetBuffer(), 0);

	m_Skeleton->Update(DeltaTime, m_vecBoneMatrix, m_Owner->GetWorldMatrix());
}

void CAnimationSequenceInstance::SetShader()
{
	m_OutputBuffer->SetShader(106, (int)Buffer_Shader_Type::Vertex);
}

void CAnimationSequenceInstance::ResetShader()
{
	m_OutputBuffer->ResetShader(106, (int)Buffer_Shader_Type::Vertex);
}

CAnimationSequenceInstance* CAnimationSequenceInstance::Clone()
{
	return new CAnimationSequenceInstance(*this);
}

void CAnimationSequenceInstance::Save(FILE* File)
{
	int	AnimCount = (int)m_mapAnimation.size();
	fwrite(&AnimCount, sizeof(int), 1, File);

	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		int	Length = (int)iter->first.length();
		fwrite(&Length, sizeof(int), 1, File);
		fwrite(iter->first.c_str(), sizeof(char), Length, File);

		iter->second->Save(File);
	}

	const std::string& CurrentName = m_CurrentAnimation->GetName();
	int	Length = (int)CurrentName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(CurrentName.c_str(), sizeof(char), Length, File);

	fwrite(&m_PlayAnimation, sizeof(bool), 1, File);
}

void CAnimationSequenceInstance::Load(FILE* File)
{
	int	AnimCount = 0;
	fread(&AnimCount, sizeof(int), 1, File);

	for (int i = 0; i < AnimCount; ++i)
	{
		int	Length = 0;
		char	AnimName[256] = {};

		fread(&Length, sizeof(int), 1, File);
		fread(AnimName, sizeof(char), Length, File);

		CAnimationSequenceData* Data = new CAnimationSequenceData;

		Data->Load(File);

		if (m_Scene)
		{
			Data->m_Sequence = m_Scene->GetResource()->FindAnimationSequence(Data->m_SequenceName);
		}

		else
		{
			Data->m_Sequence = CResourceManager::GetInst()->FindAnimationSequence(Data->m_SequenceName);
		}

		m_mapAnimation.insert(std::make_pair(AnimName, Data));
	}

	int	Length = 0;
	char	CurrentName[256] = {};

	fread(&Length, sizeof(int), 1, File);
	fread(CurrentName, sizeof(char), Length, File);

	m_CurrentAnimation = FindAnimation(CurrentName);

	fread(&m_PlayAnimation, sizeof(bool), 1, File);


	//if (m_Scene)
	//	m_CBuffer = m_Scene->GetResource()->GetAnimation2DCBuffer();
}

bool CAnimationSequenceInstance::SaveAnimationFullPath(const char* FullPath)
{
	FILE* pFile;
	fopen_s(&pFile, FullPath, "wb");
	if (!pFile)
		return false;

	fwrite(&m_TypeID, sizeof(size_t), 1, pFile);
	fwrite(&m_PlayAnimation, sizeof(bool), 1, pFile);

	int Length = (int)m_AnimInstanceName.length();
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_AnimInstanceName.c_str(), sizeof(char), Length, pFile);

	size_t AnimationSize = m_mapAnimation.size();
	fwrite(&AnimationSize, sizeof(size_t), 1, pFile);

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();
	size_t  SequenceDataNameLength = -1;
	for (; iter != iterEnd; ++iter)
	{
		SequenceDataNameLength = strlen(iter->first.c_str());
		fwrite(&SequenceDataNameLength, sizeof(int), 1, pFile);

		// Animation Key 저장
		fwrite(iter->first.c_str(), sizeof(char), SequenceDataNameLength, pFile);
		iter->second->Save(pFile);
	}

	// Current Animation
	bool CurrentAnimEnable = false;
	if (m_CurrentAnimation)
		CurrentAnimEnable = true;
	fwrite(&CurrentAnimEnable, sizeof(bool), 1, pFile);

	if (m_CurrentAnimation)
	{
		// Current Anim Length
		Length = (int)m_CurrentAnimation->m_Name.length();
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_CurrentAnimation->m_Name.c_str(), sizeof(char), Length, pFile);
	}

	fclose(pFile);

	return true;
}

bool CAnimationSequenceInstance::LoadAnimationFullPath(const char* FullPath)
{
	FILE* pFile;

	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return false;

	fread(&m_TypeID, sizeof(size_t), 1, pFile);
	fread(&m_PlayAnimation, sizeof(bool), 1, pFile);

	int Length = 0;
	fread(&Length, sizeof(int), 1, pFile);
	char Name[MAX_PATH] = {};
	fread(Name, sizeof(char), Length, pFile);
	m_AnimInstanceName = Name;

	size_t AnimationSize = -1;
	fread(&AnimationSize, sizeof(size_t), 1, pFile);


	// m_mapAnimation.clear();
	// m_CurrentAnimation = nullptr;

	for (int i = 0; i < AnimationSize; i++)
	{
		int SequenceDataKeyNameLength = 0;
		char SequenceDataNameKey[MAX_PATH] = {};

		// - CAnimationSequence2DData
		// Key Name 저장 
		fread(&SequenceDataKeyNameLength, sizeof(int), 1, pFile);
		fread(SequenceDataNameKey, sizeof(char), SequenceDataKeyNameLength, pFile);

		// CAnimationSequence2DData 를 저장하기 ===============================
		CAnimationSequenceData* SequenceData = new CAnimationSequenceData;

		SequenceData->Load(pFile);
		// Scene이 있냐 없냐에 따라
		// Scene Resource, ResourceManager에 해당 Sequence를 추가해주어야 한다
		/*
		if (m_Scene)
		{
			m_Scene->GetResource()->AddSequence2D(Sequence2DData->m_Sequence);
		}
		else
		{
			CResourceManager::GetInst()->AddSequence2D(Sequence2DData->m_Sequence);
		}
		*/

		// 중복 추가 방지
		if (FindAnimation(SequenceDataNameKey))
		{
			SAFE_DELETE(SequenceData);
			continue;
		}

		if (m_mapAnimation.empty())
		{
			m_CurrentAnimation = SequenceData;
		}

		m_mapAnimation.insert(std::make_pair(SequenceDataNameKey, SequenceData));

	}

	// Current Animation Info
	bool CurrentAnimEnable = false;
	fread(&CurrentAnimEnable, sizeof(bool), 1, pFile);

	if (CurrentAnimEnable)
	{
		int CurAnimNameLength = -1;
		fread(&CurAnimNameLength, sizeof(int), 1, pFile);
		char CurAnimName[MAX_PATH] = {};
		fread(CurAnimName, sizeof(char), CurAnimNameLength, pFile);

		CAnimationSequenceData* FoundAnimation = FindAnimation(CurAnimName);

		// 이름 자체가 잘못 저장된 것일 수도 있어서 해당 내용이 없을 수도 있다.
		if (FoundAnimation)
		{
			m_CurrentAnimation = FoundAnimation;
		}
	}

	fclose(pFile);

	return true;
}

CAnimationSequenceData* CAnimationSequenceInstance::FindAnimation(const std::string& Name)
{
	auto	iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}

