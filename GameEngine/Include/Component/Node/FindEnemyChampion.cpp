
#include "FindEnemyChampion.h"
#include "../../Input.h"
#include "../BehaviorTree.h"

CFindEnemyChampion::CFindEnemyChampion()
{
	SetTypeID(typeid(CFindEnemyChampion).hash_code());
}

CFindEnemyChampion::CFindEnemyChampion(const CFindEnemyChampion& Node) :
	CConditionNode(Node)
{
}

CFindEnemyChampion::~CFindEnemyChampion()
{
}

NodeResult CFindEnemyChampion::OnStart(float DeltaTime)
{
	return NodeResult::Node_True;


}

NodeResult CFindEnemyChampion::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CFindEnemyChampion::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CFindEnemyChampion::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
