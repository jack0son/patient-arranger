#include "PersonListSearch.h"

using namespace std;

vector<Person*>::iterator PersonListSearch::basic_search(Person* person, vector<Person*> * personList)
{
    for(int i=0; i<personList->size(); i++)
    {
		if(personList->at(i) == person)
			return personList->begin()+i;
	}
	return personList->end(); //beyond vector ie no result
}

