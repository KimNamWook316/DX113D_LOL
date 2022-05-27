#include "IMGUIGrid.h"
#include "ImGuizmo.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Component/CameraComponent.h"

CIMGUIGrid::CIMGUIGrid()
{
	m_matIdentity.Identity();
}

CIMGUIGrid::~CIMGUIGrid()
{
}

bool CIMGUIGrid::Init()
{
	CIMGUIWidget::Init();

	return true;
}

void CIMGUIGrid::Render()
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix matView, matProj;
	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	RECT Rect;
	GetWindowRect(CEngine::GetInst()->GetWindowHandle(), &Rect);

	Resolution Res = CDevice::GetInst()->GetResolution();
	Vector2 Ratio = CDevice::GetInst()->GetViewportAspectRatio();
	Res.Width *= Ratio.x;
	Res.Height *= Ratio.y;

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(Rect.left, Rect.top,Res.Width, Res.Height);
	ImGuizmo::DrawGrid((float*)&matView, (float*)&matProj, (float*)&m_matIdentity, m_Size.x);
}
