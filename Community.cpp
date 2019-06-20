#include "Community.h"

using namespace ErrorCode;
using namespace std;
#include <iostream>

Community::Community()
{}

Community::~Community()
{}

ERROR_CODE Community::addHospital(Hospital* hospital)
{
	if(findHospital(*hospital) != hospitalList.end())
	{
		return HOSPITAL_ALREADY_EXISTS;
	}
	hospitalList.push_back(hospital);
		
	return SUCCESS;
}

ERROR_CODE Community::addPersonToHospital(Person* person, Hospital hospital)
{
	vector<Hospital*>::iterator hospitalIt = findHospital(hospital);
	if(hospitalList.empty())
	{
		return NO_HOSPITALS;
	}
	if(hospitalIt == hospitalList.end())
	{
		return HOSPITAL_NOT_FOUND;
	}
	
	return (*hospitalIt)->addPerson(person);
}

ERROR_CODE removePersonFromHospital(Person* person, Hospital* hospital)
{
	return hospital->removePerson(*person);
}

ERROR_CODE Community::findPerson(Person person, Hospital hospital)
{
	vector<Hospital*>::iterator hospitalIt = findHospital(hospital);
	ERROR_CODE error = checkHospitalIt(hospitalIt);
	if(error) return error;
	//person not found search other hospitals
	Person* temp = NULL;
	temp = findPersonInHospital(person, hospitalIt);
	if(temp && *temp == person)
	{
		cout << person.print() << " is found in " << hospital.getName() << endl;
		return SUCCESS;
	}

	for(int i=0; i<hospitalList.size(); i++)
	{	
		temp = findPersonInHospital(person, hospitalList.begin()+i);
		if(temp)
		{
			return movePerson(temp, *(hospitalList.begin()+i), *(hospitalIt)); 
		}
	}
	
	return FAILURE;
	
}

Person* Community::findPersonInHospital(Person person, vector<Hospital*>::iterator hospitalIt)
{
	//return (*hospitalIt)->findPerson(person);
	return NULL;
}

vector<Hospital*>::iterator Community::findHospital(Hospital hospital)
{
	for(int i=0; i<hospitalList.size(); i++)
	{
		//cout << "Hospital in list" << i << ": " << hospitalList[i]->getName() << endl;
		//cout << "--Looking for: " << hospital.getName() << endl;
		if(hospitalList[i]->getName() == hospital.getName())
		{
			return hospitalList.begin() + i;
		}
	}
	
	return hospitalList.end();
}

ERROR_CODE Community::movePerson(Person* person, Hospital* sourceHospital, Hospital* destHospital)
{
	Person* tempPerson = new Person(*person);
	ERROR_CODE error = sourceHospital->removePerson(*person);
	if(error) return error;
	
	error = destHospital->addPerson(tempPerson);
	if(error) return error;
	
	return SUCCESS;
}

string Community::print(string spacer)
{
	string result = "";
	for(int i=0; i<hospitalList.size(); i++)
	{
		result += "Hospital: ";
		result += hospitalList[i]->getName() + "\n";
		result += hospitalList[i]->print(spacer) + "\n";
	}
	return result;
}

ERROR_CODE Community::checkHospitalIt(vector<Hospital*>::iterator hospitalIt)
{
	if(hospitalList.empty())
	{
		return NO_HOSPITALS;
	}
	if(hospitalIt == hospitalList.end())
	{
		return HOSPITAL_NOT_FOUND;
	}
	
	return SUCCESS;
}


