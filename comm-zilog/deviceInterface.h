/******************************************************************************
    SeaMAC - Synchronous Serial Driver
    deviceInterface.h
    © 2002-2017 Sealevel Systems, Inc.
*//**

    @file
    @brief Device Interface header

    This file defines data structures exposed to user mode applications.
******************************************************************************/

#ifndef _SSISMV4_INTERFACE_
#define _SSISMV4_INTERFACE_

#include "ssistatus.h"

#pragma pack(push,sss,8)

/**
    Logical name base for CreateFile().
    Append unit number for logical name.
**/
#define SSISMV4_LOGICAL_NAME_BASE   "SeaMAC"

/* This GUID identifies the device interface class used by the device */
DEFINE_GUID(Ssismv4Device_CLASS_GUID,
    0x738e9738, 0x91e0, 0x4860, 0xbe, 0xef, 0x4f, 0xe, 0x6, 0x7b, 0x76, 0xfc);

/** @name Communications Configuration **/
/**@{*/

/** Framing type **/
typedef enum
{
    ssiFramingUnspecified = 0,
    ssiFramingAsync,                   /**< Async                                  */
    ssiFramingCharacterSync,           /**< BiSync, Monosync, DDCMP                */
    ssiFramingExternalSync,            /**< 8-bit w/External Sync                  */
    ssiFramingSdlc,                    /**< SDLC/HDLC bit-synchronous              */
    ssiFramingRaw,                     /**< no framing                             */
    ssiFramingCharacterSyncISO1745,    /**< BiSync with ISO1745 control characters */
    ssiFramingCharacterSyncISO2111,    /**< (FUTURE) Transparent BiSync			   */
    ssiFramingCharacterSyncNAH,        /**< BiSync, Monosync, DDCMP, with No Automatic Hunt */
    ssiFramingMaximum = ssiFramingCharacterSyncNAH
} SSI_FRAMING_ENUM;

/** Source clock **/
typedef enum
{
    ssiTimingUnspecified = 0,
    ssiTimingRset,              /**< RSET - Receive Signal Element Timing    */
    ssiTimingTset,              /**< TSET - Transmit Signal Element Timing   */
    ssiTimingBrg,               /**< BRG  - Bit Rate Generator               */
    ssiTimingDpll,              /**< DPLL - Digital Phase-Locked Loop (SDLC modes) */
    ssiTimingMaximum = ssiTimingDpll
} SSI_TIMING_SOURCE_ENUM;

/** SDLC Encoding Type (uses DPLL) **/
typedef enum
{
    ssiClockEncodingUnspecified = 0,
    ssiClockEncodingNone,        /**< Do not use DPLL                          */
    ssiClockEncodingNrz,         /**< SDLC NRZ  - Non-Return-to-Zero, Direct   */
    ssiClockEncodingNrzi,        /**< SDLC NRZI - Non-Return-to-Zero, Inverted */
    ssiClockEncodingFm0,         /**< SDLC FM0  - Biphase Space                */
    ssiClockEncodingBiPhaseSpace = ssiClockEncodingFm0,
    ssiClockEncodingFm1,         /**< SDLC FM1  - Biphase Mark                 */
    ssiClockEncodingBiPhaseMark = ssiClockEncodingFm1,
    ssiClockEncodingManchester,  /**< SDLC Manchester Encoding (not supported for Tx on 85230/85233) */
    ssiClockEncodingBiPhaseLevel = ssiClockEncodingManchester,
    ssiClockEncodingDifferentialManchester,  /**< Differential Manchester Encoding (not supported for Tx on 85230/85233) */
    ssiClockEncodingDifferentialBiPhaseLevel = ssiClockEncodingDifferentialManchester,
    ssiClockEncodingMaximum = ssiClockEncodingDifferentialManchester
} SSI_CLOCK_ENCODING_METHOD_ENUM;

/** Addressable receive mode **/
typedef enum
{
    ssiAddressModeUnspecified = 0,
    ssiAddressModeFromAny,      /**< Receive from any address        */
    ssiAddressModeFromSingle,   /**< Receive from single address     */
    ssiAddressModeFromGroup,    /**< Receive from group of addresses */
    ssiAddressModeMaximum = ssiAddressModeFromGroup
} SSI_ADDRESS_MODE_ENUM;

/** Electrical interface **/
typedef enum
{
    ssiElectricalUnspecified = 0,
    ssiElectricalHighZ,         /**< High Impedence         */
    ssiElectricalRS232,         /**< RS-232                 */
    ssiElectricalRS485T,        /**< RS-485 w/Termination   */
    ssiElectricalRS485,         /**< RS-485 w/o Termination */
    ssiElectricalLoop,          /**< Single-ended loopback  */
    ssiElectricalLoopDiff,      /**< Differential loopback  */
    ssiElectricalEIA530,        /**< TIA/EIA-530            */
    ssiElectricalEIA530A,       /**< TIA/EIA-530            */
    ssiElectricalV35,           /**< V.35                   */
    ssiElectricalMaximum = ssiElectricalV35
} SSI_ELECTRICAL_ENUM;

/** CRC type **/
typedef enum
{
    ssiCrcUnspecified = 0,
    ssiCrcNone,                 /**< No CRC                 */
    ssiCrcCcitt = 0x8408,       /**< CRC CCITT (SDLC)       */
    ssiCrcCrc16 = 0xA001,       /**< CRC CRC16 (Bisync)     */
    ssiCrcCrc32 = 0x04C11DB7,   /**< CRC CRC32 (Ethernet)   */
    ssiCrcMaximum = ssiCrcCrc16
} SSI_CRC_ENUM;

/** Idle mode **/
typedef enum
{
    ssiIdleUnspecified = 0,
    ssiIdleSync,                /**< No CRC                     */
    ssiIdleAltZeroAndOne,       /**< Alternate between 0 and 1  */
    ssiIdleContZero,            /**< Continuous 0               */
    ssiIdleContOne,             /**< Continuous 1               */
    ssiIdleAltMarkAndSpace,     /**< Alternate Mark and Space   */
    ssiIdleSpace,               /**< Continuous Space (TxD low) */
    ssiIdleMark,                /**< Continuous Mark (TxD high) */
    ssiIdleMaximum = ssiIdleMark
} SSI_IDLEMODE_ENUM;

/** Preamble pattern **/
typedef enum
{
    ssiPreamblePatternUnspecified = 0,
    ssiPreamblePatternZeroes,   /**< All Zeroes                */
    ssiPreamblePatternOnes,		/**< All Ones                  */
    ssiPreamblePattern1010,     /**< Send 1010101010...        */
    ssiPreamblePattern0101,     /**< Send 0101010101...        */
    ssiPreamblePatternMaximum = ssiPreamblePattern0101,
    ssiPreamblePatternFlags = ssiPreamblePatternOnes /**< All flags */
} SSI_PREAMBLE_PATTERN_ENUM;

/** Direction and Concurrency **/
typedef enum
{
    ssiDirConUnspecified = 0,
    ssiDirConSimplex,           /**< Transmit-Only               */
    ssiDirConHalfDuplex,        /**< Alternate between Tx and Rx */
    ssiDirConFullDuplex         /**< Concurrent Rx and Tx        */
} SSI_DIRCON_ENUM;

/** Source for outputting TSET **/
typedef enum
{
    ssiTsetSourceUnspecified = 0,
    ssiTsetSourceXtal,           /**< Xtal is source             */
    ssiTsetSourceTransmitClock,  /**< Transmit Clock (default)   */
    ssiTsetSourceBRG,            /**< Output the BRG             */
    ssiTsetSourceDPLL,           /**< Output the clock recovered from the DPLL*/
    ssiTsetSourceCHAR,		     /**< Output the character clock */
    ssiTsetSourceTXCOMPLETE,	 /**< Output transmit complete   */
    ssiTsetSourceBRG0,		     /**< Output BRG0                */
    ssiTsetSourceBRG1,		     /**< Output BRG1                */
    ssiTsetSourceCTR1		     /**< Output CTR1                */
} SSI_TSET_SOURCE_ENUM;

/** Comm configuration **/
typedef struct SSI_COMM_CONFIG_V4
{
    ULONG                   ConfigVersion;     /**< Version (==SSI_COMM_CONFIG_VERSION)  */

    SSI_ELECTRICAL_ENUM     Electrical;        /**< Electrical Interface         */

    /** @name Parameters common to all framing types */
    /**@{*/
    SSI_FRAMING_ENUM        Framing;           /**< Async, Sync, SDLC or Raw     */
    LONG                    ulReserved;        /**< Filler                       */
    LONG                    PreTxDelayTime;    /**<\# of msecs delay after CTS before transmitting */
    LONG                    PostTxDelayTime;   /**<\# of msecs delay after transmitting before dropping RTS */
    UCHAR                   CharacterSize;     /**< Character Size (5-8)         */
    UCHAR                   StopBits;          /**<\# of stop bits [Async]     */
    UCHAR                   Parity;            /**< Parity                       */
    BOOLEAN                 Loopback;          /**< If in local loopback mode    */
    BOOLEAN                 Echo;              /**< If in auto-echo mode         */
    BOOLEAN					MergeFrames;       /**< If writes should be merged	 */
    UCHAR                   uchReserved[2];    /**< Filler                       */
    /**@}*/

    /** @name Receive and Transmit Signal Element Timing (Rx & Tx Clocks) */
    /**@{*/
    SSI_TIMING_SOURCE_ENUM  RsetSource;        /**< Receive Timing Source        */
    SSI_TIMING_SOURCE_ENUM  TsetSource;        /**< Transmit Timing Source       */
    BOOLEAN                 TsetFromHere;      /**< if TSET is output, else input*/
    UCHAR                   uchReserved2[7];   /**< Filler                       */
    ULONG                   BitRate;           /**< Bit Rate if using BRG        */
    SSI_TSET_SOURCE_ENUM    TsetFromHereSource; /**< Source for outputting TSET   */
    /**@}*/

    /** @name Bit Rate Generator */
    /**@{*/
    ULONG                   Oscillator;        /**< Oscillator frequency         */
    BOOLEAN                 BrgSourceFromRset; /**< if BRG base is from RSET, else from Oscillator       */
    UCHAR                   uchReserved3[7];   /**< Filler                       */
    /**@}*/

    /** @name CRC */
    /**@{*/
    SSI_CRC_ENUM            CrcPolynomial;     /**< Which CRC polynomial to use  [SDLC, Sync] */
    BOOLEAN                 CrcPresetOnes;     /**< If to preset CRC to ones, else zeros [SDLC, Sync]  */
    SSI_IDLEMODE_ENUM       IdleMode;          /**< What to do between frames [SDLC, bisync, monosync] */
    UCHAR                   uchReserved4[3];   /**< Filler                       */
    /**@}*/

    /** @name Settings specific to SDLC framing */
    /**@{*/
    SSI_CLOCK_ENCODING_METHOD_ENUM ClockEncoding; /**< Clock Encoding Method [SDLC]            */
    SSI_ADDRESS_MODE_ENUM   SdlcAddressMode;   /**< Rx from Any, single, group [SDLC]          */
    UCHAR                   SdlcAddress;       /**< Receive Address [SDLC]                     */
    BOOLEAN                 DpllBaseFromRset;  /**< If DPLL base is from RSET, else BRG [SDLC] */
    BOOLEAN                 SdlcAutoRetransmit;/**< If to automatically re-transmit underruns  */
    UCHAR                   uchReserved5[5];   /**< Filler                       */
    /**@}*/

    /** @name Settings specific to character framing and no framing */
    /**@{*/
    UCHAR                   SyncCharacterSize;  /**< Sync Character Size (0, 6, 8, 12, 16) [Sync, Raw] */
    BOOLEAN					SdlcFlagsShareZero;	/**< if SDLC flags share a zero     */
    UCHAR					PreambleLength;	    /**< 0=none,8,16,32, or 64 valid    */
    SSI_PREAMBLE_PATTERN_ENUM PreamblePattern;  /**< if sent, then send this one	*/
    ULONG                   SyncCharacter;      /**< Mono-/Bi-sync sync character [Sync, Raw] */
    UCHAR                   uchReserved6[3];    /**< Filler                       */
    /**@}*/

    /** @name Direction and concurrency */
    /**@{*/
    SSI_DIRCON_ENUM         DirCon;            /**< Simplex, Half-Duplex or Full-Duplex */
    ULONG                   ulReserved7;       /**< Filler                       */
    /**@}*/

    /** @name Special control for bisync and monosync control characters */
    /**@{*/
    ULONG			    CharacterSyncControl;  /**< Control bits for character sync control */
    ULONG			    CharacterSyncMaxLength;  /**< Maximum charactersync length before hunt issued */
    ULONG				RxTrigger;				/**< Current receive trigger level */
    ULONG				TxTrigger;				/**< Current transmit trigger level */
    /**@}*/

    ULONG                   ulReserved99[10];  /**< Filler                       */
    BOOLEAN                 TSET485Ctrl;     /**< Tri-state TSET when data tristates if hardware supports it */
    ULONG			    Configuration9038;   /**< Special field for 9038 only */


} SSI_COMM_CONFIG_V4, * PSSI_COMM_CONFIG_V4;

#define SSI_COMM_CONFIG_VERSION_V4  4
#define SSI_COMM_CONFIG_VERSION SSI_COMM_CONFIG_VERSION_V4
#define SSI_COMM_CONFIG SSI_COMM_CONFIG_V4
#define PSSI_COMM_CONFIG PSSI_COMM_CONFIG_V4

/**@}*/


/** @name Hardware Configuration **/
/**@{*/

/** Device sub-type **/
typedef enum
{
    ssiDeviceSubtypeUnspecified = 0,
    ssiDeviceSubtype3514 = 0x3514,
    ssiDeviceSubtype3612 = 0x3612,
    ssiDeviceSubtypeACB1Port = 0x4000,
    ssiDeviceSubtype5101 = 0x5101,
    ssiDeviceSubtype5102 = 0x5102,
    ssiDeviceSubtypeE102 = 0xE102,
    ssiDeviceSubtype5103 = 0x5103,
    ssiDeviceSubtypeE103 = 0xE103,
    ssiDeviceSubtype5104 = 0x5104,
    ssiDeviceSubtypeE104 = 0xE104,
    ssiDeviceSubtypeE204 = 0xE204,
    ssiDeviceSubtype5105 = 0x5105,
    ssiDeviceSubtype5402 = 0x5402,
    ssiDeviceSubtypeE402 = 0xE402,
    ssiDeviceSubtype9038 = 0x9038,
    ssiDeviceSubtype9039 = 0x9039,
    ssiDeviceSubtype9063 = 0x9063,
    ssiDeviceSubtype9064 = 0x9064,
    ssiDeviceSubtype9113 = 0x9113,
    ssiDeviceSubtype9155 = 0x9155,
    ssiDeviceSubtype9205 = 0x9205,
} SSI_DEVICE_SUBTYPE_ENUM;

/** Hardware configuration, version 1 **/
typedef struct SSI_HW_CONFIG_V1
{
    SSI_DEVICE_SUBTYPE_ENUM  DeviceSubtype;
    ULONG               BusNumber;
    LARGE_INTEGER       IoBase;
    ULONG               IoCount;
    ULONG               IntLevel;
    ULONG               IntVector;
    ULONG               IntAffinity;
    UCHAR               PciDeviceNumber;
    UCHAR               PciFunctionNumber;
} SSI_HW_CONFIG_V1, * PSSI_HW_CONFIG_V1;

/* There is no version 2 */

/** Hardware configuration, version 3 **/
typedef struct SSI_HW_CONFIG_V4
{
    SSI_DEVICE_SUBTYPE_ENUM  DeviceSubtype;
    ULONG               BusNumber;
    LARGE_INTEGER       IoBase;
    ULONG               IoCount;
    ULONG               IntLevel;
    ULONG               IntVector;
    ULONG               IntAffinity;
    UCHAR               PciDeviceNumber;
    UCHAR               PciFunctionNumber;
    UCHAR               rgchReserved[2];
    ULONG               rgulReserved[3];
    LARGE_INTEGER       rgMemoryBase[2];
    ULONG               rgMemoryCount[2];
    ULONG               rgulReserved2[2];
} SSI_HW_CONFIG_V4, * PSSI_HW_CONFIG_V4;

#define SSI_HW_CONFIG SSI_HW_CONFIG_V4
#define PSSI_HW_CONFIG PSSI_HW_CONFIG_V4

/**@}*/


/** Buffer queue configuration **/
typedef struct SSI_QUEUE_CONFIG {
    ULONG QueueSize;                   /**< Number of queue entries          */
    ULONG BufferSize;                  /**< Size of each buffer in the queue */
} SSI_QUEUE_CONFIG, * PSSI_QUEUE_CONFIG;

/** IO statistics **/
typedef struct SSI_IO_STATISTICS {
    ULONG               TxFrames;      /**< Total Tx frames      */
    ULONG               RxFrames;      /**< Total Rx frames      */
    ULONG               CrcErrors;     /**< Total CRC errors     */
    ULONG               OverrunErrors; /**< Total overrun errors */
    ULONG               ParityErrors;  /**< Total parity errors  */
} SSI_IO_STATISTICS, * PSSI_IO_STATISTICS;


/** @name Driver version information **/
/**@{*/

/** Compatible Version e.g. if version is 3.11.xxxx.xxxx **/
typedef struct SSI_COMPATIBLE_VERSION {
    USHORT  usVersionMSHi;      /* e.g. 3       */
    USHORT  usVersionMSLo;      /* e.g. 11      */
} SSI_COMPATIBLE_VERSION, * PSSI_COMPATIBLE_VERSION;

/** Driver Version e.g. if version is 3.14.0123.0004 **/
typedef struct SSI_DRIVER_VERSION {

    USHORT  usFileVersionMSHi;      /* e.g. 3       */
    USHORT  usFileVersionMSLo;      /* e.g. 14      */
    USHORT  usFileVersionLSHi;      /* e.g. 123     */
    USHORT  usFileVersionLSLo;      /* e.g. 4       */

    /* flags (cf. winver.h) */
    ULONG   ulFileFlagsMask;        /* valid bits in ulFileFlags    */
    ULONG   ulFileFlags;            /* e.g. VFF_DEBUG | VFF_PRERELEASE */

    /* list of prior versions of driver that this version can act like */
    ULONG ulPriorVersionCompatibilityCount;
    SSI_COMPATIBLE_VERSION rgPriorVersionCompatibility[8];

} SSI_DRIVER_VERSION, * PSSI_DRIVER_VERSION;

/**@}*/


/** @name IoCtl Codes */
/**@{*/

#define SSI_FILE_DEVICE_SSISMV4 ((ULONG)40001)

#define SSI_CTL_CODE(code,method,access) \
    CTL_CODE(SSI_FILE_DEVICE_SSISMV4, 0x800+(code), method, access)

#define IOCTL_SSI_GET_VERSION_INFO          SSI_CTL_CODE(1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_SET_VERSION_COMPATIBILITY SSI_CTL_CODE(2, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_GET_HW_CFG                SSI_CTL_CODE(3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_SET_ELECTRICAL            SSI_CTL_CODE(4, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_GET_HW_SD                 SSI_CTL_CODE(5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_GET_COMM_CONFIG_V2        SSI_CTL_CODE(6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_SET_COMM_CONFIG           SSI_CTL_CODE(7, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_SET_LL                    SSI_CTL_CODE(10, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_CLR_LL                    SSI_CTL_CODE(11, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_SET_RL                    SSI_CTL_CODE(12, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_CLR_RL                    SSI_CTL_CODE(13, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_GET_LLRL                  SSI_CTL_CODE(14, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_SAVE_COMM_CONFIG          SSI_CTL_CODE(15, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_GET_BUFFER_COUNT          SSI_CTL_CODE(16, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_GET_IO_STATISTICS         SSI_CTL_CODE(17, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_RESET_IO_STATISTICS       SSI_CTL_CODE(18, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_GET_RX_BUFF_CONFIG        SSI_CTL_CODE(19, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_HUNT                      SSI_CTL_CODE(20, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_GET_COMM_CONFIG           SSI_CTL_CODE(21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_MODIFY_COMM_IDLEMODE      SSI_CTL_CODE(22, METHOD_BUFFERED, FILE_WRITE_ACCESS)
/*
    NOTE: The following IOCTL code is BETA and should not be used in this verison of the driver.
          In the future, it may allow expanded control of bisync read capabilities.
*/
#define IOCTL_SSI_MODIFY_COMM_CHARACTERSYNCCONTROL SSI_CTL_CODE(23, METHOD_BUFFERED, FILE_WRITE_ACCESS)
/*
    The following IOCTL code is BETA and should not be used in this verison of the driver.
    In the future, it will allow reading the data from the driver's buffers WITHOUT dequeueing it.
*/
#define IOCTL_SSI_PEEK		                SSI_CTL_CODE(24, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_GET_BAUD_TIMERS           SSI_CTL_CODE(25, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SSI_MODIFY_RXTRIGGER          SSI_CTL_CODE(26, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_SSI_MODIFY_TXTRIGGER          SSI_CTL_CODE(27, METHOD_BUFFERED, FILE_WRITE_ACCESS)

/* This one is secret and is not distributed in the "normal" file */
#define IOCTL_SSI_READ_SECURITY	 	    SSI_CTL_CODE(69, METHOD_BUFFERED, FILE_WRITE_ACCESS)


/* Standard Serial IoCtls that aren't available via Win32 */
#define IOCTL_SERIAL_GET_DTRRTS         CTL_CODE(FILE_DEVICE_SERIAL_PORT,30,METHOD_BUFFERED,FILE_ANY_ACCESS)

/**@}*/

#define SSI_DTR_STATE           0x00000001UL   /**< For GET_DTRRTS */
#define SSI_RTS_STATE           0x00000002UL   /**< For GET_DTRRTS */
#define SSI_LL_STATE            0x00001000UL   /**< For GET_LLRL */
#define SSI_RL_STATE            0x00002000UL   /**< For GET_LLRL */
#define RTS_CONTROL_AUTO_RTS	0x04           /**< Use auto rts feature of 85230 */

#pragma pack(pop,sss)

#endif	/* _SSISMV4_INTERFACE_ */#pragma once
