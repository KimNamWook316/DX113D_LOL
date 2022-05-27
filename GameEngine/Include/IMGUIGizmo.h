#pragma once

#include "IMGUIWidget.h"
#include "ImGuizmo.h"
#include "GameObject\GameObject.h"

class CIMGUIGizmo :
	public CIMGUIWidget
{
public:
	CIMGUIGizmo();
	~CIMGUIGizmo();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetScale(const Vector3& Scale)
	{
		m_Scale = Scale;
		m_PrevScale = Scale;
	}

	void SetRot(const Vector3& Rot)
	{
		m_Rot = Rot;
		m_PrevRot = Rot;
	}

	void SetPos(const Vector3& Pos)
	{
		m_Pos = Pos;
		m_PrevPos = Pos;
	}

	void SetGameObject(class CGameObject* Object)
	{
		m_Object = Object;
	}

	void SetOperationMode(ImGuizmo::OPERATION Operation)
	{
		m_Operation = Operation;
	}

	void SetMode(ImGuizmo::MODE Mode)
	{
		m_Mode = Mode;
	}

private:
	Vector3 m_Scale;
	Vector3 m_Rot;
	Vector3 m_Pos;

	Vector3 m_PrevScale;
	Vector3 m_PrevRot;
	Vector3 m_PrevPos;

	Matrix m_matWorld;

	ImGuizmo::OPERATION m_Operation;
	ImGuizmo::MODE m_Mode;
	class CGameObject* m_Object;

	bool m_Using;
};

