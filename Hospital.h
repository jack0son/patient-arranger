#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "Person.h"
#include "Ward.h"
#include "ErrorCode.h"
#include "PersonListSearch.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>



using ErrorCode::ERROR_CODE;

struct Location
{
	int personIndex;
	int roomIndex;
	int wardIndex;
};

class Hospital
{
private:
    //----variables
    string name;
    std::vector<Person* > personList;
    std::vector<Ward*> wardList;
    int maxRoomsPerWard;
    int maxPeoplePerRoom;
    int maxWeightPerRoom;
    
    bool TRACE; //if true then traces for debugging will print
    
    //----methods
    void addWard();
    ERROR_CODE removeWard(vector<Ward*>::iterator wardIt);
    ERROR_CODE insertPerson(Person* person);
    
public:
    //----ctr/dtr
    Hospital(std::string a_name, int a_maxRoomsPerWard = 6, int a_maxPeoplePerRoom = 10, int a_maxWeightPerRoom = 4000, bool a_TRACE = false);
    ~Hospital();
    
    //----methods
    std::string getName();
    //person methods
    
    ERROR_CODE addPerson(Person* person);
    ERROR_CODE assignPersonToWard(vector<Person*>::iterator personIt);
    Ward* findPersonWard(vector<Person*>::iterator personIt);
    vector<Ward*>::iterator findPersonWardIterator(vector<Person*>::iterator personIt);
    ERROR_CODE reassignWards();
    vector<Person*>::iterator getPersonIterator(Person* person);
    
    ERROR_CODE updateWardBoundIterators();
    
    ERROR_CODE removePerson(Person person);
    vector<Person*>::iterator findPerson(Person person);
    vector<Person*>::iterator basicFindPerson(Person person);
    
    ERROR_CODE checkWardListValidity();
    ERROR_CODE checkRoomListValidity();
  
    int getCapacity();
    int getNumBedsAvailable();
    ERROR_CODE findPersonLocation(Person person, Location &location);
    
    string printPersonList();
    string printWardList();
    string printWardStats();
    
    void testHospitalValidity();
    string test(bool t);
    void setTrace(bool t);
    
    string print(string spacer = "\t");

};

#endif

