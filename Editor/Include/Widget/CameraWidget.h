#pragma once

#include "SceneComponentWidget.h"

class CCameraWidget :
    public CSceneComponentWidget
{
public:
	CCameraWidget();
	~CCameraWidget();

public:
	virtual bool Init() override;

public:
	virtual void SetSceneComponent(class CSceneComponent* Com) override;

private:
	void OnChangeViewAngle(float Val);
	void OnClickSetCurrent();
	void OnClickShake();

private:
	class CIMGUIInputFloat* m_ViewAngle;
	class CIMGUIButton* m_SetCurrent;
	class CIMGUIInputFloat* m_ShakeTime;
	class CIMGUIInputFloat* m_ShakeAmount;
	class CIMGUIButton* m_Shake;
};

