typedef int keyType;

struct BTNode
{
	keyType key;
	struct BTNode  *lchild, *rchild;
};                                                                                                                                   

typedef  struct BTNode BTNode;
