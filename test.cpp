#include <iostream>
#include <string>
#include "Person.h"
#include "Hospital.h"
#include "Room.h"
#include "ErrorCode.h"
#include "Community.h"
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace ErrorCode;

//struct
struct PersonInstruction
{
	Person* person;
	string hospitalName;
	string instruction;
};

//data manip
vector<Person*> * getPersonData(const char* filename);
vector<PersonInstruction> getPersonInstructionData(const char* filename);

//hospital manip
ERROR_CODE addPeopleToHospital(vector<Person*> * people, Hospital &hospital, bool trace = false);
ERROR_CODE removePeopleFromHospital(vector<Person*> * people, Hospital &hospital, bool trace = false);
ERROR_CODE manipulateCommunity(vector<PersonInstruction> piList, Community& community, bool trace = false);

//string manip
void removeCharacter(string& str, char ch = '.');
void stringToUpper(string& str);

//testing functions
void demonstrateAdding();
void demonstrateRemoving();
void stressTestAddRemove();
void demonstrateCommunity();
//

int main()
{
	int choice = 0;
	while(choice < 1 || choice > 4)
	{
		cout << "Test choices:\n"
			<< "\t1. Demonstrate adding\n"
			<< "\t2. Demonstrate removing\n"
			<< "\t3. Stress test\n"
			<< "\t4. Demonstrate community\n"
			<< "Choice: ";
		cin >> choice;
	}
	
	switch(choice)
	{
		case 1:
			demonstrateAdding();
			break;
		case 2:
			demonstrateRemoving();
			break;
		case 3:
			stressTestAddRemove();
			break;
		case 4:
			demonstrateCommunity();
			break;
	}
    
    return 0;
}

void demonstrateAdding()
{
	cout << "Demonstrating adding on small list:\n\n";
	Hospital hospital("NAME", 2,10);
	cout << "Error: " << interpretError(addPeopleToHospital(getPersonData("smallData.txt"), hospital, true)) << endl;
	
	hospital.testHospitalValidity();
}

void demonstrateRemoving()
{
	cout << "Demonstrating removing on small list:\n\n";
	Hospital hospital("NAME", 2,10);
	cout << "Error: " << interpretError(addPeopleToHospital(getPersonData("smallData.txt"), hospital)) << endl;
	hospital.testHospitalValidity();
	cout << "Error: " << interpretError(removePeopleFromHospital(getPersonData("smallData.txt"), hospital, true)) << endl;
	
	hospital.testHospitalValidity();
}

void stressTestAddRemove()
{
	cout << "Demonstrating adding then removing on large list:\n\n";
	Hospital hospital("NAME", 2,10);
	cout << "Error: " << interpretError(addPeopleToHospital(getPersonData("bigData.txt"), hospital)) << endl; 
	hospital.testHospitalValidity();
	cout << "Error: " << interpretError(removePeopleFromHospital(getPersonData("bigData.txt"), hospital)) << endl;
	
	hospital.testHospitalValidity();
}

void demonstrateCommunity()
{
	vector<PersonInstruction> piList = getPersonInstructionData("communityData.txt");
	Community community;
	Hospital* rah = new Hospital("RAH");
	Hospital* qeh = new Hospital("QEH");
	Hospital* modbury = new Hospital("MODBURY");
	Hospital* burnside = new Hospital("BUR");
	Hospital* calvary = new Hospital("CAL");
	Hospital* ashford = new Hospital("ASH");
	ERROR_CODE error;
	error = community.addHospital(rah);
	error = community.addHospital(qeh);
	error = community.addHospital(modbury);
	error = community.addHospital(burnside);
	error = community.addHospital(calvary);
	error = community.addHospital(ashford);
	
	cout << "Adding hospitals error: " << interpretError(error) << endl;
	
	cout << "Error: " << interpretError(manipulateCommunity(piList, community, true)) << endl;

	cout << community.print();
	
	
	delete rah;
	delete qeh;
	delete modbury;
}

ERROR_CODE addPeopleToHospital(vector<Person*> * people, Hospital &hospital, bool trace)
{
    ERROR_CODE error;
    cout << "Adding people to hospital.\n";
    for(int i=0; i<people->size(); i++)
    {
        error = hospital.addPerson((*people)[i]);
        if(error)
        {
            return error;
        }
		if(trace) cout << hospital.print() << endl;
    }
    return SUCCESS;
}

ERROR_CODE removePeopleFromHospital(vector<Person*> * people, Hospital &hospital, bool trace)
{
	cout << "Removing people from hospital.\n";
    ERROR_CODE error;
    float temp;
    for(int i=0; i<people->size(); i++)
    {
        if(trace) cout << "Removing " << (*people)[i]->print() << endl;
        //temp = ((i+1)/(people->size()))*100;
        //cout << temp << "%\n";
        error = hospital.removePerson(*(*people)[i]);
        if(error)
        {
            return error;
        }
        if(trace) cout << hospital.print();
    }
    return SUCCESS;
}

ERROR_CODE manipulateCommunity(vector<PersonInstruction> piList, Community& community,bool trace)
{
	cout << "Manipulating community...\n";
	ERROR_CODE error;
	for(int i=0; i<piList.size(); i++)
	{
		//if(piList[i].instuction == "A")
		{
			if(trace) cout << "Adding " << piList[i].person->getLastName() << " to "
				<< piList[i].hospitalName << endl;
			error = community.addPersonToHospital(piList[i].person, Hospital(piList[i].hospitalName));
			if(error) return error;
		}
	}
	
	return SUCCESS;
}

vector<PersonInstruction> getPersonInstructionData(const char* filename)
{
	ifstream fin(filename);
	vector<PersonInstruction> piList;
	if(!fin.is_open())
	{
		cout << "We have a problem with " << filename << ".\n";
		return piList;
	}
	
	char ch[30];
	char DELIM = ',';
	int count = 0;
	int total = 0;
	PersonInstruction pi;
	string lastName;
	string initials;
	string hospitalName;
	string instruction;
	int birthYear, weight;
	Person* temp;
	fin.seekg(0);
	while(!fin.eof())
	{
		while(count<6)
		{
			if(count < 5)
				fin.get(ch,30,DELIM); //read up to comma
			else
				fin.get(ch,30);	//read up to newline
			
			if(!fin.good())
			{
				fin.clear();
			}
			
			fin.get(); 
			
			switch(count)
			{
				case 0:
				{
					lastName = ch;
					break;
				}
				case 1:
				{
					initials = ch;
					break;
				}
				case 2:
				{
					birthYear = atoi(ch);
					break;
				}
				case 3:
				{
					weight = atoi(ch);
					break;
				}
				case 4:
				{
					instruction = ch;
					break;
				}
				case 5:
				{
					hospitalName = ch;
					break;
				}
			}
			count++;
		}
		count = 0;	
			
		removeCharacter(initials);//remove dots from intials
		removeCharacter(lastName, ' ');//remove space from last name
        stringToUpper(initials);
        stringToUpper(lastName);
        stringToUpper(hospitalName);
		temp = new Person(lastName,initials,birthYear,weight);
		//if(lastName.length() < 14)
		pi.person = temp;
		pi.hospitalName = hospitalName;
		pi.instruction = instruction;
		
		piList.push_back(pi);
		
		cout << "Stored " << total << ": " << temp->print() << endl;
		
		total++;
	}
	
	return piList;
}

vector<Person*> * getPersonData(const char* filename)
{
	ifstream fin(filename);
	if(!fin.is_open())
	{
		cout << "We have a problem with " << filename << ".\n";
		return NULL;
	}
	vector<Person*> * people = new vector<Person*>;
	char ch[30];
	char DELIM = ',';
	int count = 0;

	int total = 0;
	string lastName;
	string initials;
	int birthYear, weight;
	Person* temp;
	fin.seekg(0);
	while(!fin.eof())
	{
		while(count<4)
		{
			if(count < 3)
				fin.get(ch,30,DELIM); //read up to comma
			else
				fin.get(ch,30);	//read up to newline
			
			if(!fin.good())
			{
				fin.clear();
			}
			
			fin.get(); 
			
			switch(count)
			{
				case 0:
				{
					lastName = ch;
					break;
				}
				case 1:
				{
					initials = ch;
					break;
				}
				case 2:
				{
					birthYear = atoi(ch);
					break;
				}
				case 3:
				{
					weight = atoi(ch);
					break;
				}
			}
			count++;
		}
		count = 0;	
			
		removeCharacter(initials);//remove dots from intials
		removeCharacter(lastName, ' ');//remove space from last name
        stringToUpper(initials);
        stringToUpper(lastName);
		temp = new Person(lastName,initials,birthYear,weight);
		//if(lastName.length() < 14)
        people->push_back(temp);
		
		cout << "Stored " << total << ": " << temp->print() << endl;
		
		total++;
	}
	
	return people;
}

void removeCharacter(string& str , char ch)
{
	for(int i=0; i<str.length(); i++)
	{
		if(str[i] == ch)
			str.erase(str.begin()+i);
	}
}

void stringToUpper(string& str)
{
    for(int i=0; i<str.length(); i++)
    {
        str[i] = toupper(str[i]);
    }
}
