#ifndef ERRORCODE_H_
#define ERRORCODE_H_

#include <string>

namespace ErrorCode
{
    
    //success is 0 so any errors can always be tested
    //by checking if an ERROR_CODE is non zero
    enum ERROR_CODE
    {
        SUCCESS,
        FAILURE,
        UNKNOWN_ERROR,
        PERSON_ALREADY_EXISTS,
        PERSON_NOT_EXIST,
        WARD_FULL,
        WARD_EMPTY,
        NO_WARDS,
        TOO_MANY_PEOPLE_FOR_WARD,
        WARD_OVERLAP,
        ROOM_FULL,
        ROOM_EMPTY,
        NO_ROOMS,
        TOO_MANY_PEOPLE_FOR_ROOM,
        ROOM_OVERLAP,
        BOUND_IT_NOT_FOUND,
        INVALID_BOUND,
        WARD_NOT_FOUND,
        ROOM_NOT_FOUND,
        
        HOSPITAL_ALREADY_EXISTS,
        NO_HOSPITALS,
        HOSPITAL_NOT_FOUND
    };

    std::string interpretError(ERROR_CODE error);
};
#endif
