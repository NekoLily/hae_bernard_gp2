#include <iostream>
#include "IntArray.h"
#include "List.h"
#include "Tree.h"


int main()
{
	/*IntList* IntList1 = new IntList(1);

	IntList1->AddFirst(0);
	IntList1->AddLast(2);
	IntList1->AddLast(3);
	IntList1->Remove(3);
	IntList1->ShowElemInList();
	printf("Lenght = %d\n", IntList1->GetLenght());*/

	IntTree* tree = new IntTree();
	Node<int>* leaf = new IntNode(8);

	tree->root = leaf;
	leaf->insert(4);
	leaf->insert(3);
	leaf->insert(6);
	leaf->insert(9);

	int a;
}
