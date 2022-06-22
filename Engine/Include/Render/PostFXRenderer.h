#pragma once

#include "../GameInfo.h"

class CPostFXRenderer
{
	friend class CRenderManager;

private:
	CPostFXRenderer();
	~CPostFXRenderer();

public:
	bool Init();

public:
	void SetMiddleGray(float Gray);
	void SetLumWhite(float White);
	float GetMiddleGray() const;
	float GetLumWhite() const;

public:
	void ExcuteDownScale(class CRenderTarget* LDRTarget);
	void Render(class CRenderTarget* LDRTarget, class CRenderState* DepthDisable);

private:

	// Shader
	class CComputeShader* m_DownScaleFirstPathShader;
	class CComputeShader* m_DownScaleSecondPathShader;
	class CShader* m_HDRRenderShader;
	
	// ConstantBuffer
	class CDownScaleCBuffer* m_DownScaleCBuffer;
	class CHDRRenderCBuffer* m_HDRRenderCBuffer;

	// HDR Resources
	ID3D11Buffer* m_DownScaleBuffer;
	ID3D11UnorderedAccessView* m_DownScaleUAV;
	ID3D11ShaderResourceView* m_DownScaleSRV;

	ID3D11Buffer* m_LuminanceAverageBuffer;
	ID3D11UnorderedAccessView* m_LuminanceAverageBufferUAV;
	ID3D11ShaderResourceView* m_LuminanceAverageBufferSRV;
};

