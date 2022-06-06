
#include "StateComponent.h"
#include "AnimationMeshComponent.h"
#include "BehaviorTree.h"
#include "State.h"
#include "../PathManager.h"
#include "../GameObject/GameObject.h"

CStateComponent::CStateComponent()	:
	m_BehaviorTree(nullptr),
	m_TreeUpdate(false)
{
	SetTypeID<CStateComponent>();
	m_ComponentType = Component_Type::ObjectComponent;

	m_BehaviorTree = new CBehaviorTree;
	m_BehaviorTree->m_Owner = this;
}

CStateComponent::CStateComponent(const CStateComponent& com)	:
	CObjectComponent(com)
{

}

CStateComponent::~CStateComponent()
{
	SAFE_DELETE(m_BehaviorTree);
}

bool CStateComponent::Init()
{
	m_BehaviorTree->Init();

	return true;
}

void CStateComponent::Start()
{
	m_BehaviorTree->Start();
}

void CStateComponent::Update(float DeltaTime)
{
	auto iter = m_StateList.begin();
	auto iterEnd = m_StateList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Update(DeltaTime);
	}

	if(m_TreeUpdate)
		m_BehaviorTree->Update(DeltaTime);
}

void CStateComponent::PostUpdate(float DeltaTime)
{
	auto iter = m_StateList.begin();
	auto iterEnd = m_StateList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PostUpdate(DeltaTime);
	}

	if (m_TreeUpdate)
		m_BehaviorTree->PostUpdate(DeltaTime);

}

void CStateComponent::PrevRender()
{
}

void CStateComponent::Render()
{
}

void CStateComponent::PostRender()
{
}

CStateComponent* CStateComponent::Clone()
{
	return new CStateComponent(*this);
}

bool CStateComponent::Save(FILE* File)
{
	CComponent::Save(File);

	m_BehaviorTree->Save(File);

	return true;
}

bool CStateComponent::Save(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return false;

	Save(File);

	fclose(File);

	return true;
}

bool CStateComponent::Save(const char* FileName, const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	if (!Save(FullPath))
		return false;

	return true;
}

bool CStateComponent::Load(const char* FileName, const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	if (!Load(FullPath))
		return false;

	return true;
}

bool CStateComponent::Load(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return false;

	Load(File);

	fclose(File);

	return true;
}


bool CStateComponent::Load(FILE* File)
{
	CComponent::Load(File);

	CAnimationMeshComponent* AnimMeshComp = m_Object->FindComponentFromType<CAnimationMeshComponent>();
	m_BehaviorTree->SetAnimationMeshComponent(AnimMeshComp);

	m_BehaviorTree->Load(File);

	return true;
}

void CStateComponent::SetAnimationMeshComponent(CAnimationMeshComponent* Mesh)
{
	m_AnimationMeshComp = Mesh;
	
	if (m_BehaviorTree)
		m_BehaviorTree->SetAnimationMeshComponent(m_AnimationMeshComp);
}

CAnimationMeshComponent* CStateComponent::GetAnimationMeshComp() const
{
	return m_AnimationMeshComp;
}

void CStateComponent::AddState(CState* State)
{
	m_StateList.push_back(State);

	State->OnStart();
}

void CStateComponent::AddState(const std::string& Name)
{
	CState* State = FindState(Name);

	if (!State)
		return;

	m_StateList.push_back(State);

	State->OnStart();
}

bool CStateComponent::DeleteState(CState* State)
{
	auto iter = m_StateList.begin();
	auto iterEnd = m_StateList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == State)
		{
			m_StateList.erase(iter);
			State->OnEnd();
			return true;
		}
	}

	return false;
}

bool CStateComponent::DeleteState(const std::string& Name)
{
	CState* State = FindState(Name);

	if (!State)
		return false;

	auto iter = m_StateList.begin();
	auto iterEnd = m_StateList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == State)
		{
			m_StateList.erase(iter);
			State->OnEnd();
			return true;
		}
	}

	return false;
}

CState* CStateComponent::FindState(const std::string& Name)
{
	auto iter = m_StateList.begin();
	auto iterEnd = m_StateList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return (*iter);
	}

	return nullptr;
}
