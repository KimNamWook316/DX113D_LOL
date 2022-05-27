#include "CompositeNode.h"

CCompositeNode::CCompositeNode()
{
	SetTypeID(typeid(CCompositeNode).hash_code());
}

CCompositeNode::CCompositeNode(const CCompositeNode& Node)	:
	CNode(Node)
{
}

CCompositeNode::~CCompositeNode()
{

}


