#pragma once
template<typename Type>

class List
{
public:
	Type	elem = 0;
	List<Type>* head = nullptr;
	List<Type>* next = nullptr;

	List(const Type& elem)
	{
		head = this;
		this->elem = elem;
	};

	~List()
	{
		if (next)
			delete(next);
		next = nullptr;
	}

	int GetLenght()
	{
		List<Type>* Tmp = this->head;
		int count = 0;
		while (Tmp != nullptr)
		{
			count++;
			Tmp = Tmp->next;
		}
		return count;
	}

	void AddLast(const Type& elem)
	{
		if (next == nullptr)
			next = new List(elem);
		else
			next->AddLast(elem);
	}

	void AddFirst(const Type& elem)
	{
		auto Tmp = new List(elem);
		Tmp->next = this;
		this->head = Tmp;
	}

	List<Type>* Remove(const Type& elem)
	{
		if (this->elem == elem)
		{
			auto _next = next;
			this->next = nullptr;
			delete(this);
			if (_next != nullptr)
				return _next->Remove(elem);
			else
				return nullptr;
		}
		else
		{
			if (next)
				next = next->Remove(elem);
			return this;
		}
	}

	bool Contains(const Type* elem)
	{
		if (this->elem == elem)
			return true;
		else if (next == nullptr)
			return false;
		else
			return next->Contains(elem);
	}

	void ShowElemInList()
	{
		List<Type>* Tmp = this->head;
		while (Tmp != nullptr)
		{
			printf("%d\n", Tmp->elem);
			Tmp = Tmp->next;
		}
		delete(Tmp);
	}
};

typedef List<int> IntList;