#include "Person.h"

using namespace std;

Person::Person(string a_lastName, string a_initials, int a_birthYear, int a_weight)
    : lastName(a_lastName), initials(a_initials), birthYear(a_birthYear), weight(a_weight)
{
    //ctor
}

Person::~Person()
{
    //dtor
}

string Person::getLastName()
{
    return lastName;
}

string Person::getInitials()
{
    return initials;
}

int Person::getBirthYear()
{
    return birthYear;
}

int Person::getWeight()
{
    return weight;
}

bool Person::operator==(Person person)
{
    if(lastName == person.lastName  //check all members are equal
       && initials == person.initials
       && birthYear == person.birthYear
       && weight == person.weight)
    {
        return true;
    }
    
    return false;
}

bool Person::operator<(Person person)
{
    bool result = false;
    if(lastName < person.lastName)
    {
        result = true;
    }
    else if(lastName == person.lastName)
    {
        if(initials < person.initials)
        {
            result = true;
        }
        else if(initials == person.initials)
        {
            if(birthYear < person.birthYear)
            {
                result = true;
            }
            else if(birthYear == person.birthYear)
            {
                if(weight < person.weight)
                {
                    result = true;
                }
            }
        }
    }
    
    return result;
}

bool Person::operator>(Person person)
{
    bool result = false;
    if(lastName > person.lastName)
    {
        result = true;
    }
    else if(lastName == person.lastName)
    {
        if(initials > person.initials)
        {
            result = true;
        }
        else if(initials == person.initials)
        {
            if(birthYear > person.birthYear)
            {
                result = true;
            }
            else if(birthYear == person.birthYear)
            {
                if(weight > person.weight)
                {
                    result = true;
                }
            }
        }
    }
    
    return result;
}

string Person::print(string spacer)
{
	string s1;
	ostringstream sout;
	sout << birthYear << spacer << weight;
	s1 = sout.str();
	return lastName + spacer + initials + spacer + s1;
}
