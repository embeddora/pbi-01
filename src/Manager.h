#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Types.h"

#include <stdio.h>

#include <stdlib.h>

#include <string>

#include <vector>

#include <list>

#include <map>

#include <hash_map>

#include "Errors.h"

namespace std { using namespace __gnu_cxx; }




/* Менеджер ресурсов (громко сказано, это только набросок) */
//template <class T> class newManager
template <typename T> class newManager
{
public:

	newManager(void)
	{
	}

	~newManager(void)
	{
	}



	/* Глобальный массив */
	typedef std::map<newString, T> Items;

	Items Item;

	T *newNewPItem(const newString &Name)
	{
		if (Item.find(Name)==Item.end())
		{
			Item[Name]=NULL;

			/* Item[Name]->Name=Name; */
		}
		return &Item[Name];
	}

	T *newNewItem(const newString &Name)
	{
		if (Item.find(Name)==Item.end())
		{
			Item[Name]=T();

			Item[Name].Name=Name;
		}

		return &Item[Name];
	}

	/* Генерируем имя */
	newString newGenItemName()
	{
	static char buf[255];

	int i=0;

		sprintf(buf,"DefaultItem%d",i++);

		while (Item.find(buf)!=Item.end())
		{
			sprintf(buf,"DefaultItem%d",i++);
		}

		return buf;
	}

	/* Новый элемент со сгенерированым именем */
	T *newNewItem()
	{
		newString Name = newGenItemName();

		if (Item.find(Name)==Item.end())
		{
			Item[Name]=T();

			Item[Name].Name=Name;
		}
		return &Item[Name];
	}
};




template <class T> class newHashManager
{
public:
	newHashManager(void)
	{
	}

	~newHashManager(void)
	{
	}

	/* Глобальный массив */
	typedef std::hash_map<UINT, T> Items;


	Items Item;

//	Items::iterator Iter;

//	Items::const_iterator ConstIter;

	T GetFirst()
	{
//		Iter = Item.begin();
//		return ((Iter++)->second);
		return NULL;
	}

	T GetNext()
	{
/*		if (Iter == Item.end())

			return NULL;

		else

			return ((Iter++)->second);
*/
		return NULL;
	}

	T *newNewItem(const UINT &Name)
	{
#if 0
		if (Item.find(Name)==Item.end())
		{
//			Item[Name]=T();

			/* Item[Name].Name=Name; */
		}

		return &Item[Name];
#endif /* (0) */
		return NULL;
	}

	/* Генерируем имя */
	newString newGenItemName()
	{
	static char buf[255];

	int i=0;
/*
		sprintf(buf,"DefaultItem%d",i++);

		while (Item.find(buf)!=Item.end())
		{
			sprintf(buf,"DefaultItem%d",i++);
		}
*/
		return buf;
	}

	/* Новый элемент со сгенерированым именем */
	T *newNewItem()
	{
		newString Name = newGenItemName();

/*		if (Item.find(Name)==Item.end())
		{
			Item[Name]=T();

			Item[Name].Name=Name;
		}
		return &Item[Name];
*/
		return NULL;
	}
};

#endif /* ifndef-define include guard */
