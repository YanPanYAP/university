#pragma once

#include <Windows.h>
#include <iostream>
#include <cstring>

#define DICTNAMEMAXSIZE 20	// максимальный размер имени словар€ 
#define DICTMAXSIZE 100	// максимальна€ Ємкость словар€
#define ENTRYNAMEMAXSIZE 30 // максимальна€ длина имени в словаре
#define THROW1 "Create: превышен размер имени словар€"
#define THROW2 "Create: превышен размер максимального емкости словар€"
#define THROW3 "AddEntry: переполнение словар€"
#define THROW4 "AddEntry: дублирование индетификатора"
#define THROW5 "GetEntry:  не найден элемент"
#define THROW6 "DelEntry:  не найден элемент"
#define THROW7 "UpdEntry:  не найден элемент"
#define THROW8 "UpdEntry: дублирование индетификатора"

namespace Dictionary {
	struct Entry { // элемент словар€
		int	id;						// идентификатор (уникальный)
		char name[ENTRYNAMEMAXSIZE];	// символьна€ информаци€ (название элемента)
	};

	struct Instance {
		char name[DICTNAMEMAXSIZE];		// наименование словар€
		int maxsize;					// максимальный Ємкость словар€
		int size;						// текущий размер словар€ < DICTMAXSIZE
		Entry* dictionary[DICTMAXSIZE];			// массив элементов словар€ 
	};

	Instance Create( //создать словарь
		char name[DICTNAMEMAXSIZE],// им€ словар€ 
		int size // емкость словар€ < DICTNAMEMAXSIZE
	);

	void AddEntry( //добавить элемент словар€
		Instance& inst, // экземпл€р словар€ 
		Entry ed //элемент словар€ 
	);

	void DelEntry( //удалить элемент словар€
		Instance& inst,// экземпл€р словар€ 
		int id // экземпл€р удал€емого элемента (уникальный)
	);

	void UpdEntry(//изменить элемент словар€
		Instance& inst,//экзепл€р словар€ 
		int id, // идентификатор замен€емого слова 
		Entry new_ed // новый элемент словар€ 
	);

	Entry GetEntry(//получить элемент словар€ 
		Instance& inst,//экзепл€р словар€ 
		int id// идентификатор получаемого элемента 
	);

	void Print(Instance& d);
	void Delete(Instance& d);
}