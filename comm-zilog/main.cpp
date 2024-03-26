/*
* SeaMACSample.cpp
*
* The following test uses RS-232 with HDLC/SDLC and CRC generation and checking
* turned on.  The test is run at 19.2K and requires the use of an external loopback.
* Please see the SeaMAC Help file for full details on all options available.
*
* This program assumes you have a loopback of some sort on the DB-25 connector.  For this test
* we only use a connection from transmit data (pin 3) to receive data pin (pins 2),
* a connection from RTS(pin 4) to CTS (pin 5), and a connection from DTR(pin 20) to DSR(pin 6).
* i.e. jumper 2-3, 4-5, and 6-20.
*
* Sealevel and SeaMAC are registered trademarks of Sealevel Systems
* Incorporated.
*
* Copyright (c) 2010-2017, Sealevel Systems, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/



#include <stdio.h>
#include <windows.h>
#include "Deviceinterface.h"
#include <iostream>

using namespace std;

char* pStrDeviceName;
HANDLE hDevice;

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
	// COMENTADO PARA EVITAR ERRROR SIN TARJETA SYNC 
	/*************************************************************************************/
	/*bSuccess =
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
	} */


	/*****************************************************************************************/
	/*****************************************************************************************/



	// NOTE:  The following test uses RS-232 with HDLC/SDLC and CRC generation and checking
	// turned on.  The test is run at 19.2K and does not use the internal loopbacks.
	// Please see the SeaMAC Help file for full details on all options available.
	CommCfg.Electrical = ssiElectricalRS232;
	CommCfg.Framing = ssiFramingSdlc;
	CommCfg.ClockEncoding = ssiClockEncodingNone;
	CommCfg.SdlcAddressMode = ssiAddressModeFromAny;
	CommCfg.PreTxDelayTime = 0;
	CommCfg.PostTxDelayTime = 0;
	CommCfg.RsetSource = ssiTimingBrg;
	CommCfg.TsetSource = ssiTimingBrg;
	CommCfg.DpllBaseFromRset = false;
	CommCfg.BrgSourceFromRset = false;
	CommCfg.CrcPolynomial = ssiCrcCcitt;
	CommCfg.CharacterSize = 8;
	CommCfg.Parity = NOPARITY;
	CommCfg.IdleMode = ssiIdleSync;
	CommCfg.Loopback = false;
	CommCfg.Echo = false;
	CommCfg.BitRate = 19200;

	CommCfg.PreTxDelayTime = 0;
	CommCfg.PostTxDelayTime = 0;

	CommCfg.CharacterSyncControl = 0;


	dcb.fOutxCtsFlow = FALSE;                // don�t need CTS to send
	dcb.fOutxDsrFlow = FALSE;                // don�t need DSR to send
	dcb.fRtsControl = RTS_CONTROL_ENABLE;    // allow control of RTS
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

	// COMENTADO PARA EVITAR ERROR SIN TARJETA 
	/*******************************************************************************************/
	/*bSuccess =
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
	*/

	/***********************************************************************************************/

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

	// Configura el puerto serie para recibir eventos de caracteres
	if (!SetCommMask(hDevice, EV_RXCHAR)) {
		std::cerr << "Error al configurar el puerto serie" << std::endl;
		CloseHandle(hDevice);
		return 1;
	}

	cout << "Esperando data ..." << endl;
	DWORD dwEventMask;
	DWORD dwSize = 0;
	// Espera a que ocurra un evento en el puerto serie
	if (WaitCommEvent(hDevice, &dwEventMask, NULL)) {
		DWORD dwIncommingReadSize;

		do {
			// Lee datos del puerto serie
			if (ReadFile(hDevice, RBuffer, sizeof(RBuffer), &dwIncommingReadSize, NULL) != 0) {
				if (dwIncommingReadSize > 0) {
					dwSize += dwIncommingReadSize;
					// Puedes procesar los datos recibidos aqui o almacenarlos en otro lugar seg�n sea necesario
					// Imprimir los datos recibidos
					for (DWORD i = 0; i < dwIncommingReadSize; i++) {
						std::cout << std::hex << static_cast<int>(RBuffer[i]) << " ";  //mPrint en HEX
					}
					std::cout << std::endl;

					for (DWORD i = 0; i < dwIncommingReadSize; i++) {
						if (isprint(RBuffer[i])) {
							std::cout << RBuffer[i]; // Si el caracter es imprimible, imprimelo   
						}
						else {
							std::cout << "."; // De lo contrario, imprime un punto
						}
					}
					std::cout << std::endl;
				}
			}
			else {
				std::cerr << "Error al leer datos del puerto serie" << std::endl;
				CloseHandle(hDevice);
				return 1;
			}
		} while (dwIncommingReadSize > 0);
	}
	else {
		std::cerr << "Error al esperar eventos en el puerto serie" << std::endl;
		CloseHandle(hDevice);
		return 1;
	}




	/*
		memset(RBuffer, 0x00, 300);
		bSuccess =
			ReadFile(
				hDevice,                    // returned from CreateFile()
				&RBuffer,                     // will contain the received data
				300,				         // size of passed buffer
				&sData,                     // actual amount of returned data, in bytes
				NULL                        // non-overlapped read
			);
		if (!bSuccess)
		{
			DWORD dwLastError = GetLastError();
			printf("Read Error %u \n", dwLastError);
		}
	
	
	*/



	//detectederror = true;
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

	return 1;				//just in case
}