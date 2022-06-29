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
#include "IMGUIText.h"
#include "IMGUIComboBox.h"

class CToolWindow :
    public CIMGUIWindow
{
public:
	CToolWindow();
	~CToolWindow();

public:
	virtual bool Init();

public:
	void SetPlayText(bool Play);
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
	void OnChangeLumWhite(float Speed);
	void OnChangeMiddleGray(float Speed);
	void OnChangeAdaptationTime(float Time);
	void OnChangeBloomThreshold(float Threshold);
	void OnChangeBloomScale(float Scale);
	void OnChangeDOFMin(float Min);
	void OnChangeDOFMax(float Max);
	void OnSelectFogType(int Index, const char* Label);
	void OnChageFogColor(const Vector3& Color);
	void OnChangeFogStart(float Val);
	void OnChangeFogEnd(float Val);
	void OnChangeFogDensity(float Val);
 //	void OnChangeOutlineDepthMultiply(float Val);
 //	void OnChangeOutlineDepthBias(float Val);
 //	void OnChangeOutlineNormalMultiply(float Val);
 //	void OnChangeOutlineNormalBias(float Val);
 //	void OnCheckGrayEnable(const char* Label, bool Check);
	void OnCheckDebugRender(const char* Label, bool Check);
	void OnCheckPostProcessing(const char* Label, bool Check);
	void OnClickPlay();
	void OnClickPause();
	void OnClickStop();
	void ClearSceneRelatedWindows();
	void RefreshSceneRelatedWindow(class CGameObject* Object);
	void RefreshSceneRelatedWindow(const std::vector<CGameObject*>& vecObj);

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
	CIMGUICheckBox* m_DebugRender;
	CIMGUICheckBox* m_PostProcessing;
	CIMGUISliderFloat* m_AdaptationTime;
	CIMGUISliderFloat* m_LumWhite;
	CIMGUISliderFloat* m_MiddleGray;
	CIMGUISliderFloat* m_BloomThreshold;
	CIMGUISliderFloat* m_BloomScale;
	CIMGUISliderFloat* m_DOFMin;
	CIMGUISliderFloat* m_DOFMax;
	CIMGUIColor3* m_FogColor;
	CIMGUIComboBox* m_FogType;
	CIMGUISliderFloat* m_FogStart;
	CIMGUISliderFloat* m_FogEnd;
	CIMGUISliderFloat* m_FogDensity;
 //	CIMGUISliderFloat* m_OutlineDepthMultiply;
 //	CIMGUISliderFloat* m_OutlineDepthBias;
 //	CIMGUISliderFloat* m_OutlineNormalMutliply;
 //	CIMGUISliderFloat* m_OutlineNormalBias;
 //	CIMGUICheckBox* m_GrayEnable;

	// PlayStop
	CIMGUIText* m_PlayState;
	CIMGUIButton* m_Play;
	CIMGUIButton* m_Pause;
	CIMGUIButton* m_Stop;
};

