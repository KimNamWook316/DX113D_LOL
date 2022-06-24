#include "HDRRenderCBuffer.h"
#include "ConstantBuffer.h"

CHDRRenderCBuffer::CHDRRenderCBuffer()	:
	m_BufferData{}
{
	m_BufferData.LumWhiteSqr = 1.f;
	m_BufferData.MiddleGray = 1.f;
	m_BufferData.BloomScale = 1.f;
}

CHDRRenderCBuffer::CHDRRenderCBuffer(const CHDRRenderCBuffer& Buffer)	:
	CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CHDRRenderCBuffer::~CHDRRenderCBuffer()
{
}

bool CHDRRenderCBuffer::Init()
{
	SetConstantBuffer("HDRRenderCBuffer");

	return true;
}

void CHDRRenderCBuffer::UpdateCBuffer()
{
	// 쉐이더에서 계산 최적화를 위해 제곱
	m_BufferData.LumWhiteSqr *= m_BufferData.LumWhiteSqr;

	m_Buffer->UpdateBuffer(&m_BufferData);
}

CHDRRenderCBuffer* CHDRRenderCBuffer::Clone()
{
	return new CHDRRenderCBuffer(*this);
}
