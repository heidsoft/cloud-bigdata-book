#include "HeidTree.h"
#include <stdlib.h>

/*创建一颗空的二叉树*/
int InitiateTree(BiTree bt)
{

	bt = (BiNode *)malloc(sizeof(BiNode));

	if (NULL == bt){
		return NULL;
	}

	bt->lChildNode = NULL;
	bt->rChildNod = NULL;
	return  0;
}

/*创建一棵二叉树*/
int Create(elemType x, BiTree lbt, BiTree rbt)
{

	BiTree p;

	p = (BiNode *)malloc(sizeof(BiNode));

	if (NULL==p){
		return NULL;
	}

	p->lChildNode = lbt;//赋值左子树

	p->rChildNod = rbt;//赋值右子树

	return 0;
}
