#pragma once
#include <stdio.h>;
#include <string>;
class IntArray
{
public:
	static int Test;
	std::string name;
	int * data;
	int maxSize = 0;
	int curSize = 0;

	IntArray(int size, const char * name = "")
	{
		int i = 0;
		this->name = name;
		data = new int[size];
		//data = (int *)malloc(sizeof(int) * size);
		//data = (int*)calloc(size, sizeof(int));

		for (int i = 0; i < size; i++) data[i] = 0;
		maxSize = size;
		if (this->name.c_str())printf("Construire %s\n", this->name.c_str());
	}

	~IntArray()
	{
		printf("Detruire %s\n", this->name.c_str());
	}

	void	Ensure(int size);

	void SetPosUnsafe(int pos, int elem)
	{
		this->data[pos] = elem;
	}

	void SetPosSafe(int pos, int elem)
	{
		if (pos < maxSize)
			this->data[pos] = elem;
		else
		{
			Ensure(pos + 1);
			this->data[pos] = elem;
		}
	}

	void Push_Back(int elem)
	{
		Ensure(maxSize + 1);
		data[maxSize - 1] = elem;
	}

	void Push_Front(int elem)
	{
		Ensure(maxSize + 1);
		for (int i = maxSize - 1; i > 0; i--)
			this->data[i] = this->data[i - 1];
		this->data[0] = elem;
	}

	void Insert(int pos, int elem)
	{
		Ensure(pos <= maxSize ? maxSize + 1 : pos + 1);
		for (int i = maxSize - 1; i > pos; i--)
			this->data[i] = this->data[i - 1];
		data[pos] = elem;
	}

	int searchPosition(int elem)
	{
		for (int i = 0; i < maxSize; i++)
		{
			if (elem < data[i])
				return i;
		}
		return maxSize;
	}

	bool Remove(int Val)
	{
		if (Val < 0) return false;
		int Pos = -1;
		for (int i = 0; i < maxSize; i++)
		{
			if (this->data[i] == Val)
			{
				Pos = i;
				break;
			}
		}
		if (Pos == -1)
			return false;
		for (int i = Pos; i < maxSize; i++)
			this->data[i] = this->data[i + 1];
		this->data[maxSize - 1] = NULL;
		maxSize--;
		return true;
	}

	void RemoveAll()
	{
		for (int i = 0; i < maxSize; i++)
			this->data[i] = NULL;
		maxSize = 0;
	}

	void FillWithRandom(int Count) 
	{
		Ensure(Count);
		for (int i = 0; i < Count; ++i)
		{
			int rnd = std::rand();
			SetPosSafe(i, rnd);
		}
	};

	void Sort()
	{
		IntArray NewData(0, "NewData");
		for (int i = 0; i < maxSize; i++)
		{
			int Val = data[i];
			NewData.Insert(NewData.searchPosition(Val), Val);
		}
		for (int i = 0; i < NewData.maxSize; i++)
		{
			SetPosSafe(i, NewData.data[i]);
		}
	}

	void DisplayArray()
	{
		for (int i = 0; i < maxSize; i++)
			printf("%d\n", data[i]);
		printf("\n");
	}
};

