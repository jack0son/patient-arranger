#ifndef PERSONLISTSEARCH_H
#define PERSONLISTSEARCH_H

#include "Person.h"
#include <vector>

namespace PersonListSearch
{
	std::vector<Person*>::iterator basic_search(Person* person, std::vector<Person*> * personList);
};

#endif
