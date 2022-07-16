
#include "ObjectDataComponent.h"
#include "../DataManager.h"
#include "GameObject/GameObject.h"
#include "Resource/Excel/ExcelData.h"
#include "../EditorManager.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Engine.h"

#include <sstream>

CObjectDataComponent::CObjectDataComponent()
{
	SetTypeID<CObjectDataComponent>();
	m_ComponentType = Component_Type::ObjectComponent;

	if (m_Object && m_Object->GetObjectType() == Object_Type::Player)
	{
		m_Scene->SetPlayerObject(m_Object);
	}
}

CObjectDataComponent::CObjectDataComponent(const CObjectDataComponent& com) :
	CObjectComponent(com)
{
}

CObjectDataComponent::~CObjectDataComponent()
{
}

void CObjectDataComponent::Start()
{
	CObjectComponent::Start();

	// Editor에서는 EditorManager에서 DataManager 클래스를 갖고 있도록 함
	//if (CEngine::GetInst()->GetEditMode())
	//{
	//	CEditorManager::GetInst()->GetDataManager()->SetObjectData(m_Object);
	//}
}

bool CObjectDataComponent::Init()
{
	return true;
}

void CObjectDataComponent::Update(float DeltaTime)
{

}

void CObjectDataComponent::PostUpdate(float DeltaTime)
{

}

void CObjectDataComponent::PrevRender()
{
}

void CObjectDataComponent::Render()
{
}

void CObjectDataComponent::PostRender()
{
}

CObjectDataComponent* CObjectDataComponent::Clone()
{
	return new CObjectDataComponent(*this);
}

bool CObjectDataComponent::Save(FILE* File)
{
	CObjectComponent::Save(File);

	fwrite(&m_Data, sizeof(ObjectData), 1, File);

	return true;
}

bool CObjectDataComponent::Load(FILE* File)
{
	CObjectComponent::Load(File);

	fread(&m_Data, sizeof(ObjectData), 1, File);

	CExcelData* Data = CEditorManager::GetInst()->GetDataManager()->FindData("ObjectData");
	Row* row = nullptr;

	if (Data)
	{
		if (m_Object->GetObjectType() == Object_Type::Player)
		{
			if(CSceneManager::GetInst()->GetNextScene())
				CSceneManager::GetInst()->GetNextScene()->SetPlayerObject(m_Object);
			else
				CSceneManager::GetInst()->GetScene()->SetPlayerObject(m_Object);
			// 플레이어의 초기위치 설정?
			// CSceneManager::GetInst()->GetScene()->GetNavigation3DManager()->CheckPlayerNavMeshPoly();
		}

	}

	return true;
}

bool CObjectDataComponent::SaveOnly(FILE* File)
{
	CObjectComponent::SaveOnly(File);

	fwrite(&m_Data, sizeof(ObjectData), 1, File);

	return true;
}

bool CObjectDataComponent::LoadOnly(FILE* File)
{
	CObjectComponent::LoadOnly(File);

	fread(&m_Data, sizeof(ObjectData), 1, File);

	return true;
}
