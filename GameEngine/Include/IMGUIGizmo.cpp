#include "IMGUIGizmo.h"
#include "ImGuizmo.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Component/CameraComponent.h"

CIMGUIGizmo::CIMGUIGizmo() :
	m_Object(nullptr),
	m_Scale(1.f, 1.f, 1.f),
	m_Rot(0.f, 0.f, 0.f),
	m_Pos(0.f, 0.f, 0.f)
{
	m_Operation = ImGuizmo::OPERATION::TRANSLATE;
	m_matWorld.Identity();
}

CIMGUIGizmo::~CIMGUIGizmo()
{
}

bool CIMGUIGizmo::Init()
{
	CIMGUIWidget::Init();

	return true;
}

void CIMGUIGizmo::Render()
{
	CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix matScale, matRot, matTrans;
	matScale.Identity();
	matRot.Identity();
	matTrans.Identity();

	matScale.Scaling(m_Scale);
	matRot.Rotation(m_Rot);
	matTrans.Translation(m_Pos);

	Matrix matView, matProj;
	m_matWorld = matScale * matRot * matTrans;

	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

 	ImGuizmo::Manipulate((float*)&matView, (float*)&matProj, m_Operation,
 		ImGuizmo::MODE::WORLD, (float*)&m_matWorld);

	if (ImGuizmo::IsUsing() && m_Object)
	{
		ImGuizmo::DecomposeMatrixToComponents((float*)&m_matWorld, (float*)&m_Pos,
			(float*)&m_Rot, (float*)&m_Scale);

		switch (m_Operation)
		{
		case ImGuizmo::OPERATION::TRANSLATE:
			m_Object->SetWorldPos(m_Pos);
			break;
		case ImGuizmo::OPERATION::ROTATE:
			m_Object->SetWorldRotation(m_Rot);
			break;
		case ImGuizmo::OPERATION::SCALE:
			m_Object->SetWorldScale(m_Scale);
			break;
		}
	}
}
