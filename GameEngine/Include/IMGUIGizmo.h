#pragma once

#include "IMGUIWidget.h"
#include "ImGuizmo.h"

class CIMGUIGizmo :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

private:
	CIMGUIGizmo();
	~CIMGUIGizmo();

public:
	virtual bool Init() override;
	virtual void Render() override;

public:
	void SetScale(const Vector3& Scale)
	{
		m_Scale = Scale;
	}

	void SetRot(const Vector3& Rot)
	{
		m_Rot = Rot;
	}

	void SetPos(const Vector3& Pos)
	{
		m_Pos = Pos;
	}

	void SetGameObject(class CGameObject* Object)
	{
		m_Object = Object;
	}

	void SetOperationMode(ImGuizmo::OPERATION Operation)
	{
		m_Operation = Operation;
	}

private:
	Vector3 m_Scale;
	Vector3 m_Rot;
	Vector3 m_Pos;

	Matrix m_matWorld;

	ImGuizmo::OPERATION m_Operation;
	class CGameObject* m_Object;
};

