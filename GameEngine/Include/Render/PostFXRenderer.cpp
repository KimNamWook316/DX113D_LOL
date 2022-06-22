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
	m_DownScaleFirstPathShader(nullptr),
	m_DownScaleSecondPathShader(nullptr),
	m_HDRRenderShader(nullptr),
	m_DownScaleCBuffer(nullptr),
	m_HDRRenderCBuffer(nullptr),
	m_DownScaleBuffer(nullptr),
	m_DownScaleUAV(nullptr),
	m_DownScaleSRV(nullptr),
	m_LuminanceAverageBuffer(nullptr),
	m_LuminanceAverageBufferUAV(nullptr),
	m_LuminanceAverageBufferSRV(nullptr)
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

	// Luminance Average Resources
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

	// DownScale Constant Buffer
	Resolution DownScaleRS;
	DownScaleRS.Width = RS.Width / 4;
	DownScaleRS.Height = RS.Height / 4;

	m_DownScaleCBuffer = new CDownScaleCBuffer;
	m_DownScaleCBuffer->Init();
	m_DownScaleCBuffer->SetResoluation(DownScaleRS);
	m_DownScaleCBuffer->SetDomain(PixelCount / 16);
	m_DownScaleCBuffer->SetGroupSize(GroupCount);

	// Render Constant Buffer
	m_HDRRenderCBuffer = new CHDRRenderCBuffer;
	m_HDRRenderCBuffer->Init();

	// Shader
	m_DownScaleFirstPathShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("HDRDownScaleFirstPathShader");
	m_DownScaleSecondPathShader = (CComputeShader*)CResourceManager::GetInst()->FindShader("HDRDownScaleSecondPathShader");
	m_HDRRenderShader = CResourceManager::GetInst()->FindShader("HDRRenderShader");

	return true;
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
	/////////////////
	// First  Path //
	/////////////////

	// HDR Target
	LDRTarget->SetShader(120, (int)Buffer_Shader_Type::Compute, 0);

	// CBuffer
	m_DownScaleCBuffer->UpdateCBuffer();

	// DownScale 결과 버퍼
	unsigned int Count = -1;
	CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(0, 1, &m_DownScaleUAV, &Count);

	// Excute
	m_DownScaleFirstPathShader->SetShader();
	unsigned int GroupSize = m_DownScaleCBuffer->GetGroupSize();
	m_DownScaleFirstPathShader->Excute(GroupSize, 0, 0);


	ID3D11UnorderedAccessView* UAV = nullptr;
	CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(0, 1, &UAV, &Count);

	/////////////////
	// Second Path //
	/////////////////

	// 씬 평균 휘도를 하나의 픽셀에 저장하기 위한 Output Buffer
	CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(0, 1, &m_LuminanceAverageBufferUAV, &Count);

	// CBuffer
	m_DownScaleCBuffer->UpdateCBuffer();

	// First Path에서 64개 이하로 압축된 씬 휘도 정보
	CDevice::GetInst()->GetContext()->CSSetShaderResources(21, 1, &m_DownScaleSRV);

	// Excute
	m_DownScaleSecondPathShader->SetShader();
	m_DownScaleSecondPathShader->Excute(1, 1, 1);

	CDevice::GetInst()->GetContext()->CSSetUnorderedAccessViews(0, 1, &UAV, &Count);

	ID3D11ShaderResourceView* SRV = nullptr;
	CDevice::GetInst()->GetContext()->CSSetShaderResources(21, 1, &SRV);

	// !!!!!! 두번째 패스 돌아가기 전까지 해제하면 안됨 !!!!!!!
	LDRTarget->ResetShader(120, (int)Buffer_Shader_Type::Compute, 0);
}

void CPostFXRenderer::Render(CRenderTarget* LDRTarget, class CRenderState* DepthDisable)
{
	m_HDRRenderCBuffer->UpdateCBuffer();

	// HDR로 변환할 LDR 렌더타겟
	LDRTarget->SetTargetShader(10);

	// 컴퓨트 쉐이더에서 계산한 씬 휘도 정보
	CDevice::GetInst()->GetContext()->PSSetShaderResources(11, 1, &m_LuminanceAverageBufferSRV);

	m_HDRRenderShader->SetShader();
	
	DepthDisable->SetState();

	// Null Buffer 출력
	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	CDevice::GetInst()->GetContext()->Draw(4, 0);

	// Reset 
	DepthDisable->ResetState();

	ID3D11ShaderResourceView* SRV = nullptr;
	CDevice::GetInst()->GetContext()->PSSetShaderResources(11, 1, &SRV);

	LDRTarget->ResetTargetShader(10);
}
