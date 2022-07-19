#pragma once

#include "GameInfo.h"
#include "DDFlag.h"
#include "Flag.h"

class CDDUtil
{
public:
	// TODO : Death Door Condition Node Type 추가시마다 추가
	static std::string DDConditionNodeTypeToString(DDConditionNode NodeType);
	static DDConditionNode StringToDDConditionNodeType(const std::string& Str);

	// TODO : Death Door Action Node Type 추가시마다 추가
	static std::string DDActionNodeTypeToString(DDActionNode NodeType);
	static DDActionNode StringToDDActionNodeType(const std::string& Str);

	// TODO : Death Door SceneComponent 추가시마다 추가
	static std::string DDSceneComponentTypeToString(DDSceneComponentType Type);
	static DDSceneComponentType StringToDDSceneComponentType(const std::string& Str);

	// TODO : Death Door ObjectComponent 추가시마다 추가
	static std::string DDObjectComponentTypeToString(DDObjectComponentType Type);
	static DDObjectComponentType StringToDDObjectComponentType(const std::string& Str);
};

