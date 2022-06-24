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
	void Adaptation(float DeltaTime);

public:
	void SetMiddleGray(float Gray);
	void SetLumWhite(float White);
	float GetMiddleGray() const;
	float GetLumWhite() const;

	void SetAdaptationTime(float Time)
	{
		m_AdaptationTime = Time;
	}

	float GetAdaptationTime() const
	{
		return m_AdaptationTime;
	}

public:
	void ExcuteDownScale(class CRenderTarget* LDRTarget);
	void Render(class CRenderTarget* LDRTarget, class CRenderState* DepthDisable);

private:
	// Shader
	class CComputeShader* m_DownScaleFirstPassShader;
	class CComputeShader* m_DownScaleSecondPassShader;
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

	ID3D11Buffer* m_PrevFrameLumAverageBuffer;
	ID3D11UnorderedAccessView* m_PrevFrameLumAverageBufferUAV;
	ID3D11ShaderResourceView* m_PrevFrameLumAverageBufferSRV;

	// Adaptation ฐüทร 
	bool  m_FirstFrame;
	float m_AdaptationTimer;
	float m_AdaptationTime;

	// DEBUG SECTION
 //	ID3D11Buffer* m_Temp;
 //	ID3D11Buffer* m_Temp2;
 //	ID3D11UnorderedAccessView* m_Temp2UAV;
 //	float m_TempBuf[57];
 //	ID3D11Query* m_Query;
	// DEBUG SECTION END
};

