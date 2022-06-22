#include "HDRDownScaleFirstPassShader.h"

CHDRDownScaleFirstPathShader::CHDRDownScaleFirstPathShader()
{
	SetTypeID<CHDRDownScaleFirstPathShader>();
}

CHDRDownScaleFirstPathShader::~CHDRDownScaleFirstPathShader()
{
}

bool CHDRDownScaleFirstPathShader::Init()
{
	if (!LoadComputeShader("DownScaleSecondPass", TEXT("HDRDownScale.fx"), SHADER_PATH))
		return false;

	return true;
}
