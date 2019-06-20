#include "ErrorCode.h"
#include <vector>
using std::string;
string ErrorCode::interpretError(ERROR_CODE error)
{
    std::vector<string> result;
    result.push_back("Success.");
    result.push_back("Failure.");
    result.push_back("There was an unknown error.");
    result.push_back("Tried to add duplicate person.");
    result.push_back("Tried to remove non-existant person.");
    result.push_back("Cannot add to full ward.");
    result.push_back("Cannot remove from empty ward.");
    result.push_back("There are no wards.");
    result.push_back("Too many people for ward.");
    result.push_back("There are wards overlapping.");
    result.push_back("Cannot add to full room.");
    result.push_back("Cannot remove from empty room.");
    result.push_back("There are no rooms.");
    result.push_back("Too many people for room.");
    result.push_back("There are rooms overlapping.");
    result.push_back("Could not find a boundary iterator.");
    result.push_back("Invalid bound assigned.");
    result.push_back("Person's ward not found.");
    result.push_back("Person's room not found.");
    
    result.push_back("Tried to add duplicate hospital.");
    result.push_back("There are no hospitals.");
    result.push_back("Couldn't find a hospital.");
    
    
    
    
    
    
    /*string result[numCodes] =
     {
     result[SUCCESS] = "Success.",
     result[FAILURE] = "Something went wrong.",
     result[UNKNOWN_ERROR] = "There was an unknown error.",
     result[PERSON_ALREADY_EXISTS] = "Tried to add duplicate person.",
     result[WARD_FULL] = "Ward is full.",
     result[NO_WARDS] = "There are no wards."
     };*/
    
    return result[error];
}
