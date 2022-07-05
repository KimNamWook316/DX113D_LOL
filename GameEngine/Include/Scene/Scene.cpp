
#include "Scene.h"
#include "../PathManager.h"
#include "SceneManager.h"
#include "../Component/CameraComponent.h"
#include "../GameObject/SkyObject.h"
#include "../Input.h"
#include "../Collision/Collision.h"

CScene::CScene()
{
	m_Mode = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;
	m_CameraManager = new CCameraManager;
	m_Viewport = new CViewport;
	m_NavManager = new CNavigationManager;
	m_Nav3DManager = new CNavigation3DManager;
	m_LightManager = new CLightManager;

	m_Mode->m_Scene = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	m_CameraManager->m_Scene = this;
	m_Viewport->m_Scene = this;
	m_NavManager->m_Scene = this;
	m_Nav3DManager->m_Scene = this;
	m_LightManager->m_Scene = this;

	m_Start = false;
	m_Play = true;

	m_Collision->Init();
	m_CameraManager->Init();
	m_Viewport->Init();
	m_NavManager->Init();
	m_Nav3DManager->Init();
	m_LightManager->Init();

	m_Change = true;

	m_SkyObject = new CSkyObject;

	m_SkyObject->SetName("Sky");
	m_SkyObject->SetScene(this);

	m_SkyObject->Init();
}

CScene::~CScene()
{
	m_ObjList.clear();
	SAFE_DELETE(m_NavManager)	
	SAFE_DELETE(m_Nav3DManager);
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Resource);
	m_LightManager->Destroy();
	SAFE_DELETE(m_LightManager);
}

void CScene::Start()
{
	m_Mode->Start();

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_Start = true;

	m_SkyObject->Start();

	m_CameraManager->Start();
	m_Collision->Start();
	m_Viewport->Start();
	m_LightManager->Start();

	if (m_Mode->GetPlayerObject())
	{
		CCameraComponent* Camera = m_Mode->GetPlayerObject()->FindComponentFromType<CCameraComponent>();

		if (Camera)
		{
			m_CameraManager->SetCurrentCamera(Camera);
		}
	}

	m_NavManager->Start();
	m_Nav3DManager->Start();
}

void CScene::Update(float DeltaTime)
{
	if (!m_Play)
	{
		DeltaTime = 0.f;
	}

	m_Mode->Update(DeltaTime);

	// State에서 각 Collision Section별로 Collider를 얻어와야 하는 경우가 있어서 (ex. CheckTurretAttackTarget)
	// CComponent::Update하기 전에 이걸 먼저 해주도록 수정
	m_Collision->CheckColliderSection3D();

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_CameraManager->Update(DeltaTime);

	if (m_Play)
	{
		m_Viewport->Update(DeltaTime);
		m_NavManager->Update(DeltaTime);
		m_Nav3DManager->Update(DeltaTime);
	}

	m_LightManager->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	if (!m_Play)
	{
		DeltaTime = 0.f;
	}

	m_Mode->PostUpdate(DeltaTime);

	m_SkyObject->PostUpdate(DeltaTime);


	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	m_CameraManager->PostUpdate(DeltaTime);

	if (m_Play)
	{
		m_Viewport->PostUpdate(DeltaTime);
	}

	m_RenderComponentList.clear();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();

		const std::list<CSceneComponent*>& List = (*iter)->GetSceneComponents();

		auto iter1 = List.begin();
		auto iter1End = List.end();

		for (; iter1 != iter1End; ++iter1)
		{
			if ((*iter1)->GetRender() && !(*iter1)->GetCulling())
			{
				m_RenderComponentList.push_back(*iter1);
			}
		}
	}

	// 출력되는 물체를 정렬한다.
	if (m_RenderComponentList.size() >= 2)
	{
		m_RenderComponentList.sort(SortRenderList);
	}

	if (m_Play)
	{
		// 포함된 충돌체들을 이용해서 충돌처리를 진행한다.
		m_Collision->Collision(DeltaTime);
	}
}

bool CScene::Save(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	return SaveFullPath(FullPath);
}

bool CScene::SaveFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return false;

	size_t	SceneModeType = m_Mode->GetTypeID();

	fwrite(&SceneModeType, sizeof(size_t), 1, File);

	size_t	ObjCount = m_ObjList.size();
	int ExcludeObjectCount = GetSaveExcludeObjectCount();
	ObjCount -= (size_t)ExcludeObjectCount;

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetParentObject())
			--ObjCount;
	}

	fwrite(&ObjCount, sizeof(size_t), 1, File);

	bool Success = false;

	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsExcludeFromSceneSave())
		{
			continue;
		}

		if ((*iter)->GetParentObject())
		{
			continue;
		}

		size_t	ObjType = (*iter)->GetTypeID();

		fwrite(&ObjType, sizeof(size_t), 1, File);

		//Success = (*iter)->Save(File);
		Success = (*iter)->SaveHierarchy(File);

		if (!Success)
		{
			fclose(File);
			return false;
		}
	}

	fclose(File);
	return true;
}

bool CScene::Load(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	return LoadFullPath(FullPath);
}

bool CScene::LoadFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return false;

	// LightManager에서 생성된 Global Light를 삭제하지 않기 위해
	// m_ObjList.clear();

	size_t	SceneModeType = 0;

	fread(&SceneModeType, sizeof(size_t), 1, File);

	// SceneMode 생성
	CSceneManager::GetInst()->CallCreateSceneMode(this, SceneModeType);

	size_t	ObjCount = m_ObjList.size();

	fread(&ObjCount, sizeof(size_t), 1, File);

	bool Success = false;

	for (size_t i = 0; i < ObjCount; ++i)
	{
		size_t	ObjType = 0;
		fread(&ObjType, sizeof(size_t), 1, File);

		CGameObject* Obj = CSceneManager::GetInst()->CallCreateObject(this, ObjType);

		//Success = Obj->Load(File);
		// 여기서 NextScene 포인터를 Obj->LoadHierarchy안으로 넘겨줘야 한다
		Success = Obj->LoadHierarchy(File, this);

		CSceneManager::GetInst()->CallObjectDataSet(Obj, Obj->GetName());

		if (!Success)
		{
			fclose(File);
			return false;
		}

		Obj->SetScene(this);
	}

	fclose(File);
	return true;
}

bool CScene::Picking(CGameObject*& Result)
{
	CCameraComponent* Camera = m_CameraManager->GetCurrentCamera();

	Ray	RayWorld = CInput::GetInst()->GetRay(Camera->GetViewMatrix());

	auto	iter = m_RenderComponentList.begin();
	auto	iterEnd = m_RenderComponentList.end();

	Vector3 HitPoint;

	for (; iter != iterEnd; ++iter)
	{
		SphereInfo Info = (*iter)->GetSphereInfo();

		if (CCollision::CollisionRayToSphere(HitPoint, RayWorld, Info))
		{
			Result = (*iter)->GetGameObject();
			return true;
		}
	}

	Result = nullptr;

	return false;
}

bool CScene::CheckSameName(const std::string& Name)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return true;
	}

	return false;
}

void CScene::GetAllObjectsPointer(std::vector<CGameObject*>& vecOutObj)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		vecOutObj.push_back(*iter);
	}
}

void CScene::GetAllIncludeSaveObjectsPointer(std::vector<CGameObject*>& vecOutObj)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsExcludeFromSceneSave())
			continue;

		vecOutObj.push_back(*iter);
	}
}

void CScene::CloneAllNoDestroyObjects(std::list<CSharedPtr<CGameObject>>& OutList)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsNoDestroyFromSceneChange())
		{
			OutList.push_back((*iter));
		}
	}
}

void CScene::AddObject(CGameObject* Object)
{
	Object->SetScene(this);
	m_ObjList.push_back(Object);
}

bool CScene::CameraMove(const Vector3& Direction, const Vector3& DestPos, float Speed, float DeltaTime)
{
	CCameraComponent* CurrentCamera = m_CameraManager->GetCurrentCamera();

	CurrentCamera->AddRelativePos(Direction.x * Speed * DeltaTime, 0.f, Direction.z * Speed * DeltaTime);

	Vector3 CurrentCameraPos = CurrentCamera->GetWorldPos();
	float Dist = Vector3(CurrentCameraPos.x, 0.f, CurrentCameraPos.z).Distance(Vector3(DestPos.x, 0.f, DestPos.z));

	if (Dist < 1.f)
		return true;

	return false;
}

bool CScene::RestoreCamera(float Speed, float DeltaTime)
{
	CCameraComponent* CurrentCamera = m_CameraManager->GetCurrentCamera();

	Vector3 CurrentCamPos = CurrentCamera->GetWorldPos();
	Vector3 RestoreDir = m_OriginCamPos - CurrentCamPos;
	RestoreDir.y = 0.f;
	RestoreDir.Normalize();

	if (Vector3(CurrentCamPos.x, 0.f, CurrentCamPos.z).Distance(Vector3(m_OriginCamPos.x, 0.f, m_OriginCamPos.z)) < 0.5f)
		return true;

	else
	{
		CurrentCamera->AddRelativePos(RestoreDir.x * Speed * DeltaTime, 0.f, RestoreDir.z * Speed * DeltaTime);
	}


	return false;
}


int CScene::GetSaveExcludeObjectCount()
{
	int Count = 0;

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsExcludeFromSceneSave())
		{
			++Count;
		}
	}

	return Count;
}

bool CScene::SortRenderList(CSceneComponent* Src, CSceneComponent* Dest)
{
	SphereInfo	SrcInfo = Src->GetSphereInfoViewSpace();
	SphereInfo	DestInfo = Dest->GetSphereInfoViewSpace();

	return SrcInfo.Center.Length() - SrcInfo.Radius > DestInfo.Center.Length() - DestInfo.Radius;
}
