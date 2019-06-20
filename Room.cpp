#include "Room.h"

using namespace ErrorCode;
using namespace std;

//default ctr
Room::Room(int a_maxPeople, int a_maxWeight, int a_numPeople, vector<Person*> * a_personList, bool a_TRACE) :
    maxPeople(a_maxPeople),
    maxWeight(a_maxWeight),
    numPeople(a_numPeople),
    personList(a_personList),
    TRACE(a_TRACE)
{}

//ctr with iterator assignment
Room::Room(vector<Person*>::iterator a_frontIt, vector<Person*>::iterator a_backIt, vector<Person*> * a_personList, int a_maxPeople, int a_maxWeight, int a_numPeople, bool a_TRACE) :
	maxPeople(a_maxPeople),
	maxWeight(a_maxWeight),
	numPeople(a_numPeople),
	frontIt(a_frontIt),
	backIt(a_backIt),
	personList(a_personList),
    TRACE(a_TRACE)
{}

Room::~Room()
{}

void Room::setFrontIt(vector<Person*>::iterator personIt)
{
    frontIt = personIt;
    frontRef = *personIt;
}

void Room::setBackIt(vector<Person*>::iterator personIt)
{
    backIt = personIt;
    backRef = *personIt;
}

ERROR_CODE Room::setBounds(vector<Person*>::iterator front,vector<Person*>::iterator back, int amount)
{
    if(amount > maxPeople)
    {
        return TOO_MANY_PEOPLE_FOR_ROOM;
    }
    if(front == personList->end() || back == personList->end())
    {
        return INVALID_BOUND;
    }
   	setFrontIt(front);
    setBackIt(back);
    numPeople = amount;
    return SUCCESS;
}

ERROR_CODE Room::addPerson(vector<Person*>::iterator personIt)
{
    if(numPeople >= maxPeople)
    {
        return ROOM_FULL;
    }
    else if(*(*personIt) < *(*frontIt))
    {
        setFrontIt(personIt);
    }
    else if(*(*personIt) > *(*backIt))
    {
        setBackIt(personIt);
    }
    else if(*(*personIt) == *(*backIt) || *(*personIt) == *(*frontIt))
    {
        numPeople--;
    }
    //std::cout << "\t\t--addedtoward " << person->getLastName() << std::endl;
    numPeople++;
    return SUCCESS;
}

ERROR_CODE Room::removePerson(vector<Person*>::iterator personIt)
{
    if(personIt < frontIt || personIt > backIt)
    {
        return PERSON_NOT_EXIST;
    }
    if(numPeople <= 0)
    {
        return ROOM_EMPTY;
    }
    if(personIt == frontIt)
    {
        setFrontIt(frontIt+1);
    }
    else if(personIt == backIt)
    {
        setBackIt(backIt-1);
    }
    //check if rooms can be removed?
    numPeople--;
    return SUCCESS;
}

ERROR_CODE Room::updateBoundIterators()
{
	frontIt = getPersonIterator(frontRef);
    backIt = getPersonIterator(backRef);
    if(frontIt == (*personList).end() || backIt == (*personList).end())
    {
        return BOUND_IT_NOT_FOUND;
    }
    return SUCCESS;
}

vector<Person*>::iterator Room::getPersonIterator(Person* person)
{
    return PersonListSearch::basic_search(person, personList);
}

vector<Person*>::iterator Room::getFrontIt()
{
    return frontIt;
}

vector<Person*>::iterator Room::getBackIt()
{
    return backIt;
}

int Room::getNumPeople()
{
    return numPeople;
}

bool Room::full()
{
    if(numPeople >= maxPeople)
    {
        return true;
    }
    return false;
}

string Room::print(string spacer)
{
    ostringstream sout;
    vector<Person*>::iterator temp = frontIt;
    //sout << spacer << "Num. people: " << numPeople << endl;
    int i = 0;
    do
    {
        sout << spacer << "Person " << i << ": ";
        sout << " " << (*temp)->print() << endl;
        //cout << "printing...";
        temp++;
        i++;
    }
    while(temp <= backIt);
    //sout << spacer << "Front: " << (*frontIt)->print() << endl;
    //sout << spacer << "Back: " << (*backIt)->print() << endl;
    return sout.str();
}
