/*
CONFIGURACION DPLL
CABECERA esta seteada en CommCfg.SyncCharacter = 0xFECC.  Para un frame de komatsu creemos que debe ser CCFE  Y 
TAMBIEN DEBE SER INVERTIDA LA LINEA 77 (if (raw[i] == '\xCC' && raw[i + 1] == '\xFE') )   // si la conf es CCFE  , la funcion debe tener FE  CC

*/

#include <stdio.h>
#include <windows.h>
#include "Deviceinterface.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

bool bExitRq = false;
char* pStrDeviceName;
HANDLE hDevice;


const int BUFFER_SIZE = 500;
char TBuffer[500];
char RBuffer[500];

bool detectederror;
BOOL bSuccess;
DCB dcb;
DWORD dwTemp;
COMMTIMEOUTS cto;
DWORD lasterror;
SSI_COMM_CONFIG     CommCfg;
ULONG sData;
ULONG ic;
int iloop;
DWORD	ModemStatus;		// status of modem control inputs

void printBuffer() {
	for (int i = 0; i < BUFFER_SIZE; ++i) {
		//std::cout << static_cast<int>(TBuffer[i]) << " ";
		//std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(TBuffer[i]) << " ";
		std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(RBuffer[i])) << " ";
	}
	std::cout << std::endl;
}

// antiguo extractor 
/*std::vector<char> extractData(const char* raw, int size) {
	std::vector<char> data;
	bool dataStarted = false;

	for (int i = 0; i < size - 1; ++i) {
		if (raw[i] == '\xCC' && raw[i + 1] == '\xFE') {
			dataStarted = true;
			data.push_back(raw[i]); // Include sync bytes in the extracted data
			data.push_back(raw[i + 1]);
			i += 1; // Skip the two-byte sequence
		}
		else if (dataStarted) {
			if (raw[i] == '\x00' && raw[i + 1] == '\x00') {
				break; // End of data
			}
			data.push_back(raw[i]);
		}
	}

	return data;
}*/
std::vector<char> extractData(const char* raw, int size) {
	std::vector<char> data;
	bool dataStarted = false;

	for (int i = 0; i < size - 1; ++i) {
		if (raw[i] == '\xCC' && raw[i + 1] == '\xFE') {
			dataStarted = true;
			data.push_back(raw[i]); // Include sync bytes in the extracted data
			data.push_back(raw[i + 1]);
			i += 1; // Skip the two-byte sequence
		}
		else if (dataStarted) {
			if (raw[i] == '\x00' && raw[i + 1] == '\x00') {
				break; // End of data
			}
			data.push_back(raw[i]);
		}
	}

	return data;
}
void writeDataToFile(const std::vector<char>& data, const std::string& filename) {
	// Abrir el archivo para escribir al final
	std::ofstream file(filename + ".csv", std::ios::out | std::ios::app); // Agrega la extensión .csv al nombre del archivo
	if (file.is_open()) {
		std::stringstream hexStream; // Stream para almacenar la representación hexadecimal
		hexStream << std::hex << std::setfill('0'); // Configuración del stream para escribir en hexadecimal y rellenar con ceros
		// Escribir los datos en formato hexadecimal en el archivo
		for (char c : data) {
			hexStream << std::setw(2) << std::uppercase << static_cast<int>(static_cast<unsigned char>(c)); // Escribe el byte en hexadecimal en mayúsculas
			hexStream << ","; // Agrega una coma para separar los bytes
		}

		std::string hexString = hexStream.str();
		if (!hexString.empty()) {

			// Escribe la cadena hexadecimal en el archivo
			file << hexStream.str() << std::endl;
			std::cout << "Los datos se han añadido al archivo " << filename << ".csv" << std::endl;

		}

		// Cerrar el archivo
		file.close();
		
	}
	else {
		std::cerr << "Error al abrir el archivo " << filename << ".csv" << " para escribir" << std::endl;
	}
}

std::string getTimestampString() {
	auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::system_clock::to_time_t(now);
	std::tm timeinfo;
	localtime_s(&timeinfo, &timestamp);

	// Formatear la fecha y hora manualmente
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << timeinfo.tm_mday
		<< std::setw(2) << (timeinfo.tm_mon + 1)
		<< (1900 + timeinfo.tm_year)
		<< std::setw(2) << timeinfo.tm_hour
		<< std::setw(2) << timeinfo.tm_min
		<< std::setw(2) << timeinfo.tm_sec;
	return oss.str();
}

void printBufferDos(const std::vector<char>& buffer) {
	for (char c : buffer) {
		std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(static_cast<unsigned char>(c)) << " ";
	}
	std::cout << std::endl;
}


int main(int argc, char* argv[])
{
	hDevice = CreateFile(
		L"\\\\.\\SeaMAC0",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		lasterror = GetLastError();
		printf("unable to open \"%s\"\n", pStrDeviceName);
		return (1);
	}


	// get initial state of the device's DCB structure
	bSuccess =
		GetCommState(
			hDevice,                    // returned from CreateFile()
			&dcb                        // will contain current timeout values
		);

	if (!bSuccess)
	{
		DWORD dwLastError = GetLastError();
		printf("error %u from GetCommState\n", dwLastError);
	}

	// get initial state of the device's SSI_COMM_CONFIG structure
	bSuccess =
		DeviceIoControl(
			hDevice,                    // returned from CreateFile()
			IOCTL_SSI_GET_COMM_CONFIG,  // I/O control code
			NULL, 0,                    // no input to this function
			&CommCfg, sizeof CommCfg,   // indicate return buffer
			&dwTemp,                    // bytes returned not interesting
			NULL                        // not overlapped (even if opened overlapped)
		);

	if (!bSuccess)
	{
		DWORD dwLastError = GetLastError();
		printf("error %u from IOCTL_SSI_GET_COMM_CONFIG\n", dwLastError);
	}


	
	CommCfg.Electrical = ssiElectricalRS485;   // ssiElectricalRS232;
	CommCfg.Framing = ssiFramingCharacterSync;
	CommCfg.ClockEncoding = ssiClockEncodingNrz;
	CommCfg.SdlcAddressMode = ssiAddressModeFromAny;
	CommCfg.RsetSource = ssiTimingDpll;
	CommCfg.TsetSource = ssiTimingDpll;

	CommCfg.PreTxDelayTime = 0;
	CommCfg.PostTxDelayTime = 0;
	CommCfg.DpllBaseFromRset = false;
	CommCfg.BrgSourceFromRset = false;
	CommCfg.CrcPolynomial = ssiCrcCcitt;
	CommCfg.CharacterSize = 8;
	CommCfg.Parity = NOPARITY;
	CommCfg.IdleMode = ssiIdleSync;  ///ssiIdleSpace; // ssiIdleSync;
	CommCfg.Loopback = false;
	CommCfg.Echo = false;
	CommCfg.BitRate = 1000;

	CommCfg.PreTxDelayTime = 0;
	CommCfg.PostTxDelayTime = 0;

	CommCfg.CharacterSyncControl = 0;

	// PARAMETROS NUEVOS
	CommCfg.SyncCharacterSize = 16;
	CommCfg.SyncCharacter = 0xFECC; //OJO --> Puede ser 0xCCFE 
	CommCfg.CrcPresetOnes = true;
	CommCfg.PreamblePattern = ssiPreamblePatternOnes;
	CommCfg.PreambleLength = 0;

	dcb.fOutxCtsFlow = FALSE;                // don�t need CTS to send
	dcb.fOutxDsrFlow = FALSE;                // don�t need DSR to send
	dcb.fRtsControl = RTS_CONTROL_DISABLE;    //RTS_CONTROL_ENABLE;    // allow control of RTS
	dcb.Parity = NOPARITY;                   // no parity in this application
	dcb.BaudRate = CommCfg.BitRate;			// make it the same as above

	// update the device's DCB
	 
	bSuccess =
		SetCommState(
			hDevice,                    // returned from CreateFile()
			&dcb                        // new values
		);
	if (!bSuccess)
	{
		DWORD dwLastError = GetLastError();
		printf("error %u from SetCommState\n", dwLastError);

	}


	// update the device's SSI_COMM_CONFIG structure

	bSuccess =
		DeviceIoControl(
			hDevice,                    // returned from CreateFile()
			IOCTL_SSI_SET_COMM_CONFIG,  // I/O control code
			&CommCfg, sizeof CommCfg,   // indicate address of structure
			NULL, 0,                    // no output from this function
			&dwTemp,                    // bytes returned not interesting
			NULL                        // not overlapped (even if opened overlapped)
		);


	if (!bSuccess)
	{
		DWORD dwLastError = GetLastError();
		printf("error %u from IOCTL_SSI_SET_COMM_CONFIG\n", dwLastError);
	}


	// get initial timeouts settings before changing them
	bSuccess =
		GetCommTimeouts(
			hDevice,                    // returned from CreateFile()
			&cto                        // will contain current timeout values
		);

	if (!bSuccess)
	{
		DWORD dwLastError = GetLastError();
		printf("error %u from GetCommTimeouts\n", dwLastError);
	}

	// change time out
	cto.WriteTotalTimeoutMultiplier = 0;     // no per-character timeout
	cto.WriteTotalTimeoutConstant = 1000;    // wait a total
	cto.ReadIntervalTimeout = 0;             // no interval timeout
	cto.ReadTotalTimeoutMultiplier = 0;      // no per-character timeout
	cto.ReadTotalTimeoutConstant = 1000;    // wait a total

	bSuccess =
		SetCommTimeouts(
			hDevice,                    // returned from CreateFile()
			&cto);                        // new timeout values

	if (!bSuccess)
	{
		DWORD dwLastError = GetLastError();
		printf("error %u from SetCommTimeouts\n", dwLastError);
	}


	// initialize the buffer to all values from 0 to 255
	for (int fcp = 0; fcp < 256; fcp++)
	{
		TBuffer[fcp] = fcp;
	}

	printf("Beginning test\n");


	iloop = 0;
	detectederror = false;



	Sleep(500);	 // make sure it has fully come in

/****************************************************************/
// start while 

	std::string filename = "data_" + getTimestampString();



	printf("Iniciamos la lectura\n");

	while (iloop++ < 50)
	{
		printf("*");

		// hCommPort

		//memset(buf, 0, 256);

		memset(RBuffer, 0x00, 256);
		bSuccess =
			ReadFile(
				hDevice,                    // returned from CreateFile()
				&RBuffer,                     // will contain the received data
				256,				         // size of passed buffer
				&sData,                     // actual amount of returned data, in bytes
				NULL                        // non-overlapped read
			);
		if (!bSuccess)
		{
			DWORD dwLastError = GetLastError();
			printf("Read Error %u \n", dwLastError);
		}

		int size = sizeof(RBuffer) - 1; // Resta 1 para excluir el carácter nulo final

		// Llama a extractData
		std::vector<char> extractedData = extractData(RBuffer, size);

		writeDataToFile(extractedData, filename);
		//printBuffer();

		printBufferDos(extractedData);

	} 	

//// end while 
/********************************************/

	CloseHandle(hDevice);

	if (detectederror)
	{
		printf("\ntest failed\n");
		return 1;
	}
	else
	{
		printf("\ntest passed\n");
		return 0;
	}

	return 1;				
}