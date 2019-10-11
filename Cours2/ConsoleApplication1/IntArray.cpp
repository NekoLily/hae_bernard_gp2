#include "pch.h"
#include "IntArray.h"

int IntArray::Test = 66;

void IntArray::Ensure(int size)
{
	if (size > maxSize)
	{
		curSize = maxSize;
		int *Tmp = data;
		data = new int[size];
		for (int i = 0; i < maxSize; i++)
			data[i] = Tmp[i];
		maxSize = size;
		delete(Tmp);
		for (curSize; curSize < maxSize; curSize++)
			data[curSize] = 0;
	}

}
