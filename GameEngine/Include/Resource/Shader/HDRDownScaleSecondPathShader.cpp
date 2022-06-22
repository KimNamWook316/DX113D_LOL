#include "HDRDownScaleSecondPathShader.h"

CHDRDownScaleSecondPathShader::CHDRDownScaleSecondPathShader()
{
	SetTypeID<CHDRDownScaleSecondPathShader>();
}

CHDRDownScaleSecondPathShader::~CHDRDownScaleSecondPathShader()
{
}

bool CHDRDownScaleSecondPathShader::Init()
{
	if (!LoadComputeShader("DownScaleFirstPass", TEXT("HDRDownScale.fx"), SHADER_PATH))
		return false;

	return true;
}