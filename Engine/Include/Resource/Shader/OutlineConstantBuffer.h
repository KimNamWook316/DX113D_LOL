#pragma once

#include "ConstantBufferBase.h"

class COutlineConstantBuffer :
    public CConstantBufferBase
{
public:
	COutlineConstantBuffer();
	COutlineConstantBuffer(const COutlineConstantBuffer& Buffer);
	virtual ~COutlineConstantBuffer();

protected:
	OutlineCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual COutlineConstantBuffer* Clone();

public:
	void SetThickness(float Val)
	{
		m_BufferData.Thickness = Val;
	}

	void SetDepthMultiplier(float Val)
	{
		m_BufferData.DepthMultiplier = Val;
	}

	void SetDepthBias(float Val)
	{
		m_BufferData.DepthBias = Val;
	}

	void SetNormalMultiplier(float Val)
	{
		m_BufferData.NormalMultiplier = Val;
	}

	void SetNormalBias(float Val)
	{
		m_BufferData.NormalBias = Val;
	}

	void SetColor(const Vector3& Color)
	{
		m_BufferData.Color = Color;
	}

	void SetColor(const Vector4& Color)
	{
		m_BufferData.Color = Vector3(Color.x, Color.y, Color.z);
	}
};

