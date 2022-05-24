#include "EngineUtil.h"

std::string CEngineUtil::LightTypeToString(Light_Type Type)
{
	std::string ret = "";
	switch (Type)
	{
	case Light_Type::Dir:
		ret = "Dir";
		break;
	case Light_Type::Point:
		ret = "Point";
		break;
	case Light_Type::Spot:
		ret = "Spot";
		break;
	}
	return ret;
}

Light_Type CEngineUtil::StringToLightType(const std::string& TypeString)
{
	if (TypeString == "Dir")
	{
		return Light_Type::Dir;
	}
	else if (TypeString == "Point")
	{
		return Light_Type::Point;
	}
	else if (TypeString == "Spot")
	{
		return Light_Type::Spot;
	}
	return (Light_Type)(-1);
}
