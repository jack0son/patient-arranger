#ifndef WARD_H
#define WARD_H

#include "Person.h"
#include "Room.h"
#include "ErrorCode.h"
#include "PersonListSearch.h"
#include <vector>
#include <sstream>
#include <iostream>

using ErrorCode::ERROR_CODE;

class Ward
{
private:
    vector<Person*>::iterator frontIt; //iterator to first person in room
    vector<Person*>::iterator backIt; //iterator to last person in room
    Person* frontRef;   //pointer to first person in room
    Person* backRef;    //pointer to last person in room
    int numPeople; //number of people in the room
    
    std::vector<Room* > roomList;
    vector<Person*> * personList; //pointer to a person* list
    
    //Specifications
    int maxRooms;
    int maxPeoplePerRoom;
    int maxWeightPerRoom;
    
    bool TRACE;
    
    //methods
    //no one should use this except the ward
    ERROR_CODE insertPerson(vector<Person*>::iterator personIt);
    
public:
    Ward(int a_maxRooms = 6, int a_maxPeoplePerRoom = 10, int a_maxWeightPerRoom = 4000, int a_numPeople = 0, vector<Person*> * a_personList = NULL, bool a_TRACE = false);
    Ward(vector<Person*>::iterator a_frontIt, vector<Person*>::iterator a_backIt, vector<Person*> * a_personList, int a_maxRooms = 6, int a_maxPeoplePerRoom = 10, int a_maxWeightPerRoom = 4000, int a_numPeople = 0, bool a_TRACE = false);
    ~Ward();

    void setFrontIt(vector<Person*>::iterator personIt);
    void setBackIt(vector<Person*>::iterator personIt);
    ERROR_CODE setBounds(vector<Person*>::iterator front, vector<Person*>::iterator, int amount);
    
    //ward adds people to rooms in similar way to how hospital does
    //to wards
    void addRoom();
    ERROR_CODE removeRoom(vector<Room*>::iterator roomIt);
    ERROR_CODE addPerson(vector<Person*>::iterator personIt);
    ERROR_CODE assignPersonToRoom(vector<Person*>::iterator personIt);
    Room* findPersonRoom(vector<Person*>::iterator personIt);
    vector<Room*>::iterator findPersonRoomIterator(vector<Person*>::iterator personIt);
    ERROR_CODE reassignRooms();
    ERROR_CODE removePerson(vector<Person*>::iterator personIt);
    
    ERROR_CODE updateRoomBoundIterators();
    ERROR_CODE checkRoomListValidity();

    vector<Person*>::iterator getFrontIt();
    vector<Person*>::iterator getBackIt();
    ERROR_CODE updateBoundInterators();
    vector<Person*>::iterator getPersonIterator(Person* person);
    
    
    
    int getNumPeople();
    
    int getMaxPeople();
    int getMaxWeight();
    
    int getNumBedsAvailable();
    bool full();

    vector<Room*> * getRoomList();
    std::string print(std::string spacer = "");
};

#endif
