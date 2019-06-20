#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <vector>
#include <string>
#include "ErrorCode.h"
#include "Hospital.h"
#include "Person.h"

using ErrorCode::ERROR_CODE;

class Community
{
private:
	vector<Hospital*> hospitalList;
	
public:
	Community();
	~Community();
	
	ERROR_CODE addHospital(Hospital* hospital);
	ERROR_CODE addPersonToHospital(Person* person, Hospital hospital);
	ERROR_CODE removePersonFromHospital(Person* person, Hospital* hospital);
	ERROR_CODE findPerson(Person person, Hospital hospital);
	
	vector<Hospital*>::iterator findHospital(Hospital hospital);
	Person* findPersonInHospital(Person person, vector<Hospital*>::iterator hospitalIt);
	ERROR_CODE checkHospitalIt(vector<Hospital*>::iterator hospitalIt);
	
	//ERROR_CODE movePerson(vector<Person*>::iterator personIt, vector<Hospital*>::iterator sourceHospitalIt, vector<Hospital*>::iterator destHospitalIt);
	ERROR_CODE movePerson(Person* person, Hospital* sourceHospital, Hospital* destHospital);
	
	std::string print(std::string spacer = "\t");
};

#endif
