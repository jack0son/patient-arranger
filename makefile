all: Person.o Patient.o Ward.o Room.o test.cpp Hospital.o ErrorCode.o PersonListSearch.o Community.o
	g++ Person.o Patient.o Ward.o Room.o Hospital.o ErrorCode.o test.cpp PersonListSearch.o Community.o

debug: Person.o Patient.o Ward.o test.cpp Hospital.o Room.o ErrorCode.o PersonListSearch.o Community.o
	g++ -g test.cpp Person.o Patient.o Ward.o Hospital.o Room.o ErrorCode.o PersonListSearch.o Community.o -g -o debug

rebuild:
	make Person
	make Patient
	make Ward
	make Hospital
	make ErrorCode
	make PersonListSearch

Person: Person.h Person.cpp
	g++ -c Person.cpp -g

Patient: Patient.h Patient.cpp
	g++ -c Patient.cpp -g

Ward: Ward.h Ward.cpp
	g++ -c Ward.cpp -g

Room: Room.h Room.cpp
	g++ -c Room.cpp -g

Hospital: Hospital.h Hospital.cpp
	g++ -c Hospital.cpp -g

ErrorCode: ErrorCode.h ErrorCode.cpp
	g++ -c ErrorCode.cpp -g
	
PersonListSearch: PersonListSearch.h PersonListSearch.cpp
	g++ -c PersonListSearch.cpp -g 
	
Community: Community.h Community.cpp
	g++ -c Community.cpp -g
	
