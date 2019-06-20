 #include "Ward.h"

using namespace ErrorCode;
using namespace std;

//default ctr
Ward::Ward(int a_maxRooms, int a_maxPeoplePerRoom, int a_maxWeightPerRoom, int a_numPeople, vector<Person*> * a_personList, bool a_TRACE)
:   maxRooms(a_maxRooms),
    maxPeoplePerRoom(a_maxPeoplePerRoom),
    maxWeightPerRoom(a_maxWeightPerRoom),
    numPeople(a_numPeople),
    personList(a_personList),
    TRACE(a_TRACE)
{}

//ctr with iterator assignment
Ward::Ward(vector<Person*>::iterator a_frontIt, vector<Person*>::iterator a_backIt, vector<Person*> * a_personList, int a_maxRooms, int a_maxPeoplePerRoom, int a_maxWeightPerRoom, int a_numPeople, bool a_TRACE)
:   maxRooms(a_maxRooms),
    maxPeoplePerRoom(a_maxPeoplePerRoom),
    maxWeightPerRoom(a_maxWeightPerRoom),
    numPeople(a_numPeople),
    frontIt(a_frontIt),
    backIt(a_backIt),
    personList(a_personList),
    TRACE(a_TRACE)
{}

//destructor
Ward::~Ward()
{
    //free dynamic memory
    if(roomList.size())
    {
        for(int i=0; i<roomList.size(); i++)
        {
            delete roomList[i];
        }
    }
}

void Ward::setFrontIt(vector<Person*>::iterator personIt)
{
    frontIt = personIt;
    frontRef = *personIt;
}

void Ward::setBackIt(vector<Person*>::iterator personIt)
{
    backIt = personIt;
    backRef = *personIt;
}

ERROR_CODE Ward::setBounds(vector<Person*>::iterator front, vector<Person*>::iterator back, int amount)
{
    if(amount > getMaxPeople())
    {
        return TOO_MANY_PEOPLE_FOR_WARD;
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

void Ward::addRoom()
{
    Room* room = new Room(frontIt, frontIt, personList, maxPeoplePerRoom, maxWeightPerRoom, 0);
    roomList.push_back(room);
}

ERROR_CODE Ward::removeRoom(vector<Room*>::iterator roomIt)
{
    roomList.erase(roomIt);
    return SUCCESS;
}

//add a person to the ward, return error if full
ERROR_CODE Ward::insertPerson(vector<Person*>::iterator personIt)
{
    if(numPeople >= getMaxPeople())
    {
        return WARD_FULL;
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

//add person to the ward
ERROR_CODE Ward::addPerson(vector<Person*>::iterator personIt)
{
    ERROR_CODE error = insertPerson(personIt);
    if(error)
    {
        return error;
    }
    //find a room for the person
    updateRoomBoundIterators();
    error = assignPersonToRoom(personIt);
    if(error)
    {
        return error;
    }
    
    return SUCCESS;
}

ERROR_CODE Ward::assignPersonToRoom(vector<Person*>::iterator personIt)
{
    if(TRACE) cout << "\tAssigning to room " << (*personIt)->getLastName() << endl;
    Room* appRoom = findPersonRoom(personIt); //appRoom = appropriate room
    
    if(getNumBedsAvailable() < 0) //remembering that the person is already taking up a bed
    {                             //we just dont know where
        if(TRACE) cout << "\t\tAdding a room because of " << (*personIt)->getLastName() << endl;
        addRoom();
        return reassignRooms();
    }
    else if(appRoom && appRoom->addPerson(personIt) == SUCCESS)
    {
        if(TRACE) cout << "\t\tAdded " << (*personIt)->getLastName() << " to room " << (*(appRoom->getFrontIt()))->getLastName() << endl;
        return SUCCESS;
    }
    else
    {
        return reassignRooms();
    }
    
    return UNKNOWN_ERROR; //this should never execute
}

Room* Ward::findPersonRoom(vector<Person*>::iterator personIt)
{
    if(TRACE) cout << "\t\tFinding room for " << (*personIt)->getLastName() << endl;
    if(!roomList.size())
    {
        return NULL;
    }
    
    int lower = 0;
    int upper = roomList.size();
    int pivot;
    bool test = false;
    Room* result = NULL;
    
    while(upper >= lower && !test)
    {
        pivot = (upper+lower)/2;
        //if person fits before pivot room
        if(*(*personIt) < *(*(roomList[pivot]->getFrontIt())))
        {
            if(!roomList[pivot]->full())//if there is space in the pivot room
            {
                if(pivot > 0) //if not at first room
                {
                    //if person is between the back of previous room
                    //and the front of the pivot room
                    if(*(*personIt) > *(*(roomList[pivot-1]->getBackIt())))
                    {
                        result = roomList[pivot]; //person fits at front of pivot room;
                        test = true;
                    }
                    else //person does not fit in this room
                    {
                        upper = pivot-1;
                    }
                }
                else //pivot room is first room and it has room
                {
                    //person fits before first room
                    //and this room has space
                    result = roomList[pivot];
                    test = true;
                }
            }
            else //room is full and patient fits before it
            {
                if(pivot <= 0) // at first room
                {
                    //result = roomList[pivot]; //patient fits in no room
                    result = NULL;
                    test = true;
                }
                else
                {
                    upper = pivot-1; //person cannot fit in this room, must go before
                }
            }
        }
        
        //if person fits after pivot room
        else if(*(*personIt) > *(*(roomList[pivot]->getBackIt())))
        { 
            if(!roomList[pivot]->full())//if space in pivot room
            {
                if(pivot < roomList.size()-1) //if not at last room
                {
                    if(*(*personIt) < *(*(roomList[pivot+1]->getFrontIt())))
                    {
                        result = roomList[pivot];
                        test = true;
                    }
                    else
                    {
                        lower = pivot+1;
                    }
                }
                else
                {
                    result = roomList[pivot];
                    test = true;
                }
            }
            else
            {
                if(pivot >= (roomList.size()-1))
                {
                    //result = roomList[pivot];
                    result = NULL;
                    test = true;
                }
                else
                {
                    lower = pivot+1;
                }
            }
        }
        
        //person is equal to either of the bounds or between them
        else
        {
            result = roomList[pivot];
            test = true;
        }
    }
    
    return result;
}

//why are there two versions of this function?
//cause im halfway through updating to use iterators
vector<Room*>::iterator Ward::findPersonRoomIterator(vector<Person*>::iterator personIt)
{
    if(TRACE) cout << "\t\tFinding room for " << (*personIt)->getLastName() << endl;
    if(!roomList.size())
    {
        return roomList.end();
    }
    
    int lower = 0;
    int upper = roomList.size();
    int pivot;
    bool test = false;
    vector<Room*>::iterator result;
    
    while(upper >= lower && !test)
    {
        pivot = (upper+lower)/2;
        //if person fits before pivot room
        if(*(*personIt) < *(*(roomList[pivot]->getFrontIt())))
        {
            if(!roomList[pivot]->full())//if there is space in the pivot room
            {
                if(pivot > 0) //if not at first room
                {
                    //if person is between the back of previous room
                    //and the front of the pivot room
                    if(*(*personIt) > *(*(roomList[pivot-1]->getBackIt())))
                    {
                        result = roomList.begin()+pivot; //person fits at front of pivot room;
                        test = true;
                    }
                    else //person does not fit in this room
                    {
                        upper = pivot-1;
                    }
                }
                else //pivot room is first room and it has room
                {
                    //person fits before first room
                    //and this room has space
                    result = roomList.begin()+pivot;
                    test = true;
                }
            }
            else //room is full and patient fits before it
            {
                if(pivot <= 0) // at first room
                {
                    //result = roomList[pivot]; //patient fits in no room
                    result = roomList.end();
                    test = true;
                }
                else
                {
                    upper = pivot-1; //person cannot fit in this room, must go before
                }
            }
        }
        
        //if person fits after pivot room
        else if(*(*personIt) > *(*(roomList[pivot]->getBackIt())))
        { 
            if(!roomList[pivot]->full())//if space in pivot room
            {
                if(pivot < roomList.size()-1) //if not at last room
                {
                    if(*(*personIt) < *(*(roomList[pivot+1]->getFrontIt())))
                    {
                        result = roomList.begin()+pivot;
                        test = true;
                    }
                    else
                    {
                        lower = pivot+1;
                    }
                }
                else
                {
                    result = roomList.begin()+pivot;
                    test = true;
                }
            }
            else
            {
                if(pivot >= (roomList.size()-1))
                {
                    //result = roomList[pivot];
                    result = roomList.end();
                    test = true;
                }
                else
                {
                    lower = pivot+1;
                }
            }
        }
        
        //person is equal to either of the bounds or between them
        else
        {
            result = roomList.begin()+pivot;
            test = true;
        }
    }
    
    return result;
}

ERROR_CODE Ward::reassignRooms()
{
    if(TRACE) cout << "\t\t\tReassigning rooms...\n";
    if(!roomList.size())
    {
        return NO_ROOMS;
    }
    
    //this will execute when a new ward is added
    //(it needs some rooms)
    while(getNumBedsAvailable() < 0)
    {
    	if(TRACE) cout << "\t\t\t\tAdding room during room reassign\n";
		addRoom();
	}
    
    //int capacityDesired = capacity + wardList.size()*pItferedSpace;
    int peoplePerRoom = (numPeople/roomList.size());
    if(numPeople%roomList.size())
    {
        peoplePerRoom++; //account for truncation of peopleperward (ie round up)
    }
    //add ward if even spread is not possible
    /*while(peoplePerRoom > maxPeoplePerRoom)
    {
        //this should never happen
        //if we dont round up and just do this instead sorting is less efficient'
        //add more room vs do more reassignments
        addRoom();
        peoplePerRoom = (numPeople/roomList.size());
        if(numPeople%roomList.size())
        {
            peoplePerRoom++; //account for truncation of peopleperward (ie round up)
        }
    }*/
    
    vector<Person*>::iterator front;
    vector<Person*>::iterator back;
    int peopleInRoom;
    int count = 0;
    //the problem is the front It
    for(int i=0; i<roomList.size(); i++)
    {
        front = frontIt+(i*peoplePerRoom);
        //if not at end of room list
        if(i < roomList.size()-1)
        {
            back = front + peoplePerRoom-1;
            peopleInRoom = peoplePerRoom;
        }
        else //put the rest of the patients into the last room
        {
            back = backIt;
            peopleInRoom = numPeople - (i*peoplePerRoom);
        }
        
        //trace
        /*if(TRACE)
        {
            cout << "ward:" << i << " " << "People in ward " << peopleInWard << endl;
            cout << "Front: " << front->getLastName() << endl;
            cout << "Back: " << back->getLastName() << "\n\n";
        }*/
        
        //check for errors
        ERROR_CODE error = roomList[i]->setBounds(front, back, peopleInRoom);
        if(error)
        {
            return error;
        }
        //cout <<  roomList[i]->print() << endl;
    }
    return SUCCESS;
}

ERROR_CODE Ward::removePerson(vector<Person*>::iterator personIt)
{

	if(personIt < frontIt || personIt > backIt)
    {
        return PERSON_NOT_EXIST;
    }
    if(numPeople <= 0)
    {
        return WARD_EMPTY;
    }
	
    vector<Room*>::iterator roomIt = findPersonRoomIterator(personIt);
    if(roomIt == roomList.end())
    {
        return ROOM_NOT_FOUND;
    }
    ERROR_CODE error = (*roomIt)->removePerson(personIt);
    if(error) return error;
    if((*roomIt)->getNumPeople() <= 0)
    {
        removeRoom(roomIt);
    }
    
    if(personIt == frontIt)
    {
        setFrontIt(frontIt+1);
    }
    if(personIt == backIt)
    {
        setBackIt(backIt-1);
    }
    numPeople--;
    
    return SUCCESS;
}

ERROR_CODE Ward::updateRoomBoundIterators()
{
	ERROR_CODE error;
	for(int i=0; i<roomList.size(); i++)
	{
		error = roomList[i]->updateBoundIterators();
		if(error)
			return error;
	}
	return SUCCESS;
}

vector<Person*>::iterator Ward::getFrontIt()
{
    return frontIt;
}

vector<Person*>::iterator Ward::getBackIt()
{
    return backIt;
}

ERROR_CODE Ward::updateBoundInterators()
{
    frontIt = getPersonIterator(frontRef);
    backIt = getPersonIterator(backRef);
    if(frontIt == (*personList).end() || backIt == (*personList).end())
    {
        return BOUND_IT_NOT_FOUND;
    }
    ERROR_CODE error = updateRoomBoundIterators();
    if(error)
    {
        return error;
    }
    return SUCCESS;
}

ERROR_CODE Ward::checkRoomListValidity()
{
    vector<Person*>::iterator prevFront = personList->end();
    vector<Person*>::iterator prevBack = personList->end();
    for(int i=0; i<roomList.size(); i++)
    {
        //check for invalid bounds
        if(roomList[i]->getFrontIt() == personList->end() || roomList[i]->getBackIt() == personList->end())
        {
            return INVALID_BOUND;
        }
        //check for overlaps
        if(prevFront != personList->end() && prevBack != personList->end())
        {
            if(*(*(roomList[i]->getFrontIt())) < *(*prevBack)) //|| *(*prevFront) > *(*(roomList[i]->getBackIt())))
            {
                /*cout << "Room " << i << endl;
                cout << "frontIt: " << (*(roomList[i]->getFrontIt()))->print() << endl;
                cout << "backIt: " << (*(roomList[i]->getBackIt()))->print() << endl;
                cout << "prevFront: " << (*prevFront)->print() << endl;
                cout << "prevBack: " << (*prevBack)->print() << endl;*/
                
                return ROOM_OVERLAP;
            }
        }
        
        //check for capacity valid
        if(roomList[i]->getNumPeople() > maxPeoplePerRoom)
        {
            return TOO_MANY_PEOPLE_FOR_ROOM;
        }
        prevFront = roomList[i]->getFrontIt();
        prevBack = roomList[i]->getBackIt();
    }
    return SUCCESS;
}

vector<Person*>::iterator Ward::getPersonIterator(Person* person)
{
    return PersonListSearch::basic_search(person, personList);
}

int Ward::getNumPeople()
{
    return numPeople;
}

int Ward::getMaxPeople()
{
    return maxRooms*maxPeoplePerRoom;
}

int Ward::getMaxWeight()
{
    return maxRooms*maxWeightPerRoom;
}


int Ward::getNumBedsAvailable()
{
    int temp = (roomList.size()*maxPeoplePerRoom)-numPeople;
    return  temp;
}

bool Ward::full()
{
    if(numPeople >= getMaxPeople())
    {
        return true;
    }
    return false;
}

vector<Room*> * Ward::getRoomList()
{
    return &roomList;
}

string Ward::print(string spacer)
{
    ostringstream sout;
    for(int i=0; i<roomList.size(); i++)
    {
        sout << spacer << "Room " << i << ":\n";
        sout << roomList[i]->print(spacer+spacer);
    }
    return sout.str();
}



