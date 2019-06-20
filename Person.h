#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <sstream>

using std::string;

class Person
{
public:
    Person(string a_lastName = "DEFAULT", string a_initials = "DF", int a_birthYear = 0, int a_weight = 0);
    ~Person();

    string getLastName();
    string getInitials();
    int getBirthYear();
    int getWeight();
    std::string print(string spacer = ", ");

    bool operator==(Person person);
    bool operator<(Person person);
    bool operator>(Person person);


protected:
    string lastName;
    string initials;
    int birthYear;
    int weight;
};

#endif // PERSON_H
