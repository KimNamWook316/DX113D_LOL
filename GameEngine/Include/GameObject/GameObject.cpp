
#include "GameObject.h"
#include "../Scene/SceneManager.h"
#include "../PathManager.h"
#include "../Component/NavAgent.h"

CGameObject::CGameObject() :
	m_Scene(nullptr),
	m_Parent(nullptr),
	m_LifeSpan(0.f),
	m_NavAgent(nullptr)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;

	m_RefCount = 0;

	if (obj.m_RootComponent)
	{
		m_RootComponent = obj.m_RootComponent->Clone();

		m_RootComponent->SetGameObject(this);

		m_RootComponent->SetSceneComponent(this);
	}

	m_vecObjectComponent.clear();

	size_t	Size = obj.m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent.push_back(obj.m_vecObjectComponent[i]->Clone());

		m_vecObjectComponent[i]->SetGameObject(this);
	}
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetScene(CScene* Scene)
{
	m_Scene = Scene;
}

void CGameObject::Destroy()
{
	CRef::Destroy();

	if (m_RootComponent)
		m_RootComponent->Destroy();

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Destroy();
	}
}

void CGameObject::AddChildObject(CGameObject* Obj)
{
	Obj->m_Parent = this;
	m_vecChildObject.push_back(Obj);
}

void CGameObject::DeleteObj()
{
	if (!m_Parent)
	{
		// 루트노드를 지우는 경우, 그냥 모두 Destroy
		Destroy();

		return;
	}

	m_Parent->DeleteChildObj(m_Name);
}

bool CGameObject::DeleteChildObj(const std::string& Name)
{
	size_t	Size = m_vecChildObject.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecChildObject[i]->GetName() == Name)
		{
			CGameObject* DeleteTarget = m_vecChildObject[i];

			auto	iter = m_vecChildObject.begin() + i;

			if ((*iter)->m_vecChildObject.size() > 0)
			{
				auto FirstChildIter = (*iter)->m_vecChildObject.begin();

				CGameObject* FirstChildObj = (*iter)->m_vecChildObject[0];

				(*iter)->m_vecChildObject.erase(FirstChildIter);

				m_vecChildObject[i] = nullptr;

				FirstChildObj->m_Parent = this;
				m_vecChildObject[i] = FirstChildObj;
			}

			else
				m_vecChildObject.erase(iter);

			m_Scene->EraseObjFromList(DeleteTarget);

			return true;
		}

		if (m_vecChildObject[i]->DeleteChildObj(Name))
			return true;
	}

	return false;
}

void CGameObject::ClearParent()
{
	if (!m_Parent)
		return;

	size_t Count = m_Parent->m_vecChildObject.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_Parent->m_vecChildObject[i] == this)
		{
			auto iter = m_Parent->m_vecChildObject.begin();
			std::advance(iter, i);

			m_Parent->m_vecChildObject.erase(iter);
			break;
		}
	}

	m_Parent = nullptr;
}

bool CGameObject::DeleteObjectComponent(const std::string& Name)
{
	size_t Count = m_vecObjectComponent.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecObjectComponent[i]->GetName() == Name)
		{
			auto iter = m_vecObjectComponent.begin();
			std::advance(iter, i);

			m_vecObjectComponent.erase(iter);
			return true;
		}
	}

	return false;
}

CComponent* CGameObject::FindComponent(const std::string& Name)
{
	{
		auto	iter = m_SceneComponentList.begin();
		auto	iterEnd = m_SceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}
	}

	{
		auto	iter = m_vecObjectComponent.begin();
		auto	iterEnd = m_vecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}
	}

	return nullptr;
}

void CGameObject::GetAllSceneComponentsName(std::vector<FindComponentName>& vecNames)
{
	if (!m_RootComponent)
		return;

	m_RootComponent->GetAllSceneComponentsName(vecNames);
}

void CGameObject::GetAllSceneComponentsPointer(std::vector<CSceneComponent*>& OutVecSceneComp)
{
	if (!m_RootComponent)
	{
		return;
	}

	m_RootComponent->GetAllSceneComponentsPointer(OutVecSceneComp);
}

void CGameObject::GetAllObjectComponentsPointer(std::vector<CObjectComponent*>& OutVecObjComp)
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		OutVecObjComp.push_back(m_vecObjectComponent[i]);
	}
}

void CGameObject::Start()
{
	if (m_RootComponent)
		m_RootComponent->Start();

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Start();
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	if (m_LifeSpan > 0.f)
	{
		m_LifeSpan -= DeltaTime;

		if (m_LifeSpan <= 0.f)
		{
			Destroy();
			return;
		}
	}

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Update(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->Update(DeltaTime);
}

void CGameObject::PostUpdate(float DeltaTime)
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostUpdate(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->PostUpdate(DeltaTime);
}

void CGameObject::AddCollision()
{
	if (m_RootComponent)
		m_RootComponent->CheckCollision();
}

void CGameObject::PrevRender()
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PrevRender();
	}

	if (m_RootComponent)
		m_RootComponent->PrevRender();
}

void CGameObject::Render()
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Render();
	}

	if (m_RootComponent)
		m_RootComponent->Render();
}

void CGameObject::PostRender()
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostRender();
	}

	if (m_RootComponent)
		m_RootComponent->PostRender();
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

bool CGameObject::Save(FILE* File)
{
	CRef::Save(File);

	if (m_RootComponent)
	{
		bool	Root = true;
		fwrite(&Root, sizeof(bool), 1, File);

		size_t	TypeID = m_RootComponent->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_RootComponent->Save(File);
	}

	else
	{
		bool	Root = false;
		fwrite(&Root, sizeof(bool), 1, File);
	}

	int	ObjComponentCount = (int)m_vecObjectComponent.size();

	fwrite(&ObjComponentCount, sizeof(int), 1, File);

	for (int i = 0; i < ObjComponentCount; ++i)
	{
		size_t	TypeID = m_vecObjectComponent[i]->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_vecObjectComponent[i]->Save(File);
	}

	return true;
}

bool CGameObject::Load(FILE* File)
{
	CRef::Load(File);

	bool	Root = false;
	fread(&Root, sizeof(bool), 1, File);

	if (Root)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		CSceneManager::GetInst()->CallCreateComponent(this, TypeID);

		if (!m_RootComponent->Load(File))
			return false;
	}

	int	ObjComponentCount = 0;

	fread(&ObjComponentCount, sizeof(int), 1, File);

	for (int i = 0; i < ObjComponentCount; ++i)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		CComponent* Component = CSceneManager::GetInst()->CallCreateComponent(this, TypeID);

		if (!Component->Load(File))
			return false;

		m_vecObjectComponent.push_back((CObjectComponent*)Component);
	}

	return true;
}

bool CGameObject::Save(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return false;

	Save(File);

	fclose(File);

	return true;
}

bool CGameObject::Load(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return false;

	if (!Load(File))
		return false;

	fclose(File);

	return true;
}

bool CGameObject::Save(const char* FileName, const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	Save(FullPath);

	return true;
}

bool CGameObject::Load(const char* FileName, const std::string& PathName)
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

void CGameObject::Move(const Vector3& EndPos)
{
	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecObjectComponent[i]->CheckType<CNavAgent>())
		{
			((CNavAgent*)m_vecObjectComponent[i].Get())->Move(EndPos);
			break;
		}
	}
}

void CGameObject::AddPath(const Vector3& TargetPos)
{
	if (m_NavAgent)
	{
		m_NavAgent->AddTargetPos(TargetPos);
	}
}

void CGameObject::SetNavManagerLandScape(CLandScape* LandScape)
{
	m_Scene->GetNavigation3DManager()->SetLandScape(LandScape);
}

void CGameObject::SetUpdateByMat(bool UpdateByMat)
{
	m_RootComponent->SetUpdateByMat(UpdateByMat);
}

void CGameObject::DecomposeWorld()
{
	m_RootComponent->DecomposeWorld();
}

void CGameObject::SetTransformByWorldMatrix(const Matrix& matTRS)
{
	m_RootComponent->SetTransformByWorldMatrix(matTRS);
}

void CGameObject::AddWorldPosByLocalAxis(AXIS Axis, float Amount)
{
	m_RootComponent->AddWorldPosByLocalAxis(Axis, Amount);
}

void CGameObject::AddWorldPosByLocalAxis(const Vector3& Pos)
{
	m_RootComponent->AddWorldPosByLocalAxis(Pos);
}

