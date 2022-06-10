#pragma once

#include "IMGUIWindow.h"
#include "IMGUICollapsingHeader.h"
#include "IMGUIGizmo.h"
#include "IMGUIGrid.h"
#include "IMGUIRadioButton.h"
#include "IMGUIButton.h"
#include "IMGUISliderFloat.h"
#include "IMGUIColor3.h"
#include "IMGUITree.h"
#include "IMGUICheckBox.h"

class CToolWindow :
    public CIMGUIWindow
{
public:
	CToolWindow();
	~CToolWindow();

public:
	virtual bool Init();

public:
	void SetGizmoObject(class CGameObject* Object);
	void SetGizmoComponent(class CSceneComponent* SceneComp);

public:
	// 디버그용 임시 키
	void OnQDown(float DetlaTime);
	void OnWDown(float DetlaTime);
	void OnEDown(float DetlaTime);

private:
	void OnSelectGizmoOperationMode(const char* Label, bool Check);
	void OnSelectGizmoTransformMode(const char* Label, bool Check);
	void OnChangeCameraSpeed(float Speed);
	void OnChangeOutlineDepthMultiply(float Val);
	void OnChangeOutlineDepthBias(float Val);
	void OnChangeOutlineNormalMultiply(float Val);
	void OnChangeOutlineNormalBias(float Val);
	void OnCheckGrayEnable(const char* Label, bool Check);
	void OnClickPlay();
	void OnClickPause();
	void OnClickStop();

private:
	// Gizmo
	CIMGUICollapsingHeader* m_GizmoBlock;
	CIMGUIGizmo* m_Gizmo;
	CIMGUIGrid* m_Grid;
	CIMGUIRadioButton* m_GizmoTransformMode;
	CIMGUIRadioButton* m_GizmoOperationMode;

	// Camera
	CIMGUICollapsingHeader* m_EditorCameraBlock;
	CIMGUISliderFloat* m_CameraSpeed;

	// Render
	CIMGUICollapsingHeader* m_RenderBlock;
	CIMGUISliderFloat* m_OutlineDepthMultiply;
	CIMGUISliderFloat* m_OutlineDepthBias;
	CIMGUISliderFloat* m_OutlineNormalMutliply;
	CIMGUISliderFloat* m_OutlineNormalBias;
	CIMGUICheckBox* m_GrayEnable;

	// PlayStop
	CIMGUIButton* m_Play;
	CIMGUIButton* m_Pause;
	CIMGUIButton* m_Stop;
};

