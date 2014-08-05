#include "StdAfx.h"
#include "HexBinController.h"


HexBinController::~HexBinController(void) {
}

void HexBinController::init(CString& fileName) {
	CString hexFileName; 
	CString binFileName;

	string fileNameStr = CT2A(fileName);

	if (isHexFile(fileName)) {
		hexFileName = fileName;

		int pos = fileNameStr.find(".hex");
		fileNameStr.replace(pos, 4, ".bin");
		
		binFileName = fileNameStr.c_str();
		

	} else {
		binFileName = fileName;

		int pos = fileNameStr.find(".bin");
		fileNameStr.replace(pos, 4, ".hex");
		
		hexFileName = fileNameStr.c_str();
	}

	_hex = IntelHex(hexFileName);
	_bin = Bin(binFileName);
}

bool HexBinController::isHexFile(const CString& fileName) {

	CString fileExtension = PathFindExtension(fileName);
	fileExtension.MakeLower();

	if (fileExtension == _T(".hex"))
	{
		return true;
	}

	return false;
}

void HexBinController::parse() {
	
	//_hex.parse();
	_bin.parse();

}

void HexBinController::getEditFieldText(CString& hexField, CString& binField) {
	hexField = _hex.getEditFieldText().c_str();
	binField = _bin.getEditFieldText().c_str();
}

void HexBinController::getFilePath(CString& hexPath, CString& binPath) {
	hexPath = _hex.getFilePath().c_str();
	binPath = _bin.getFilePath().c_str();
}


	// output
void HexBinController::writeToBinFile() {

	FILE* pFileHandler = _bin.getFileWriteHandler();
	_hex.writeToBinFile(pFileHandler);

}