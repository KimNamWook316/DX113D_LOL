#pragma once

#include "SceneComponentWidget.h"

class CColliderBox3DWidget	:
	public CSceneComponentWidget
{
public:
	CColliderBox3DWidget();
	~CColliderBox3DWidget();

public:
	virtual bool Init() override;

public:
	virtual void SetSceneComponent(class CSceneComponent* Com) override;

private:
	Vector3 m_Offset;
	class CIMGUIInputFloat3* m_OffsetInput;


private:
	void OnChangeOffset(const Vector3& Offset);
};

