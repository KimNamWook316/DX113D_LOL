#pragma once

#include "GameInfo.h"

class CEngineUtil
{
public:
	static std::string LightTypeToString(Light_Type Type);
	static Light_Type StringToLightType(const std::string& TypeString);

};

