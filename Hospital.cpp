#include "Hospital.h"

using namespace std;
using namespace ErrorCode;

//constructor
Hospital::Hospital(string a_name,int a_maxRoomsPerWard, int a_maxPeoplePerRoom, int a_maxWeightPerRoom, bool a_TRACE) :
	name(a_name),
    maxRoomsPerWard(a_maxRoomsPerWard),
    maxPeoplePerRoom(a_maxPeoplePerRoom),
    maxWeightPerRoom(a_maxWeightPerRoom),
    TRACE(a_TRACE)
{}

//destructor
Hospital::~Hospital()
{
    //free dynamic memory
    if(personList.size())
    {
        for(int i=0; i<personList.size(); i++)
        {
            delete personList[i];
        }
    }
    if(wardList.size())
    {
        for(int i=0; i<wardList.size(); i++)
        {
            delete wardList[i];
        }
    }
}

string Hospital::getName()
{
	return name;
}

//find the appropriate position for a person in the master patient list
ERROR_CODE Hospital::insertPerson(Person* person)
{
    if(!personList.size()) //if no people just push back the first person
    {
        personList.push_back(person);
    }
    else
    {
        //use binary search to find the right spot for new PERSON
        int lower = 0;
        int upper = personList.size();
        int pivot;
        bool test = false;
        while(upper >= lower && !test)
        {
            pivot = (lower+upper)/2;
            if(*person > *personList[pivot])
            {
                if(pivot+1 < personList.size())
                {
                    if(*person < *personList[pivot+1])
                    {
                        test = true;
                        ++pivot;
                    }
                    else
                    {
                        lower = pivot+1;
                    }
                }
                else
                {
                    //person must go to back
                    personList.push_back(person);
                    return SUCCESS;
                }
                
            }
            else if(*person < *personList[pivot])
            {
                if(pivot-1 >= 0)
                {
                    if(*person > *personList[pivot-1])
                    {
                        test = true;
                    }
                    else
                    {
                        upper = pivot-1;
                    }
                }
                else
                {
                    test = true; //at front
                }
            }
            else
            {
                return PERSON_ALREADY_EXISTS;
            }
        }
        personList.insert(personList.begin()+pivot, person);
    }
    //TRACE
    if(TRACE) cout << "Inserted " << person->getLastName() << endl;
    return SUCCESS;
}

//add an empty ward
void Hospital::addWard()
{
    //personList.end() represents no value for the iterator
    Ward* tempWard = new Ward(personList.end(), personList.end(), &personList, maxRoomsPerWard, maxPeoplePerRoom, maxWeightPerRoom, TRACE);
    tempWard->addRoom();
    wardList.push_back(tempWard);
}

ERROR_CODE Hospital::removeWard(vector<Ward*>::iterator wardIt)
{
    wardList.erase(wardIt);
    return SUCCESS;
}

//add a person into the hospital in correct ward
ERROR_CODE Hospital::addPerson(Person* person)
{
    //first insert them into main list
    ERROR_CODE error = insertPerson(person);
    if(error)
    {
        return error;
    }
    
    //adjust boundary iterators for new person list
    error = updateWardBoundIterators();
    if(error)
    {
        return error;
    }
    
    //assign them to a ward
    error = assignPersonToWard(getPersonIterator(person));
    if(error)
    {
        return error;
    }
    //cout << printWardStats() << endl;
    
    //cout << printWardList() << endl;
    
    return error;
}

//fit a person to a ward. Using the following interpretation
//assign implies the ward must be 'found'
//add implies putting a person in a specific, known, ward
ERROR_CODE Hospital::assignPersonToWard(vector<Person*>::iterator personIt)
{
    if(TRACE) cout << "\tAssigning " << (*personIt)->getLastName() <<  " to a ward.\n";
    
    //find ward person would fit in
    Ward* appWard = findPersonWard(personIt); //appWard=appropriate ward
    
    if(getNumBedsAvailable() < 0)//if more patients than ward capacity
    {
        if(TRACE) cout << "\t\tAdding ward because of " << (*personIt)->getLastName() << endl;
        addWard(); 
        return reassignWards(); // reshuffle patient into correct wards
    }
    
    //if there is an appropriate ward, and there is is room in it
    else if(appWard != NULL && appWard->addPerson(personIt) == SUCCESS)
    {
        //patient gets added to ward by above ward.addPerson(...)
        if(TRACE) cout << "\t\tAdded " << (*personIt)->getLastName() << " to ward " << (*(appWard->getFrontIt()))->getLastName() << endl; //TRACE
        
        //return checkWardListValidity(); //check for errors
        return SUCCESS;
    }
    
    //if there is no appropriate ward but there
    //is room in the hospital -> just reshuffle the wards
    else
    {
        return reassignWards(); // return error
    }
    
    return UNKNOWN_ERROR; // this should never execute
}

//binary search for ward
Ward* Hospital::findPersonWard(vector<Person*>::iterator personIt)
{
    if(TRACE) cout << "\t\tFinding ward for " << (*personIt)->getLastName() << endl;
    if(!wardList.size())
    {
        return NULL; //there are no wards
    }
    
    //current segment of list to check
    int lower = 0;
    int upper = wardList.size();
    
    int pivot;  //ward currently examining
    Ward* result = NULL;    //the correct ward
    bool test = false;  //have we found the result
    while(upper >= lower && !test)
    {
        pivot = (upper+lower)/2;
        //if person fits before pivot ward
        if(*(*personIt) < *(*(wardList[pivot]->getFrontIt()))) //compare person objects
        {
            if(!wardList[pivot]->full())//if there is space in the pivot ward
            {
                if(pivot > 0) //if not at first ward
                {
                    //if person is between the back of previous ward
                    //and the front of the pivot ward
                    if(*(*personIt) > *(*(wardList[pivot-1]->getBackIt())))
                    {
                        result = wardList[pivot]; //person fits at front of pivot ward;
                        test = true;
                    }
                    else //person does not fit in this ward
                    {
                        upper = pivot-1;
                    }
                }
                else //pivot ward is first ward and it has room
                {
                    //person fits before first ward
                    //and this ward has space
                    result = wardList[pivot];
                    test = true;
                }
            }
            else //ward is full and patient fits before it
            {
                if(pivot <= 0) // at first ward
                {
                    //result = wardList[pivot]; //patient fits in no ward
                    result = NULL;
                    test = true;
                }
                else
                {
                    upper = pivot-1; //person cannot fit in this ward
                }
            }
        }
        
        //if person fits after pivot ward
        else if(*(*personIt) > *(*(wardList[pivot]->getBackIt())))
        {
            if(!wardList[pivot]->full())//if space in pivot ward
            {
                if(pivot < wardList.size()-1) //if not at last ward
                {
                    if(*(*personIt) < *(*(wardList[pivot+1]->getFrontIt())))
                    {
                        result = wardList[pivot];
                        test = true;
                    }
                    else
                    {
                        lower = pivot+1;
                    }
                }
                else
                {
                    result = wardList[pivot];
                    test = true;
                }
            }
            else
            {
                if(pivot >= (wardList.size()-1))
                {
                    //result = wardList[pivot];
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
            result = wardList[pivot];
            test = true;
        }
    }
    
    return result;
}

vector<Ward*>::iterator Hospital::findPersonWardIterator(vector<Person*>::iterator personIt)
{
    if(TRACE) cout << "\t\tFinding ward for " << (*personIt)->getLastName() << endl;
    if(!wardList.size())
    {
        return wardList.end(); //there are no wards
    }
    
    //current segment of list to check
    int lower = 0;
    int upper = wardList.size();
    
    int pivot;  //ward currently examining
    vector<Ward*>::iterator result;    //the correct ward
    bool test = false;  //have we found the result
    while(upper >= lower && !test)
    {
        pivot = (upper+lower)/2;
        //if person fits before pivot ward
        if(*(*personIt) < *(*(wardList[pivot]->getFrontIt()))) //compare person objects
        {
            if(!wardList[pivot]->full())//if there is space in the pivot ward
            {
                if(pivot > 0) //if not at first ward
                {
                    //if person is between the back of previous ward
                    //and the front of the pivot ward
                    if(*(*personIt) > *(*(wardList[pivot-1]->getBackIt())))
                    {
                        result = wardList.begin()+pivot; //person fits at front of pivot ward;
                        test = true;
                    }
                    else //person does not fit in this ward
                    {
                        upper = pivot-1;
                    }
                }
                else //pivot ward is first ward and it has room
                {
                    //person fits before first ward
                    //and this ward has space
                    result = wardList.begin()+pivot;
                    test = true;
                }
            }
            else //ward is full and patient fits before it
            {
                if(pivot <= 0) // at first ward
                {
                    //result = wardList[pivot]; //patient fits in no ward
                    result = wardList.end();
                    test = true;
                }
                else
                {
                    upper = pivot-1; //person cannot fit in this ward
                }
            }
        }
        
        //if person fits after pivot ward
        else if(*(*personIt) > *(*(wardList[pivot]->getBackIt())))
        {
            if(!wardList[pivot]->full())//if space in pivot ward
            {
                if(pivot < wardList.size()-1) //if not at last ward
                {
                    if(*(*personIt) < *(*(wardList[pivot+1]->getFrontIt())))
                    {
                        result = wardList.begin()+pivot;
                        test = true;
                    }
                    else
                    {
                        lower = pivot+1;
                    }
                }
                else
                {
                    result = wardList.begin()+pivot;
                    test = true;
                }
            }
            else
            {
                if(pivot >= (wardList.size()-1))
                {
                    //result = wardList[pivot];
                    result = wardList.end();
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
            result = wardList.begin()+pivot;
            test = true;
        }
    }
    
    return result;
}

ERROR_CODE Hospital::reassignWards()
{
    if(TRACE) cout << "\t\t\tReassigning wards\n"; //TRACE
    
    if(!wardList.size())
    {
        return NO_WARDS;
        cin.get();
    }
    
    //int capacityDesired = capacity + wardList.size()*pItferedSpace;
    int peoplePerWard = (personList.size()/wardList.size());
    if(personList.size()%wardList.size())
    {
        peoplePerWard++; //account for truncation of peopleperward (ie round up)
    }
    
    //add ward if even spread is not possible
    if(personList.size() - peoplePerWard*(wardList.size()-1) > wardList[0]->getMaxPeople())
    {
        //this should never happen
        //if we dont round up and just do this instead, sorting is less efficient -> more wards
        //added than needed, but we shuffle the patients less
        //add more wards vs do more reassignments
        addWard();
    }
    
    vector<Person*>::iterator front = personList.end();
    vector<Person*>::iterator back = personList.end();
    int peopleInWard;
    int count = 0;
    vector<Person*>::iterator temp;
    
    for(int i=0; i<wardList.size(); i++)
    {
        front = personList.begin() + (i*peoplePerWard);
        
        //if not at end of ward list
        //if(((i*peoplePerWard)+(peoplePerWard-1)) < personList.size())
        if(i < wardList.size()-1)
        {
            back = personList.begin() + (i*peoplePerWard)+(peoplePerWard-1);
            peopleInWard = peoplePerWard;
        }
        
        else //put the rest of the patients into the last ward
        {
            back = personList.end()-1;
            peopleInWard = personList.size() - (i*peoplePerWard);
        }
        
        //trace
        /*if(TRACE)
        {
            cout << "ward:" << i << " " << "People in ward " << peopleInWard << endl;
            cout << "Front: " << (*front)->getLastName() << endl;
            cout << "Back: " << (*back) ->getLastName() << "\n\n";
        }*/
        
        //check for errors
        //ERROR_CODE error;// = wardLisst[i]->setBounds(front, back, peopleInWard);
        ERROR_CODE error = wardList[i]->setBounds(front,back,peopleInWard);
        if(error)
        {
            return error;
        }
        error = wardList[i]->reassignRooms();
        
        //error = wardList[i]->reassignRooms();
        if(error)
        {
           return error;
        }
    }
    return SUCCESS;
}

//get the iterator of a person in the personlist
vector<Person*>::iterator Hospital::getPersonIterator(Person* person)
{
    return PersonListSearch::basic_search(person, &personList);
}

ERROR_CODE Hospital::removePerson(Person person)
{
    vector<Person*>::iterator personIt = findPerson(person);
    if(personIt == personList.end())
    {
        return PERSON_NOT_EXIST;
    }
    vector<Ward*>::iterator wardIt = findPersonWardIterator(personIt);
    if(wardIt == wardList.end())
    {
        return WARD_NOT_FOUND;
    }
    ERROR_CODE error = (*wardIt)->removePerson(personIt);
    if(error) return error;
    
    if((*wardIt)->getNumPeople() <= 0)
    {
        removeWard(wardIt);
    }
    
    delete *personIt;
    personList.erase(personIt);
    updateWardBoundIterators();
    
    //check if ward can be removed?
    
    return SUCCESS;
}

//binary search for person
vector<Person*>::iterator Hospital::findPerson(Person person)
{
    int lower = 0;
    int upper = personList.size();
    int pivot;
    
    while(upper >= lower)
    {
        pivot = (upper+lower)/2;
        if(*(personList[pivot]) < person)
        {
            lower = pivot+1;
        }
        else if(*(personList[pivot]) > person)
        {
            upper = pivot-1;
        }
        else
        {
            return personList.begin()+pivot;
        }
    }
    
    return personList.end();
}

ERROR_CODE Hospital::updateWardBoundIterators()
{
    ERROR_CODE error;
    for(int i=0; i<wardList.size(); i++)
    {
        error = wardList[i]->updateBoundInterators();
        if(error)
        {
            return error;
        }
    }
    return SUCCESS;
}

ERROR_CODE Hospital::checkWardListValidity()
{
    vector<Person*>::iterator prevFront = personList.end();
    vector<Person*>::iterator prevBack = personList.end();
    for(int i=0; i<wardList.size(); i++)
    {
        //check for invalid bounds
        if(wardList[i]->getFrontIt() == personList.end() || wardList[i]->getBackIt() == personList.end())
        {
            return INVALID_BOUND;
        }
        //check for overlaps
        if(prevFront != personList.end() && prevBack != personList.end())
        {
            if(*(*(wardList[i]->getFrontIt())) < *(*prevBack) || *(*prevFront) > *(*(wardList[i]->getBackIt())))
            {
                return WARD_OVERLAP;
            }
        }
        
        //check for capacity valid
        if(wardList[i]->getNumPeople() > wardList[i]->getMaxPeople())
        {
            return TOO_MANY_PEOPLE_FOR_WARD;
        }
        prevFront = wardList[i]->getFrontIt();
        prevBack = wardList[i]->getBackIt();
    }
    return SUCCESS;
}

ERROR_CODE Hospital::checkRoomListValidity()
{
    ERROR_CODE error;
    for(int i=0; i<wardList.size(); i++)
    {
        error = wardList[i]->checkRoomListValidity();
        if(error)
        {
            return error;
        }
    }
    return SUCCESS;
}


int Hospital::getCapacity()
{
    return wardList.size()*maxPeoplePerRoom*maxRoomsPerWard;
}

//number spaces available in hospital
int Hospital::getNumBedsAvailable()
{
    //number of total beds minus number of patients
    int temp = getCapacity() - personList.size();
    if(temp < 0)
    {
        //temp = 0;
    }
    //will always return number >= 0?
    return temp;
}

ERROR_CODE Hospital::findPersonLocation(Person person, Location &location)
{
	Location result;
	vector<Person*>::iterator personIt = findPerson(person);
	if(personIt == personList.end())
	{
		return PERSON_NOT_EXIST;
	}
	vector<Ward*>::iterator wardIt = findPersonWardIterator(personIt);
	if(wardIt == wardList.end())
	{
		return FAILURE;
	}
	vector<Room*>::iterator roomIt = (*wardIt)->findPersonRoomIterator(personIt);
	
	result.personIndex = personIt - personList.begin();
	result.roomIndex = roomIt - (*wardIt)->getRoomList()->begin();
	result.wardIndex = wardIt - wardList.begin();
    
    return SUCCESS;
}

string Hospital::printPersonList()
{
    string result;
    for(int i=0; i<personList.size(); i++)
    {
        result += personList[i]->print() + "\n";
    }
    return result;
}

string Hospital::printWardList()
{
    ostringstream result;
    int wardCount = 0;
    int roomCount = 0;
    for(int i=0; i<personList.size(); i++)
    {
        if(personList.begin()+i == wardList[wardCount]->getFrontIt())
        {
            result << "Ward " << wardCount << ":\n";
        }
        result << "\t\tPerson " << i << ": " << personList[i]->print() << endl;
        if(personList.begin()+i == wardList[wardCount]->getBackIt())
        {
            ++wardCount;
        }
    }
    
    return result.str();
}

string Hospital::printWardStats()
{
    string result = "";
    
    for(int i=0; i<wardList.size(); i++)
    {
        if(wardList[i]->getFrontIt() == personList.end())
        {
            cout << "front bound is wrong\n";
            break;
        }
        if(wardList[i]->getBackIt() == personList.end())
        {
            cout << "back bound is wrong\n";
            break;
        }
        result += "Front: " + (*(wardList[i]->getFrontIt()))->print() + "\n";
        result += "Back: " + (*(wardList[i]->getBackIt()))->print() + "\n";
    }
    return result;
}

void Hospital::testHospitalValidity()
{
    cout << "Testing hospital validity...\n";
    cout << "Enough room for patients:\t" << test(getNumBedsAvailable() >= 0) << endl;
    cout << "Wards are valid:\t\t" << test(!checkWardListValidity()) << endl;
    cout << "Rooms are valid:\t\t" << interpretError(checkRoomListValidity()) << endl;
}

string Hospital::test(bool t)
{
    if(t)
        return "PASS.";
    else
        return "TEST FAILED.";
}

void Hospital::setTrace(bool t)
{
    TRACE = t;
}

string Hospital::print(string spacer)
{
    ostringstream sout;
    if(wardList.empty())
    {
        sout << "Hospital is empty.\n";
    }
    for(int i=0; i<wardList.size(); i++)
    {
        sout << "Ward " << i << ":\n";
        sout << wardList[i]->print(spacer);
    }
    return sout.str();
}
