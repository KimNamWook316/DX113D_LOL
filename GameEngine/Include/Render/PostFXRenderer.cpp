#include "PostFXRenderer.h"
#include "../Device.h"
#include "../Engine.h"
#include "../Resource/Shader/HDRRenderCBuffer.h"
#include "../Resource/Shader/DownScaleCBuffer.h"
#include "../Resource/Shader/ComputeShader.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture/RenderTarget.h"
#include "RenderState.h"
#include "RenderManager.h"

CPostFXRenderer::CPostFXRenderer()	:
	m_DownScaleFirstPassShader(nullptr),
	m_DownScaleSecondPassShader(nullptr),
	m_HDRRenderShader(nullptr),
	m_DownScaleCBuffer(nullptr),
	m_HDRRenderCBuffer(nullptr),
	m_DownScaleBuffer(nullptr),
	m_DownScaleUAV(nullptr),
	m_DownScaleSRV(nullptr),
	m_LuminanceAverageBuffer(nullptr),
	m_LuminanceAverageBufferUAV(nullptr),
	m_LuminanceAverageBufferSRV(nullptr),
	m_PrevFrameLumAverageBuffer(nullptr),
	m_PrevFrameLumAverageBufferUAV(nullptr),
	m_PrevFrameLumAverageBufferSRV(nullptr),
	m_AdaptationTime(3.f),
	m_FirstFrame(true)
{
}

CPostFXRenderer::~CPostFXRenderer()
{
	SAFE_DELETE(m_DownScaleCBuffer);
	SAFE_DELETE(m_HDRRenderCBuffer);

	SAFE_RELEASE(m_DownScaleBuffer);
	SAFE_RELEASE(m_DownScaleUAV);
	SAFE_RELEASE(m_DownScaleSRV);

	SAFE_RELEASE(m_LuminanceAverageBuffer);
	SAFE_RELEASE(m_LuminanceAverageBufferUAV);
	SAFE_RELEASE(m_LuminanceAverageBufferSRV);

	SAFE_RELEASE(m_PrevFrameLumAverageBuffer);
	SAFE_RELEASE(m_PrevFrameLumAverageBufferUAV);
	SAFE_RELEASE(m_PrevFrameLumAverageBufferSRV);

 //	SAFE_RELEASE(m_Query);
 //	SAFE_RELEASE(m_Temp);
 //	SAFE_RELEASE(m_Temp2);
 //	SAFE_RELEASE(m_Temp2UAV);
}

bool CPostFXRenderer::Init()
{
	Resolution RS = CDevice::GetInst()->GetResolution();
	unsigned int PixelCount = RS.Width * RS.Height;
	unsigned int GroupCount = (unsigned int)ceil((float)(PixelCount / 16.f) / 1024.f);

	// DownScale Luminance Buffer
	D3D11_BUFFER_DESC DownScaleBufferDesc = {};
	DownScaleBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	DownScaleBufferDesc.StructureByteStride = sizeof(float);
	DownScaleBufferDesc.ByteWidth = GroupCount * DownScaleBufferDesc.StructureByteStride;
	DownScaleBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	DownScaleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&DownScaleBufferDesc, nullptr, &m_DownScaleBuffer)))
	{
		assert(false);
		return false;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC DownScaleUAVDesc = {};
	DownScaleUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	DownScaleUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	DownScaleUAVDesc.Buffer.NumElements = GroupCount;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_DownScaleBuffer, &DownScaleUAVDesc, &m_DownScaleUAV)))
	{
		assert(false);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC DownScaleSRVDesc = {};
	DownScaleSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	DownScaleSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	DownScaleSRVDesc.Buffer.NumElements = GroupCount;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_DownScaleBuffer, &DownScaleSRVDesc, &m_DownScaleSRV)))
	{
		assert(false);
		return false;
	}

	// Average Luminance Buffer
	D3D11_BUFFER_DESC AverageBufferDesc = {};
	AverageBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	AverageBufferDesc.StructureByteStride = sizeof(float);
	AverageBufferDesc.ByteWidth = sizeof(float);
	AverageBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	AverageBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&AverageBufferDesc, nullptr, &m_LuminanceAverageBuffer)))
	{
		assert(false);
		return false;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC AverageUAVDesc = {};
	AverageUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	AverageUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	AverageUAVDesc.Buffer.NumElements = 1;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_LuminanceAverageBuffer, &AverageUAVDesc, &m_LuminanceAverageBufferUAV)))
	{
		assert(false);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC AverageSRVDesc = {};
	AverageSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	AverageSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	AverageSRVDesc.Buffer.NumElements = 1;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_LuminanceAverageBuffer, &AverageSRVDesc, &m_LuminanceAverageBufferSRV)))
	{
		assert(false);
		return false;
	}

	// Prev Frame Luminance Resource ( Same As Above )
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&AverageBufferDesc, nullptr, &m_PrevFrameLumAverageBuffer)))
	{
		assert(false);
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_PrevFrameLumAverageBuffer, &AverageUAVDesc, &m_PrevFrameLumAverageBufferUAV)))
	{
		assert(false);
		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_PrevFrameLumAverageBuffer, &AverageSRVDesc, &m_PrevFrameLumAverageBufferSRV)))
	{
		assert(false);
		return false;
	}

 //	///  Debug Section
 //
 //	// Temp
 //	D3D11_BUFFER_DESC TempBufferDes = {};
 //	TempBufferDes.StructureByteStride = sizeof(float);
 //	TempBufferDes.ByteWidth = sizeof(float);
 //	TempBufferDes.Usage = D3D11_USAGE_STAGING;
 //	TempBufferDes.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
 //	TempBufferDes.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
 //
 //	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&TempBufferDes, nullptr, &m_Temp)))
 //	{
 //		assert(false);
 //		return false;
 //	}
 //
 //	TempBufferDes.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
 //	TempBufferDes.StructureByteStride = sizeof(float);
 //	TempBufferDes.ByteWidth = sizeof(float);
 //	TempBufferDes.Usage = D3D11_USAGE_DEFAULT;
 //	TempBufferDes.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
 //	TempBufferDes.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
 //
 //	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&TempBufferDes, nullptr, &m_Temp2)))
 //	{
 //		assert(false);
 //		return false;
 //	}
 //
 //	AverageUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
 //	AverageUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
 //	AverageUAVDesc.Buffer.NumElements = 1;
 //
 //	if (FAILED(CDevice::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Temp2, &AverageUAVDesc, &m_Temp2UAV)))
 //	{
 //		assert(false);
 //		return false;
 //	}
 //
 //	
 //	D3D11_QUERY_DESC QueryDesc = {};
 //	QueryDesc.Query = D3D11_QUERY_EVENT;
 //	QueryDesc.MiscFlags = 0;
 //	CDevice::GetInst()->GetDevice()->CreateQuery(&QueryDesc, &m_Query);
 //
 //	///  Debug Section End

	// DownScale Constant Buffer
	Resolution DownScaleRS;
	DownScaleRS.Width = RS.Width / 4;
	DownScaleRS.Height = RS.Height / 4;

	m_DownScaleCBuffer = new CDownScaleCBuffer;
	m_DownScaleCBuffer->Init();
	m_DownScaleCBuffer->SetResolution(DownScaleRS);
	m_DownScaleCBuffer->SetDomain(PixelCount / 16);
	m_DownScaleCBuffer->SetGroupSize(GroupCount);

	// Render Constant Buffer
	m_HDRRenderCBuffer = new CHDRRenderCBuffer;
	m_HDRRenderCBuffer->Init();

	// Shader
	m_DownScaleFirstPassShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("HDRDownScaleFirstPassShader");
	m_DownScaleSecondPassShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("HDRDownScaleSecondPassShader");
	m_HDRRenderShader = CResourceManager::GetInst()->FindShader("HDRRenderShader");

	return true;
}

void CPostFXRenderer::Adaptation(float DeltaTime)
{
	// Adaptation 계수 계산
	float AdaptNorm;

	m_AdaptationTimer += DeltaTime;

	if (m_FirstFrame)
	{
		AdaptNorm = 1.f;
		m_FirstFrame = false;
	}
	else
	{
		if (m_AdaptationTimer >= m_AdaptationTime)
		{
			m_AdaptationTimer = 0.f;
		}
		
		AdaptNorm = m_AdaptationTime > 0.0001f ? (m_AdaptationTimer / m_AdaptationTime) : 0.9999f;
	}

	m_DownScaleCBuffer->SetAdaptation(AdaptNorm);
}

void CPostFXRenderer::SetMiddleGray(float Gray)
{
	m_HDRRenderCBuffer->SetMiddleGray(Gray);
}

void CPostFXRenderer::SetLumWhite(float White)
{
	m_HDRRenderCBuffer->SetLumWhite(White);
}

float CPostFXRenderer::GetMiddleGray() const
{
	return m_HDRRenderCBuffer->GetMiddleGray();
}

float CPostFXRenderer::GetLumWhite() const
{
	return m_HDRRenderCBuffer->GetLumWhite();
}

void CPostFXRenderer::ExcuteDownScale(class CRenderTarget* LDRTarget)
{
	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	/////////////////
	// First  Path //
	/////////////////

	// HDR Target
	LDRTarget->SetShader(120, (int)Buffer_Shader_Type::Compute, 0);

	// CBuffer
	m_DownScaleCBuffer->UpdateCBuffer();

	// DownScale 결과 버퍼
	unsigned int Count = -1;
	Context->CSSetUnorderedAccessViews(7, 1, &m_DownScaleUAV, &Count);

	// Context->CSSetUnorderedAccessViews(6, 1, &m_Temp2UAV, &Count);

	// Excute
	m_DownScaleFirstPassShader->SetShader();
	unsigned int GroupSize = m_DownScaleCBuffer->GetGroupSize();
	m_DownScaleFirstPassShader->Excute(GroupSize, 1, 1);

	// DEBUG
	 //Context->End(m_Query);
 //
 //	while(Context->GetData(m_Query, NULL, 0, 0) == S_FALSE)
 //	{
 //		int a = 1;
 //	}
 //
	// DEBUG
	ID3D11UnorderedAccessView* UAV = nullptr;
	Context->CSSetUnorderedAccessViews(7, 1, &UAV, &Count);
	//Context->CSSetUnorderedAccessViews(6, 1, &UAV, &Count);

	// Context->CopyResource(m_Temp2, m_DownScaleBuffer);
 //	D3D11_MAPPED_SUBRESOURCE Map2 = {};
 //	Context->Map(m_DownScaleBuffer, 0, D3D11_MAP_READ, 0, &Map2);
 //	memcpy(m_TempBuf, Map2.pData, (UINT)sizeof(float) * 57);
 //	Context->Unmap(m_DownScaleBuffer, 0);

 //	D3D11_MAPPED_SUBRESOURCE Map2 = {};
 //	float Temp2 = 2.f;
 //	Context->Map(m_Temp2, 0, D3D11_MAP_READ, 0, &Map2);
 //	memcpy(&Temp2, Map2.pData, sizeof(float));
 //	Context->Unmap(m_Temp2, 0);

	/////////////////
	// Second Path //
	/////////////////

	// 씬 평균 휘도를 하나의 픽셀에 저장하기 위한 Output Buffer
	Context->CSSetUnorderedAccessViews(7, 1, &m_LuminanceAverageBufferUAV, &Count);

	// 이전 프레임 휘도 정보
	Context->CSSetShaderResources(22, 1, &m_PrevFrameLumAverageBufferSRV);

	// CBuffer
	m_DownScaleCBuffer->UpdateCBuffer();

	// First Path에서 64개 이하로 압축된 씬 휘도 정보
	Context->CSSetShaderResources(21, 1, &m_DownScaleSRV);

	// Excute
	m_DownScaleSecondPassShader->SetShader();
	m_DownScaleSecondPassShader->Excute(1, 1, 1);

 //	// DEBUG
 //	Context->End(m_Query);
 //
 //	while(Context->GetData(m_Query, NULL, 0, 0) == S_FALSE)
 //	{
 //		int a = 1;
 //	}
 //
 //	// DEBUG

	// Reset
	Context->CSSetUnorderedAccessViews(7, 1, &UAV, &Count);

	ID3D11ShaderResourceView* SRV = nullptr;
	Context->CSSetShaderResources(21, 1, &SRV);
	Context->CSSetShaderResources(22, 1, &SRV);

	// !!!!!! 두번째 패스 돌아가기 전까지 해제하면 안됨 !!!!!!!
	LDRTarget->ResetShader(120, (int)Buffer_Shader_Type::Compute, 0);
	
 //	Context->CopyResource(m_Temp, m_LuminanceAverageBuffer);
 //	D3D11_MAPPED_SUBRESOURCE Map = {};
 //	float CurLumAv;
 //	Context->Map(m_Temp, 0, D3D11_MAP_READ, 0, &Map);
 //	memcpy(&CurLumAv, Map.pData, sizeof(float));
 //	Context->Unmap(m_Temp, 0);
}

void CPostFXRenderer::Render(CRenderTarget* LDRTarget, class CRenderState* DepthDisable)
{
	ID3D11DeviceContext* Context = CDevice::GetInst()->GetContext();

	m_HDRRenderCBuffer->UpdateCBuffer();

	// HDR로 변환할 LDR 렌더타겟
	LDRTarget->SetTargetShader(10);

	// 컴퓨트 쉐이더에서 계산한 씬 휘도 정보
	Context->PSSetShaderResources(11, 1, &m_LuminanceAverageBufferSRV);

	m_HDRRenderShader->SetShader();
	
	DepthDisable->SetState();

	// Null Buffer 출력
	UINT Offset = 0;
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Context->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	Context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	Context->Draw(4, 0);

	// Reset 
	DepthDisable->ResetState();

	ID3D11ShaderResourceView* SRV = nullptr;
	Context->PSSetShaderResources(11, 1, &SRV);

	LDRTarget->ResetTargetShader(10);

	// 현재 프레임 평균 휘도를 저장
	ID3D11Buffer* TempBuffer = m_PrevFrameLumAverageBuffer;
	ID3D11UnorderedAccessView* TempUAV = m_PrevFrameLumAverageBufferUAV;
	ID3D11ShaderResourceView* TempSRV = m_PrevFrameLumAverageBufferSRV;

	m_PrevFrameLumAverageBuffer = m_LuminanceAverageBuffer;
	m_PrevFrameLumAverageBufferUAV = m_LuminanceAverageBufferUAV;
	m_PrevFrameLumAverageBufferSRV = m_LuminanceAverageBufferSRV;

	m_LuminanceAverageBuffer = TempBuffer;
	m_LuminanceAverageBufferUAV = TempUAV;
	m_LuminanceAverageBufferSRV = TempSRV;

 //	D3D11_MAPPED_SUBRESOURCE Map = {};
 //	float CurLumAv;
 //	Context->Map(m_LuminanceAverageBuffer, 0, D3D11_MAP_READ, 0, &Map);
 //	memcpy(&CurLumAv, Map.pData, sizeof(float));
 //	Context->Unmap(m_LuminanceAverageBuffer, 0);
 //
 //	D3D11_MAPPED_SUBRESOURCE Map2 = {};
 //	float PrevLumAv;
 //	Context->Map(m_PrevFrameLumAverageBuffer, 0, D3D11_MAP_READ, 0, &Map2);
 //	memcpy(&PrevLumAv, Map2.pData, sizeof(float));
 //	Context->Unmap(m_PrevFrameLumAverageBuffer, 0);
 //
 //	if (abs(PrevLumAv - CurLumAv) > 0.2)
 //	{
 //		int a = 0;
 //	}
}
