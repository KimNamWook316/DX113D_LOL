#pragma once

#include "ConstantBufferBase.h"

class CHDRRenderCBuffer :
    public CConstantBufferBase
{
public:
	CHDRRenderCBuffer();
	CHDRRenderCBuffer(const CHDRRenderCBuffer& Buffer);
	virtual ~CHDRRenderCBuffer();

protected:
	HDRRenderCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CHDRRenderCBuffer* Clone();

public:
	void SetMiddleGray(float MiddleGray)
	{
		m_BufferData.MiddleGray = MiddleGray;
	}

	void SetLumWhite(float White)
	{
		m_BufferData.LumWhiteSqr = White;
	}

	float GetMiddleGray() const
	{
		return m_BufferData.MiddleGray;
	}

	float GetLumWhite() const
	{
		return m_BufferData.LumWhiteSqr;
	}
};

