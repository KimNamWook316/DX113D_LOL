#include "ToolWindow.h"
#include "IMGUISetRect.h"
#include "Input.h"
#include "../EditorManager.h"
#include "../Object/3DCameraObject.h"
#include "Render/RenderManager.h"
#include "Engine.h"
#include "IMGUISameLine.h"
#include "Scene/SceneManager.h"

CToolWindow::CToolWindow()	:
	m_GizmoBlock(nullptr),
	m_Gizmo(nullptr),
	m_Grid(nullptr),
	m_GizmoOperationMode(nullptr),
	m_GizmoTransformMode(nullptr)
{
}

CToolWindow::~CToolWindow()
{
}

bool CToolWindow::Init()
{
	CIMGUIWindow::Init();

	// Play Stop
	CIMGUICollapsingHeader* PlayStopBlock = AddWidget<CIMGUICollapsingHeader>("Play", 200.f);
	m_Play = PlayStopBlock->AddWidget<CIMGUIButton>("Play", 0.f, 0.f);
	PlayStopBlock->AddWidget<CIMGUISameLine>("line");
	m_Pause = PlayStopBlock->AddWidget<CIMGUIButton>("Pause", 0.f, 0.f);
	PlayStopBlock->AddWidget<CIMGUISameLine>("line");
	m_Stop = PlayStopBlock->AddWidget<CIMGUIButton>("Stop", 0.f, 0.f);

	m_GizmoBlock = AddWidget<CIMGUICollapsingHeader>("Gizmo", 200.f);
	//m_Grid = AddWidget<CIMGUIGrid>("Grid", 100.f);
	AddWidget<CIMGUISetRect>("GridRect"); // Gizmo가 그려지기 위한 배경
	m_Gizmo = AddWidget<CIMGUIGizmo>("Gimzo");
	m_GizmoTransformMode = m_GizmoBlock->AddWidget<CIMGUIRadioButton>("Transform");
	m_GizmoOperationMode = m_GizmoBlock->AddWidget<CIMGUIRadioButton>("Operation");

	m_GizmoTransformMode->SetColNum(2);
	m_GizmoOperationMode->SetColNum(3);

	m_GizmoTransformMode->AddCheckInfo("Local");
	m_GizmoTransformMode->AddCheckInfo("World");
	m_GizmoTransformMode->SetCheck(1, true);
	m_GizmoTransformMode->AlwaysCheck(true);

	m_GizmoOperationMode->AddCheckInfo("Translation");
	m_GizmoOperationMode->AddCheckInfo("Rotation");
	m_GizmoOperationMode->AddCheckInfo("Scale");
	m_GizmoOperationMode->SetCheck(0, true);
	m_GizmoOperationMode->AlwaysCheck(true);

	// Camera
	m_EditorCameraBlock = AddWidget<CIMGUICollapsingHeader>("Editor Camera", 200.f);
	m_CameraSpeed = m_EditorCameraBlock->AddWidget<CIMGUISliderFloat>("Speed");

	// Render
	// Outline
	m_RenderBlock = AddWidget<CIMGUICollapsingHeader>("Render", 200.f);
	CIMGUITree* Tree = m_RenderBlock->AddWidget<CIMGUITree>("Outline", 200.f);
	m_OutlineDepthMultiply = Tree->AddWidget<CIMGUISliderFloat>("Outline Depth Multiplier");
	m_OutlineDepthBias = Tree->AddWidget<CIMGUISliderFloat>("Outline Depth Bias");
	m_OutlineNormalMutliply = Tree->AddWidget<CIMGUISliderFloat>("Outline Normal Mutiplier");
	m_OutlineNormalBias = Tree->AddWidget<CIMGUISliderFloat>("Outline Normal Bias");
	// Gray
	Tree = m_RenderBlock->AddWidget<CIMGUITree>("Gray", 200.f);
	m_GrayEnable = Tree->AddWidget<CIMGUICheckBox>("GrayShader Enable", 200.f);

	// Initial Value
	m_CameraSpeed->SetMin(0.f);
	m_CameraSpeed->SetMax(10.f);
	m_CameraSpeed->SetValue(CEditorManager::GetInst()->Get3DCameraObject()->GetCameraSpeed());
	m_OutlineDepthMultiply->SetMin(0.1f);
	m_OutlineDepthMultiply->SetMax(5.f);
	m_OutlineDepthBias->SetMin(0.1f);
	m_OutlineDepthBias->SetMax(50.f);
	m_OutlineNormalMutliply->SetMin(0.1f);
	m_OutlineNormalMutliply->SetMax(5.f);
	m_OutlineNormalBias->SetMin(0.1f);
	m_OutlineNormalBias->SetMax(50.f);

	m_OutlineDepthMultiply->SetValue(CRenderManager::GetInst()->GetOutlineDepthMultiplier());
	m_OutlineDepthBias->SetValue(CRenderManager::GetInst()->GetOutlineDepthBias());
	m_OutlineNormalMutliply->SetValue(CRenderManager::GetInst()->GetOutlineNormalMultiplier());
	m_OutlineNormalBias->SetValue(CRenderManager::GetInst()->GetOutlineNormalBias());
	
	m_GrayEnable->AddCheckInfo("Enable GrayShader");

	bool IsGray = CRenderManager::GetInst()->IsGray();
	m_GrayEnable->SetCheck(0, IsGray);

	// CallBack
	m_GizmoTransformMode->SetCallBack(this, &CToolWindow::OnSelectGizmoTransformMode);
	m_GizmoOperationMode->SetCallBack(this, &CToolWindow::OnSelectGizmoOperationMode);
	m_CameraSpeed->SetCallBack(this, &CToolWindow::OnChangeCameraSpeed);
	m_OutlineDepthMultiply->SetCallBack(this, &CToolWindow::OnChangeOutlineDepthMultiply);
	m_OutlineDepthBias->SetCallBack(this, &CToolWindow::OnChangeOutlineDepthBias);
	m_OutlineNormalMutliply->SetCallBack(this, &CToolWindow::OnChangeOutlineNormalMultiply);
	m_OutlineNormalBias->SetCallBack(this, &CToolWindow::OnChangeOutlineNormalBias);
	m_GrayEnable->SetCallBackLabel(this, &CToolWindow::OnCheckGrayEnable);
	m_Play->SetClickCallback(this, &CToolWindow::OnClickPlay);
	m_Pause->SetClickCallback(this, &CToolWindow::OnClickPause);
	m_Stop->SetClickCallback(this, &CToolWindow::OnClickStop);

	// 디버그용 임시 키
	CInput::GetInst()->CreateKey("Z", 'Z');
	CInput::GetInst()->CreateKey("X", 'X');
	CInput::GetInst()->CreateKey("C", 'C');

	// 디버그용 임시 키
	CInput::GetInst()->SetKeyCallback("Z", KeyState_Down, this, &CToolWindow::OnQDown);
	CInput::GetInst()->SetKeyCallback("X", KeyState_Down, this, &CToolWindow::OnWDown);
	CInput::GetInst()->SetKeyCallback("C", KeyState_Down, this, &CToolWindow::OnEDown);

	return true;
}

void CToolWindow::SetGizmoObject(CGameObject* Object)
{
	m_Gizmo->SetGameObject(Object);
}

void CToolWindow::SetGizmoComponent(CSceneComponent* SceneComp)
{
	m_Gizmo->SetComponent(SceneComp);
}

void CToolWindow::OnSelectGizmoOperationMode(const char* Label, bool Check)
{
	if (strcmp(Label, "Translation") == 0)
	{
		m_Gizmo->SetOperationMode(ImGuizmo::OPERATION::TRANSLATE);
	}
	else if (strcmp(Label, "Rotation") == 0)
	{
		m_Gizmo->SetOperationMode(ImGuizmo::OPERATION::ROTATE);
	}
	else if (strcmp(Label, "Scale") == 0)
	{
		m_Gizmo->SetOperationMode(ImGuizmo::OPERATION::SCALE);
	}
}

void CToolWindow::OnSelectGizmoTransformMode(const char* Label, bool Check)
{
	if (strcmp(Label, "Local") == 0)
	{
		m_Gizmo->SetMode(ImGuizmo::MODE::LOCAL);
	}
	else if (strcmp(Label, "World") == 0)
	{
		m_Gizmo->SetMode(ImGuizmo::MODE::WORLD);
	}
}

void CToolWindow::OnChangeCameraSpeed(float Speed)
{
	CEditorManager::GetInst()->Get3DCameraObject()->SetCameraSpeed(Speed);
}

void CToolWindow::OnChangeOutlineDepthMultiply(float Val)
{
	CRenderManager::GetInst()->SetOutlineDepthMultiplier(Val);
}

void CToolWindow::OnChangeOutlineDepthBias(float Val)
{
	CRenderManager::GetInst()->SetOutlineDepthBias(Val);
}

void CToolWindow::OnChangeOutlineNormalMultiply(float Val)
{
	CRenderManager::GetInst()->SetOutlineNormalMultiplier(Val);
}

void CToolWindow::OnChangeOutlineNormalBias(float Val)
{
	CRenderManager::GetInst()->SetOutlineNormalBias(Val);
}

void CToolWindow::OnCheckGrayEnable(const char* Label, bool Check)
{
	CRenderManager::GetInst()->GrayEnable(Check);
}

void CToolWindow::OnClickPlay()
{
	bool Pause = CSceneManager::GetInst()->IsPause();

	if (Pause)
	{
		CSceneManager::GetInst()->Resume();

		m_Play->SetName("Play");
	}
	else
	{
		CSceneManager::GetInst()->Play();
	}
}

void CToolWindow::OnClickPause()
{
	CSceneManager::GetInst()->Pause();

	bool Pause = CSceneManager::GetInst()->IsPause();

	if (Pause)
	{
		m_Play->SetName("Resume");
	}
}

void CToolWindow::OnClickStop()
{
}

void CToolWindow::OnQDown(float DetlaTime)
{
	m_Gizmo->SetOperationMode(ImGuizmo::OPERATION::TRANSLATE);
}

void CToolWindow::OnWDown(float DetlaTime)
{
	m_Gizmo->SetOperationMode(ImGuizmo::OPERATION::ROTATE);
}

void CToolWindow::OnEDown(float DetlaTime)
{
	m_Gizmo->SetOperationMode(ImGuizmo::OPERATION::SCALE);
}


