
#include "pch.h"
#include <iostream>
#include "IntArray.h"

int main()
{

	auto nameTab1 = "Tab0";
	auto nameTab2 = "Tab1";

	IntArray Tab0 = IntArray(4, nameTab1);
	
	/*Tab0.DisplayArray();
	Tab0.Push_Back(10);
	Tab0.DisplayArray();
	Tab0.Push_Front(2);
	Tab0.DisplayArray();
	Tab0.Push_Front(1);
	Tab0.DisplayArray();
	Tab0.Insert(2,3);
	Tab0.Insert(3, 4);
	Tab0.Insert(15, 5);
	Tab0.DisplayArray();*/
	
	Tab0.data[0] = 5;
	Tab0.data[1] = 15;
	Tab0.data[2] = 10;
	Tab0.data[3] = 12;
	Tab0.FillWithRandom(30);
	//printf("%d\n\n", Tab0.searchPosition(4));
	//printf("%d\n\n", Tab0.searchPosition(7));
	//Tab0.Remove(13);
	//Tab0.RemoveAll();
	Tab0.Sort();
	Tab0.DisplayArray();
	//IntArray *Tab1 = new IntArray(16, nameTab2);

	//printf("%d\n", IntArray::Test);
	

}
