#ifndef ROOM_H
#define ROOM_H

#include "Person.h"
#include "ErrorCode.h"
#include "PersonListSearch.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using ErrorCode::ERROR_CODE;
using std::vector;

class Room
{
private:
    //NB:   As iterators are being used, if a room is added it must also be
    //      assigned some intelligble bounds otherwise, otherwise when we attempt
    //      too findPersonWard() we will get garbage!!!!!
    
    //syntax for accessing a person at an iterator
    //(*frontIt) is a Person*
    //*(*frontIt) is a Person
    
    vector<Person*>::iterator frontIt; //iterator to first person in room
    vector<Person*>::iterator backIt; //iterator to last person in room
    Person* frontRef;
    Person* backRef;
    vector<Person*> * personList;
    
    int numPeople; //number of people in the room
    
    //specs
    int maxPeople;
    int maxWeight;
    
    bool TRACE;
    
public:
    Room(int a_maxPeople = 10, int a_maxWeight = 4000, int a_numPeople = 0, vector<Person*> * personList = NULL, bool a_TRACE = false);
    Room(vector<Person*>::iterator a_frontIt, vector<Person*>::iterator a_backIt, vector<Person*> * personList, int a_maxPeople = 10, int a_maxWeight = 4000, int a_numPeople = 0, bool a_TRACE = false);
    ~Room();
    
    void setFrontIt(vector<Person*>::iterator personIt);
    void setBackIt(vector<Person*>::iterator personIt);
    ERROR_CODE setBounds(vector<Person*>::iterator front,vector<Person*>::iterator back, int amount);
    ERROR_CODE addPerson(vector<Person*>::iterator personIt );
    ERROR_CODE removePerson(vector<Person*>::iterator personIt);
    ERROR_CODE updateBoundIterators();
    
    vector<Person*>::iterator getPersonIterator(Person* person);
    vector<Person*>::iterator getFrontIt();
    vector<Person*>::iterator getBackIt();
    
    int getNumPeople();
    bool full();
    string print(string spacer = "");
};

#endif
