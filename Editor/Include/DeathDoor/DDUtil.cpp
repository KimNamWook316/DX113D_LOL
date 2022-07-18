#include "DDUtil.h"

std::string CDDUtil::DDConditionNodeTypeToString(DDConditionNode NodeType)
{
	switch (NodeType)
	{
	case DDConditionNode::BossKnightFinalAttackCheck:
		return "BossKnightFinalAttackCheck";

	case DDConditionNode::BossKnightJumpAttackRangeCheck:
		return "BossKnightJumpAttackRangeCheck";

	case DDConditionNode::MeleeAttackRangeCheck:
		return "MeleeAttackRangeCheck";

	case DDConditionNode::PostAttackDelayCheck:
		return "PostAttackDelayCheck";
	}

	return "";
}

DDConditionNode CDDUtil::StringToDDConditionNodeType(const std::string& Str)
{
	if (Str == "BossKnightFinalAttackCheck")
	{
		return DDConditionNode::BossKnightFinalAttackCheck;
	}
	else if (Str == "BossKnightJumpAttackRangeCheck")
	{
		return DDConditionNode::BossKnightJumpAttackRangeCheck;
	}
	else if (Str == "MeleeAttackRangeCheck")
	{
		return DDConditionNode::MeleeAttackRangeCheck;
	}
	else if (Str == "PostAttackDelayCheck")
	{
		return DDConditionNode::PostAttackDelayCheck;
	}

	return DDConditionNode(-1);
}

std::string CDDUtil::DDActionNodeTypeToString(DDActionNode NodeType)
{
	switch (NodeType)
	{
	case DDActionNode::BossKnightContinueAttack:
		return "BossKnightContinueAttack";

	case DDActionNode::BossKnightFinalAttack:
		return "BossKnightFinalAttack";

	case DDActionNode::BossKnightJumpAttack:
		return "BossKnightJumpAttack";

	case DDActionNode::BossKnightMeleeAttack:
		return "BossKnightMeleeAttack";

	case DDActionNode::BossKnightSlamEnd:
		return "BossKnightSlamEnd";
	}

	return "";
}

DDActionNode CDDUtil::StringToDDActionNodeType(const std::string& Str)
{
	if (Str == "BossKnightFinalAttack")
	{
		return DDActionNode::BossKnightFinalAttack;
	}
	else if (Str == "BossKnightContinueAttack")
	{
		return DDActionNode::BossKnightContinueAttack;
	}
	else if (Str == "BossKnightJumpAttack")
	{
		return DDActionNode::BossKnightJumpAttack;
	}
	else if (Str == "BossKnightMeleeAttack")
	{
		return DDActionNode::BossKnightMeleeAttack;
	}
	else if (Str == "BossKnightSlamEnd")
	{
		return DDActionNode::BossKnightSlamEnd;
	}

	return DDActionNode(-1);
}

std::string CDDUtil::DDSceneComponentTypeToString(DDSceneComponentType Type)
{
	switch (Type)
	{
	}

	return "";
}

DDSceneComponentType CDDUtil::StringToDDSceneComponentType(const std::string& Str)
{
	return DDSceneComponentType();
}

std::string CDDUtil::DDObjectComponentTypeToString(DDObjectComponentType Type)
{
	switch (Type)
	{
	case DDObjectComponentType::MonsterData:
		return "MonsterData";

	case DDObjectComponentType::KnightData:
		return "KnightData";
	}

	return "";
}

DDObjectComponentType CDDUtil::StringToDDObjectComponentType(const std::string& Str)
{
	if (Str == "MonsterData")
	{
		return DDObjectComponentType::MonsterData;
	}
	else if (Str == "KnightData")
	{
		return DDObjectComponentType::KnightData;
	}

	return DDObjectComponentType(-1);
}
