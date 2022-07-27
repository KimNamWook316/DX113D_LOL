#include "CameraComponent.h"
#include "../Device.h"
#include "Frustum.h"
#include "../Scene/Scene.h"
#include "../Scene/LightManager.h"
#include "LightComponent.h"
#include "../Render/RenderManager.h"
#include "../Scene/CameraManager.h"
#include "Arm.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();
	m_Render = false;
	m_Init = false;

	m_CameraType = Camera_Type::Camera3D;
	m_ViewAngle = 90.f;
	m_Distance = 1000.f;

	m_Frustum = new CFrustum;
}

CCameraComponent::CCameraComponent(const CCameraComponent& com) :
	CSceneComponent(com)
{
	m_CameraType = com.m_CameraType;
	m_matView = com.m_matView;
	m_matProj = com.m_matProj;
	m_ViewAngle = com.m_ViewAngle;
	m_Distance = com.m_Distance;
	m_RS = com.m_RS;
	
	m_Frustum = new CFrustum;
}

CCameraComponent::~CCameraComponent()
{
	SAFE_DELETE(m_Frustum);

	ClearMoveData();
}

void CCameraComponent::Shake(float Time, float Amount)
{
	if (m_Shake)
	{
		return;
	}

	m_Shake = true;
	m_ShakeTimer = 0.f;
	m_ShakeMaxTime = Time;
	m_ShakeAmount = Amount;
	m_ShakeDecreaseTick = m_ShakeAmount / Time;
	m_OriginRelavitePos = GetRelativePos();
}

CamMoveData* CCameraComponent::FindMoveData(int Index)
{
	if (!IsValidMoveDataIndex(Index))
	{
		return nullptr;
	}

	CamMoveData* Find = nullptr;

	int Idx = 0;
	auto iter = m_MoveDataList.begin();
	auto iterEnd = m_MoveDataList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (Index == Idx)
		{
			Find = (*iter);
			break;
		}
		++Idx;
	}


	return Find;
}

bool CCameraComponent::FrustumInPoint(const Vector3& Point)
{
	return m_Frustum->FrustumInPoint(Point);
}

bool CCameraComponent::FrustumInSphere(const SphereInfo& Sphere)
{
	return m_Frustum->FrustumInSphere(Sphere);
}

void CCameraComponent::CreateProjectionMatrix()
{
	switch (m_CameraType)
	{
	case Camera_Type::Camera2D:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	case Camera_Type::Camera3D:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle),
			m_RS.Width / (float)m_RS.Height, 0.1f, m_Distance);
		break;
	case Camera_Type::CameraUI:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	}

	m_matShadowProj = XMMatrixOrthographicOffCenterLH(-SHADOWMAP_WIDTH / 50.f, SHADOWMAP_WIDTH / 50.f, -SHADOWMAP_HEIGHT / 50.f, SHADOWMAP_HEIGHT / 50.f, 0.f, 1000.f);
	// m_matShadowProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle), SHADOWMAP_WIDTH / SHADOWMAP_HEIGHT, 0.1f, m_Distance);
}

bool CCameraComponent::IsValidMoveDataIndex(int Index)
{
	if (Index < 0 || Index >= m_MoveDataList.size())
	{
		return false;
	}
	return true;
}

void CCameraComponent::UpdateCurMoveData()
{
	m_CurMoveData = m_MoveDataList.front();

	Vector3 ToMovePoint = m_CurMoveData->DestPoint - GetWorldPos();

	m_CurMoveTick = ToMovePoint.Length() / m_CurMoveData->NextPointReachTime;
	m_CurMoveDir = ToMovePoint;

	m_CurMoveDir.Normalize();

	m_MoveDataList.pop_front();

	if (m_CurMoveData->CallBack[(int)CamMoveCallBackCallType::START_MOVE])
	{
		m_CurMoveData->CallBack[(int)CamMoveCallBackCallType::START_MOVE]();
	}
}

void CCameraComponent::CreateCustomResolutionProjMatrix(float Width, float Height)
{
	switch (m_CameraType)
	{
	case Camera_Type::Camera2D:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, Width, 0.f, Height, 0.f, 1000.f);
		break;
	case Camera_Type::Camera3D:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle),
			Width / Height, 0.1f, m_Distance);
		break;
	case Camera_Type::CameraUI:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, Width, 0.f, Height, 0.f, 1000.f);
		break;
	}
}

void CCameraComponent::ComputeShadowView()
{
	CLightComponent* GlobalLight = m_Scene->GetLightManager()->GetGlobalLightComponent();

	m_matShadowView.Identity();

	for (int i = 0; i < AXIS::AXIS_MAX; ++i)
	{
		Vector3 Axis = GlobalLight->GetWorldAxis((AXIS)i);
		memcpy(&m_matShadowView[i][0], &Axis, sizeof(Vector3));
	}

	m_matShadowView.Transpose();

	Vector3 TargetPos;

	if (m_Parent)
	{
		if (m_Parent->CheckType<CArm>())
		{
			CArm* Parent = (CArm*)(m_Parent);

			TargetPos = GetWorldPos() + GetWorldAxis(AXIS_Z) * Parent->GetTargetDistance();
		}
	}

	float ShadowLightDistance = CRenderManager::GetInst()->GetShadowLightDistance();

	Vector3 Pos = TargetPos - GlobalLight->GetWorldAxis(AXIS_Z) * ShadowLightDistance;

	// 플레이어가 있을 경우 그림자 범위가 플레이어를 추적하도록
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	Vector3 Add;

	if (PlayerObj)
	{
		Vector3 PlayerPos = PlayerObj->GetWorldPos();

		Vector3 Offset = Pos - PlayerPos;
		if (!m_Init)
		{
			m_OriginOffset = Offset;
			m_Init = true;
		}
		else
		{
			Add = m_OriginOffset - Offset;
		}
	}

	// 일렁거림 방지를 위해 일정 거리 이동시에만 적용
	if (Add.Length() >= 50.f)
	{
		Pos += Add;
	}

	Pos *= -1.f;

	for (int i = 0; i < AXIS::AXIS_MAX; ++i)
	{
		Vector3 Axis = GlobalLight->GetWorldAxis((AXIS)i);
		m_matShadowView[3][i] = Pos.Dot(Axis);
	}
}

void CCameraComponent::StartMove()
{
	if (m_MoveDataList.empty())
	{
		return;
	}

	m_StartMove = true;

	UpdateCurMoveData();

	SetWorldPos(m_CurMoveData->DestPoint);

	m_Scene->GetCameraManager()->KeepCamera();
	m_Scene->GetCameraManager()->SetCurrentCamera(this);
}

void CCameraComponent::Move(float DeltaTime)
{
	if (!m_CurMoveData)
	{
		UpdateCurMoveData();
	}

	Vector3 ToPoint = m_CurMoveData->DestPoint - GetWorldPos();

	if (ToPoint.Dot(m_CurMoveDir) < 0)
	{
		SetWorldPos(m_CurMoveData->DestPoint);
		m_StayStart = true;

		return;
	}

	if (m_CurMoveData->NextPointReachTime == 0.f)
	{
		SetWorldPos(m_CurMoveData->DestPoint);
		m_StayStart = true;
	}

	if (m_StayStart)
	{
		m_StayTimer += DeltaTime;

		if (m_StayTimer >= m_CurMoveData->StayTime)
		{
			if (m_CurMoveData->CallBack[(int)CamMoveCallBackCallType::REACHED_POINT])
			{
				m_CurMoveData->CallBack[(int)CamMoveCallBackCallType::REACHED_POINT]();
			}

			m_StayStart = false;
			SAFE_DELETE(m_CurMoveData);
			m_CurMoveData = nullptr;
			m_StayTimer = 0.f;

			if (m_MoveDataList.empty())
			{
				if (m_MoveEndCallBack)
				{
					m_MoveEndCallBack();
				}

				m_StartMove = false;
			}
		}
	}
	else
	{
		AddWorldPos(m_CurMoveDir * m_CurMoveTick * DeltaTime);
	}
}

void CCameraComponent::ClearMoveData()
{
	auto iter = m_MoveDataList.begin();
	auto iterEnd = m_MoveDataList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	m_MoveDataList.clear();

#ifdef _DEBUG
	iter = m_TestMoveDataList.begin();
	iterEnd = m_TestMoveDataList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	m_TestMoveDataList.clear();
#endif // _DEBUG
}

CamMoveData* CCameraComponent::AddMoveData(const Vector3& Point, float NextPointReachTime, float StayTime)
{
	CamMoveData* Data = new CamMoveData;
	Data->DestPoint = Point;
	Data->NextPointReachTime = NextPointReachTime;
	Data->StayTime = StayTime;

	m_MoveDataList.push_back(Data);

	return Data;
}

bool CCameraComponent::DeleteMoveData(int Index)
{
	if (!IsValidMoveDataIndex(Index))
	{
		return false;
	}

	auto iter = m_MoveDataList.begin();
	auto iterEnd = m_MoveDataList.end();

	for (int Idx = 0; iter != iterEnd; ++iter, ++Idx)
	{
		if (Index == Idx)
		{
			SAFE_DELETE((*iter));
			m_MoveDataList.erase(iter);
			return true;
		}
	}

	return false;
}

bool CCameraComponent::DeleteMoveData(CamMoveData* Data)
{
	auto iter = m_MoveDataList.begin();
	auto iterEnd = m_MoveDataList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Data)
		{
			SAFE_DELETE((*iter));
			m_MoveDataList.erase(iter);
			return true;
		}
	}

	return false;
}

void CCameraComponent::ChangeMoveData(int Index, const Vector3& Point, float NextPointReachTime, float StayTime)
{
	if (!IsValidMoveDataIndex(Index))
	{
		return;
	}

	CamMoveData* Data = FindMoveData(Index);
	Data->DestPoint = Point;
	Data->NextPointReachTime = NextPointReachTime;
	Data->StayTime = StayTime;
}

void CCameraComponent::ChangeMoveDestPoint(int Index, const Vector3& Point)
{
	if (!IsValidMoveDataIndex(Index))
	{
		return;
	}

	CamMoveData* Data = FindMoveData(Index);
	Data->DestPoint = Point;
}

void CCameraComponent::ChangeMoveDestReachTime(int Index, float Time)
{
	if (!IsValidMoveDataIndex(Index))
	{
		return;
	}

	CamMoveData* Data = FindMoveData(Index);
	Data->NextPointReachTime = Time;
}

void CCameraComponent::ChangeMoveStayTime(int Index, float Time)
{
	if (!IsValidMoveDataIndex(Index))
	{
		return;
	}

	CamMoveData* Data = FindMoveData(Index);
	Data->StayTime = Time;
}

#ifdef _DEBUG

void CCameraComponent::StartTestMove()
{
	if (m_MoveDataList.empty())
	{
		return;
	}

	m_TestMove = true;

	m_TestOriginPos = GetRelativePos();

	auto iter = m_MoveDataList.begin();
	auto iterEnd = m_MoveDataList.end();

	CamMoveData* TestData = nullptr;

	for (; iter != iterEnd; ++iter)
	{
		TestData = new CamMoveData;
		TestData->DestPoint = (*iter)->DestPoint;
		TestData->NextPointReachTime = (*iter)->NextPointReachTime;
		TestData->StayTime = (*iter)->StayTime;

		m_TestMoveDataList.push_back(TestData);
	}

	UpdateCurTestMoveData();

	SetWorldPos(m_CurMoveData->DestPoint);

	m_Scene->GetCameraManager()->KeepCamera();
	m_Scene->GetCameraManager()->SetCurrentCamera(this);
}

#endif // _DEBUG

#ifdef _DEBUG

void CCameraComponent::TestMove(float DeltaTime)
{
	if (!m_CurMoveData)
	{
		UpdateCurTestMoveData();
	}

	Vector3 ToPoint = m_CurMoveData->DestPoint - GetWorldPos();

	if (ToPoint.Dot(m_CurMoveDir) < 0)
	{
		SetWorldPos(m_CurMoveData->DestPoint);
		m_StayStart = true;

		return;
	}

	if (m_CurMoveData->NextPointReachTime == 0.f)
	{
		SetWorldPos(m_CurMoveData->DestPoint);
		m_StayStart = true;
	}

	if (m_StayStart)
	{
		m_StayTimer += DeltaTime;

		if (m_StayTimer >= m_CurMoveData->StayTime)
		{
			m_StayStart = false;
			SAFE_DELETE(m_CurMoveData);
			m_CurMoveData = nullptr;
			m_StayTimer = 0.f;

			if (m_TestMoveDataList.empty())
			{
				SetRelativePos(m_TestOriginPos);
				m_Scene->GetCameraManager()->ReturnCamera();
				m_TestMove = false;
				return;
			}
		}
	}
	else
	{
		AddWorldPos(m_CurMoveDir * m_CurMoveTick * DeltaTime);
	}
}

void CCameraComponent::UpdateCurTestMoveData()
{
	m_CurMoveData = m_TestMoveDataList.front();
	m_TestMoveDataList.pop_front();
	Vector3 ToMovePoint = m_CurMoveData->DestPoint - GetWorldPos();
	m_CurMoveTick = ToMovePoint.Length() / m_CurMoveData->NextPointReachTime;
	m_CurMoveDir = ToMovePoint;
	m_CurMoveDir.Normalize();
}

#endif // _DEBUG

void CCameraComponent::Start()
{
	CSceneComponent::Start();

	CreateProjectionMatrix();
}

bool CCameraComponent::Init()
{
	m_RS = CDevice::GetInst()->GetResolution();

	return true;
}

void CCameraComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_Shake)
	{
		m_ShakeTimer += DeltaTime;

		if (m_ShakeTimer >= m_ShakeMaxTime)
		{
			m_Shake = false;
			SetRelativePos(m_OriginRelavitePos);
			return;
		}

		float x, y;
		x = (float)(rand()) / RAND_MAX;
		y = (float)(rand()) / RAND_MAX;

		bool Reverse = (bool)(rand() / 2);
		if (Reverse)
		{
			x *= -1.f;
		}
		Reverse = (bool)(rand() / 2);
		if (Reverse)
		{
			y *= -1.f;
		}

 		Vector3 Shake = Vector3(x * m_ShakeAmount, y * m_ShakeAmount, 0.f);
 		SetRelativePos(m_OriginRelavitePos + Shake);

		m_ShakeAmount -= m_ShakeDecreaseTick * DeltaTime;
	}

	if (!m_Shake && m_StartMove)
	{
		Move(DeltaTime);
	}

#ifdef _DEBUG
	if (m_TestMove)
	{
		TestMove(DeltaTime);
	}
#endif // _DEBUG
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	m_matView.Identity();

	/*
	i가 0일때
	Xx Xy Xz 0
	0  1  0  0
	0  0  1  0
	0  0  0  1

	i가 1일때
	Xx Xy Xz 0
	Yx Yy Yz 0
	0  0  1  0
	0  0  0  1

	i가 2일때
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1
	*/
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}

	/*
	Xx Xy Xz 0
	Yx Yy Yz 0
	Zx Zy Zz 0
	0  0  0  1
	 
	전치하면
	Xx Yx Zx 0
	Xy Yy Zy 0
	Xz Yz Zz 0
	0  0  0  1
	*/
	m_matView.Transpose();

	Vector3	Pos = GetWorldPos() * -1.f;

	/*
	Xx    Yx   Zx  0
	Xy    Yy   Zy  0
	Xz    Yz   Zz  0
	-X.P -Y.P -Z.P 1
	*/
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);

		m_matView[3][i] = Pos.Dot(Axis);
	}
}

void CCameraComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

void CCameraComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}

bool CCameraComponent::Save(FILE* File)
{
	fwrite(&m_CameraType, sizeof(Camera_Type), 1, File);
	fwrite(&m_matView, sizeof(Matrix), 1, File);
	fwrite(&m_matProj, sizeof(Matrix), 1, File);
	fwrite(&m_ViewAngle, sizeof(float), 1, File);
	fwrite(&m_Distance, sizeof(float), 1, File);
	fwrite(&m_RS, sizeof(Resolution), 1, File);

	int Length = (int)m_MoveDataList.size();
	fwrite(&Length, sizeof(int), 1, File);

	auto iter = m_MoveDataList.begin();
	auto iterEnd = m_MoveDataList.end();

	for (; iter != iterEnd; ++iter)
	{
		Vector3 DestPoint = (*iter)->DestPoint;
		float	NextPointReachTime = (*iter)->NextPointReachTime;
		float	StayTime = (*iter)->StayTime;

		fwrite(&DestPoint, sizeof(Vector3), 1, File);
		fwrite(&NextPointReachTime, sizeof(float), 1, File);
		fwrite(&StayTime, sizeof(float), 1, File);
	}
	
	CSceneComponent::Save(File);

	return true;
}

bool CCameraComponent::Load(FILE* File)
{
	fread(&m_CameraType, sizeof(Camera_Type), 1, File);
	fread(&m_matView, sizeof(Matrix), 1, File);
	fread(&m_matProj, sizeof(Matrix), 1, File);
	fread(&m_ViewAngle, sizeof(float), 1, File);
	fread(&m_Distance, sizeof(float), 1, File);
	fread(&m_RS, sizeof(Resolution), 1, File);

 	int Length = 0;
 	fread(&Length, sizeof(int), 1, File);
 
 	for (int i = 0; i < Length; ++i)
 	{
 		CamMoveData* Data = new CamMoveData;
 
 		Vector3 DestPoint;
 		float NextPointReachTime, StayTime;
 
 		fread(&DestPoint, sizeof(Vector3), 1, File);
 		fread(&NextPointReachTime, sizeof(float), 1, File);
 		fread(&StayTime, sizeof(float), 1, File);
 
 		Data->DestPoint = DestPoint;
 		Data->NextPointReachTime = NextPointReachTime;
 		Data->StayTime = StayTime;
 
 		m_MoveDataList.push_back(Data);
 	}

	CSceneComponent::Load(File);

	return true;
}
