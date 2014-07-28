/*******************************************************************************
* IntelHex - class definitions                                                 *
*                                                                              *
* A class to handle the encoding and decoding of an Intel HEX format file as   *
* generated by many tool chains for embedded processors and microcontrollers.  *
*                                                                              *
********************************************************************************/

#pragma once
#include <iostream>
#include <map>
#include <list>
using namespace std;

class IntelHex
{


private:
	CString fileName;
	FILE *pHexFile;

	bool open();

public:
	IntelHex(void) { }
	IntelHex(const CString& hexFileName) {
		fileName = hexFileName;
	}

	~IntelHex(void) { 
		if (pHexFile){
			fclose(pHexFile);
		}
	}

	void parse();
	


	


};

