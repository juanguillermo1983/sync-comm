/******************************************************************************
	SeaMAC - Synchronous Serial Driver
	ssistatus.h
	¬ 2002-2017 Sealevel Systems, Inc.
*//**

	@file
	@brief Sealevel Status codes header

	This file contains status code definitions.
******************************************************************************/
//
//  Status values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-------------------------+-------------------------------+
//  |Sev|C|       Facility          |               Code            |
//  +---+-+-------------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//


//
// %1 is reserved by the IO Manager. If IoAllocateErrorLogEntry is
// called with a device, the name of the device will be inserted into
// the message at %1. Otherwise, the place of %1 will be left empty.
// In either case, the insertion strings from the driver's error log
// entry starts at %2. In other words, the first insertion string goes
// to %2, the second to %3 and so on.
//

//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_RPC_STUBS               0x3
#define FACILITY_RPC_RUNTIME             0x2
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: STATUS_SSI_INFO
//
// MessageText:
//
// %2
//
#define STATUS_SSI_INFO                  ((NTSTATUS)0x40040001L)

//
// MessageId: STATUS_SSI_WARNING
//
// MessageText:
//
// %2
//
#define STATUS_SSI_WARNING               ((NTSTATUS)0x80040002L)

//
// MessageId: STATUS_SSI_DEVICE_INIT_SUCCEEDED
//
// MessageText:
//
// Device initialization successful: %1
//
#define STATUS_SSI_DEVICE_INIT_SUCCEEDED ((NTSTATUS)0x40040003L)

//
// MessageId: STATUS_SSI_DEVICE_INIT_FAILED
//
// MessageText:
//
// Device initialization failed: %1.
//
#define STATUS_SSI_DEVICE_INIT_FAILED    ((NTSTATUS)0xC0040004L)

//
// MessageId: STATUS_SSI_FRAME_ABORTED
//
// MessageText:
//
// Frame Aborted
//
#define STATUS_SSI_FRAME_ABORTED         ((NTSTATUS)0xC0040005L)

//
// MessageId: STATUS_SSI_FRAME_ABORTED_PARTIAL_DATA
//
// MessageText:
//
// Frame Aborted
//
#define STATUS_SSI_FRAME_ABORTED_PARTIAL_DATA ((NTSTATUS)0x40040005L)

//
// MessageId: STATUS_SSI_CRC_ERROR_PARTIAL_DATA
//
// MessageText:
//
// A cyclic redundancy check (CRC) checksum error occurred.
//
#define STATUS_SSI_CRC_ERROR_PARTIAL_DATA ((NTSTATUS)0x40040006L)

//
// MessageId: STATUS_SSI_DATA_OVERRUN_PARTIAL_DATA
//
// MessageText:
//
// A data overrun error occurred.
//
#define STATUS_SSI_DATA_OVERRUN_PARTIAL_DATA ((NTSTATUS)0x40040007L)

//
// MessageId: STATUS_SSI_DEVICE_CRIPPLED
//
// MessageText:
//
// %1 operating without normal feature: %2
//
#define STATUS_SSI_DEVICE_CRIPPLED       ((NTSTATUS)0x80040008L)

//
// MessageId: STATUS_SSI_NTSTATUS
//
// MessageText:
//
// NT status in event data for device %1.
//
#define STATUS_SSI_NTSTATUS              ((NTSTATUS)0xC0040009L)

#pragma once
