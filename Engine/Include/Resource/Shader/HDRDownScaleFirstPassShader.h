#pragma once

#include "ComputeShader.h"

class CHDRDownScaleFirstPathShader :
    public CComputeShader
{
    friend class CShaderManager;

private:
    CHDRDownScaleFirstPathShader();
    virtual ~CHDRDownScaleFirstPathShader();

public:
    virtual bool Init() override;
};

