
#include "StateComponent.h"
#include "BehaviorTree.h"
#include "State.h"

CStateComponent::CStateComponent()	:
	m_BehaviorTree(nullptr)
{
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
	return nullptr;
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
