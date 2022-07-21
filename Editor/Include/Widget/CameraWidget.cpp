#include "CameraWidget.h"
#include "Component/CameraComponent.h"
#include "IMGUIText.h"
#include "IMGUIInputFloat.h"
#include "IMGUISeperator.h"
#include "IMGUISameLine.h"
#include "IMGUIButton.h"
#include "IMGUITree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"

CCameraWidget::CCameraWidget()
{
}

CCameraWidget::~CCameraWidget()
{
}

bool CCameraWidget::Init()
{
	CSceneComponentWidget::Init();

	m_ComponentTypeText->SetText("Camera Component");

	CIMGUITree* Tree = AddWidget<CIMGUITree>("Camrera Variables", 200.f);

	m_ViewAngle = Tree->AddWidget<CIMGUIInputFloat>("ViewAngle", 200.f);
	m_SetCurrent = Tree->AddWidget<CIMGUIButton>("Set Current", 0.f, 0.f);
	m_ShakeTime = Tree->AddWidget<CIMGUIInputFloat>("Shake Time", 200.f);
	m_ShakeAmount = Tree->AddWidget<CIMGUIInputFloat>("Shake Amount", 200.f);
	m_Shake = Tree->AddWidget<CIMGUIButton>("Shake Test", 0.f, 0.f);
	AddWidget<CIMGUISeperator>("Sep");

	m_ViewAngle->SetCallBack(this, &CCameraWidget::OnChangeViewAngle);
	m_SetCurrent->SetClickCallback(this, &CCameraWidget::OnClickSetCurrent);
	m_Shake->SetClickCallback(this, &CCameraWidget::OnClickShake);

	return true;
}

void CCameraWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CCameraComponent* CamCom = dynamic_cast<CCameraComponent*>(m_Component);

	float ViewAngle = CamCom->GetViewAngle();

	m_ViewAngle->SetVal(ViewAngle);
}

void CCameraWidget::OnChangeViewAngle(float Val)
{
	CCameraComponent* CamCom = dynamic_cast<CCameraComponent*>(m_Component);

	CamCom->SetViewAngle(Val);
}

void CCameraWidget::OnClickSetCurrent()
{
	CCameraManager* CamMgr = CSceneManager::GetInst()->GetScene()->GetCameraManager();

	CCameraComponent* CurCam = CamMgr->GetCurrentCamera();

	if (CurCam == (CCameraComponent*)m_Component)
	{
		CamMgr->ReturnCamera();
		return;
	}
	else
	{
		CamMgr->KeepCamera();
		CamMgr->SetCurrentCamera((CCameraComponent*)m_Component);
	}
}

void CCameraWidget::OnClickShake()
{
	CCameraManager* CamMgr = CSceneManager::GetInst()->GetScene()->GetCameraManager();

	CCameraComponent* CurCam = CamMgr->GetCurrentCamera();

	if (CurCam != (CCameraComponent*)m_Component)
	{
		MessageBox(nullptr, TEXT("현재 카메라가 아님"), TEXT("Warning"), MB_OK);
		return;
	}

	float Time = m_ShakeTime->GetVal();
	float Amount = m_ShakeAmount->GetVal();

	if (Time * Amount < 0)
	{
		MessageBox(nullptr, TEXT("잘못된 입력값"), TEXT("Warning"), MB_OK);
		return;
	}

	CurCam->Shake(Time, Amount);
}
