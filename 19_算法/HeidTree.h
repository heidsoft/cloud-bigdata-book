/*
	HeidTree.h

   二叉树的二叉链表 结点存储结构
 */
 typedef int elemType; //将int 定义为元素类型

 typedef struct BiNode  *BiTree;//将结构体类型重新定义为简单类型
 typedef struct BiNode  BiNode;//将结构体类型重新定义为简单类型

 struct BiNode{
	 elemType data;// 结点数据域
	 struct BiNode  *lChildNode, *rChildNod;//结点指针域
 };

 /*
	定义二叉树的基本操作
 */

 int InitiateTree(BiTree bt);//建立一颗空二叉树,仅仅初始化头结点

 int Create(elemType x, BiTree lbt,BiTree rbt);
