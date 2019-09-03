#include <windows.h>
#include <iostream>
#include <time.h>
#include <string>
#include <assert.h>

#include "detours.h"

#pragma comment( lib, "detours.lib" )

// #define IOCTL_DISK_GET_DRIVE_GEOMETRY_EX \
//   CTL_CODE( IOCTL_DISK_BASE, 0x0028, METHOD_BUFFERED, FILE_ANY_ACCESS )

// #define IOCTL_STORAGE_QUERY_PROPERTY \
//   CTL_CODE( IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS )

using tDeviceIoControl = decltype( DeviceIoControl )*;

tDeviceIoControl original_device_io_control = DeviceIoControl;

// char* flipAndCodeBytes( const char* str, int pos, int flip, char* buf ) {
//  int i;
//  int j = 0;
//  int k = 0;
//
//  buf[ 0 ] = '\0';
//  if ( pos <= 0 )
//    return buf;
//
//  if ( !j ) {
//    char p = 0;
//
//    // First try to gather all characters representing hex digits only.
//    j = 1;
//    k = 0;
//    buf[ k ] = 0;
//    for ( i = pos; j && str[ i ] != '\0'; ++i ) {
//      char c = tolower( str[ i ] );
//
//      if ( isspace( c ) )
//        c = '0';
//
//      ++p;
//      buf[ k ] <<= 4;
//
//      if ( c >= '0' && c <= '9' )
//        buf[ k ] |= ( unsigned char )( c - '0' );
//      else if ( c >= 'a' && c <= 'f' )
//        buf[ k ] |= ( unsigned char )( c - 'a' + 10 );
//      else {
//        j = 0;
//        break;
//      }
//
//      if ( p == 2 ) {
//        if ( buf[ k ] != '\0' && !isprint( buf[ k ] ) ) {
//          j = 0;
//          break;
//        }
//        ++k;
//        p = 0;
//        buf[ k ] = 0;
//      }
//    }
//  }
//
//  if ( !j ) {
//    // There are non-digit characters, gather them as is.
//    j = 1;
//    k = 0;
//    for ( i = pos; j && str[ i ] != '\0'; ++i ) {
//      char c = str[ i ];
//
//      if ( !isprint( c ) ) {
//        j = 0;
//        break;
//      }
//
//      buf[ k++ ] = c;
//    }
//  }
//
//  if ( !j ) {
//    // The characters are not there or are not printable.
//    k = 0;
//  }
//
//  buf[ k ] = '\0';
//
//  if ( flip )
//    // Flip adjacent characters
//    for ( j = 0; j < k; j += 2 ) {
//      char t = buf[ j ];
//      buf[ j ] = buf[ j + 1 ];
//      buf[ j + 1 ] = t;
//    }
//
//  // Trim any beginning and end space
//  i = j = -1;
//  for ( k = 0; buf[ k ] != '\0'; ++k ) {
//    if ( !isspace( buf[ k ] ) ) {
//      if ( i < 0 )
//        i = k;
//      j = k;
//    }
//  }
//
//  if ( ( i >= 0 ) && ( j >= 0 ) ) {
//    for ( k = i; ( k <= j ) && ( buf[ k ] != '\0' ); ++k )
//      buf[ k - i ] = buf[ k ];
//    buf[ k - i ] = '\0';
//  }
//
//  return buf;
//}

std::string GenerateNumberString( const uint32_t length ) {
  std::string new_string = "";
  for ( uint32_t i = 0; i < length; ++i )
    new_string += std::to_string( rand() % 9 );
  return new_string;
}

#pragma pack( 1 )

typedef struct _IDENTIFY_DATA {
  USHORT GeneralConfiguration;  // 00 00
  USHORT NumberOfCylinders;  // 02  1
  USHORT Reserved1;  // 04  2
  USHORT NumberOfHeads;  // 06  3
  USHORT UnformattedBytesPerTrack;  // 08  4
  USHORT UnformattedBytesPerSector;  // 0A  5
  USHORT SectorsPerTrack;  // 0C  6
  USHORT VendorUnique1[ 3 ];  // 0E  7-9
  USHORT SerialNumber[ 10 ];  // 14  10-19
  USHORT BufferType;  // 28  20
  USHORT BufferSectorSize;  // 2A  21
  USHORT NumberOfEccBytes;  // 2C  22
  USHORT FirmwareRevision[ 4 ];  // 2E  23-26
  USHORT ModelNumber[ 20 ];  // 36  27-46
  UCHAR MaximumBlockTransfer;  // 5E  47
  UCHAR VendorUnique2;  // 5F
  USHORT DoubleWordIo;  // 60  48
  USHORT Capabilities;  // 62  49
  USHORT Reserved2;  // 64  50
  UCHAR VendorUnique3;  // 66  51
  UCHAR PioCycleTimingMode;  // 67
  UCHAR VendorUnique4;  // 68  52
  UCHAR DmaCycleTimingMode;  // 69
  USHORT TranslationFieldsValid : 1;  // 6A  53
  USHORT Reserved3 : 15;
  USHORT NumberOfCurrentCylinders;  // 6C  54
  USHORT NumberOfCurrentHeads;  // 6E  55
  USHORT CurrentSectorsPerTrack;  // 70  56
  ULONG CurrentSectorCapacity;  // 72  57-58
  USHORT CurrentMultiSectorSetting;  //     59
  ULONG UserAddressableSectors;  //     60-61
  USHORT SingleWordDMASupport : 8;  //     62
  USHORT SingleWordDMAActive : 8;
  USHORT MultiWordDMASupport : 8;  //     63
  USHORT MultiWordDMAActive : 8;
  USHORT AdvancedPIOModes : 8;  //     64
  USHORT Reserved4 : 8;
  USHORT MinimumMWXferCycleTime;  //     65
  USHORT RecommendedMWXferCycleTime;  //     66
  USHORT MinimumPIOCycleTime;  //     67
  USHORT MinimumPIOCycleTimeIORDY;  //     68
  USHORT Reserved5[ 2 ];  //     69-70
  USHORT ReleaseTimeOverlapped;  //     71
  USHORT ReleaseTimeServiceCommand;  //     72
  USHORT MajorRevision;  //     73
  USHORT MinorRevision;  //     74
  USHORT Reserved6[ 50 ];  //     75-126
  USHORT SpecialFunctionsEnabled;  //     127
  USHORT Reserved7[ 128 ];  //     128-255
} IDENTIFY_DATA, *PIDENTIFY_DATA;

#pragma pack()

BOOL WINAPI DeviceIoControlHooked( HANDLE hDevice,
                                   DWORD dwIoControlCode,
                                   LPVOID lpInBuffer,
                                   DWORD nInBufferSize,
                                   LPVOID lpOutBuffer,
                                   DWORD nOutBufferSize,
                                   LPDWORD lpBytesReturned,
                                   LPOVERLAPPED lpOverlapped ) {
  // #define DFP_GET_VERSION 0x00074080
  //#define DFP_RECEIVE_DRIVE_DATA 0x0007c088
  // #define IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

  /*
  constexpr auto lol = DFP_GET_VERSION;
  constexpr auto lol2 = SMART_GET_VERSION;
  constexpr auto lo3 = SMART_RCV_DRIVE_DATA;
  constexpr auto lol4 = IOCTL_DISK_GET_DRIVE_GEOMETRY_EX;
  constexpr auto lol6 = DFP_RECEIVE_DRIVE_DATA;
  constexpr auto lol5 = IOCTL_SCSI_MINIPORT;
  */

  bool ret = original_device_io_control(
      hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer,
      nOutBufferSize, lpBytesReturned, lpOverlapped );

  // char original_serial_number[ MAX_PATH ]{ 0 };
  std::string original_serial_number( MAX_PATH, '\0' );
  std::string new_serial_number = "";

  static bool serial_changed = false;

  if ( ret ) {
    switch ( dwIoControlCode ) {
      case SMART_RCV_DRIVE_DATA: {
        std::cout << "SMART_RCV_DRIVE_DATA" << std::endl;

        PIDENTIFY_DATA pIdSector =
            ( PIDENTIFY_DATA )( ( PSENDCMDOUTPARAMS )lpOutBuffer )->bBuffer;

        memcpy( ( void* )original_serial_number.data(), pIdSector->SerialNumber,
                sizeof( pIdSector->SerialNumber ) );

        new_serial_number =
            std::string( "     WD-WCAV5V" ) + GenerateNumberString( 6 );

        strcpy( ( char* )pIdSector->SerialNumber, new_serial_number.c_str() );

        serial_changed = true;
      } break;

      case IOCTL_STORAGE_QUERY_PROPERTY: {
        auto dev_desc =
            reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>( lpOutBuffer );

        strcpy( ( char* )original_serial_number.data(),
                ( char* )lpOutBuffer + dev_desc->SerialNumberOffset );

        new_serial_number =
            std::string( "     WD-WCAV5V" ) + GenerateNumberString( 6 );

        strcpy( ( char* )lpOutBuffer + dev_desc->SerialNumberOffset,
                new_serial_number.c_str() );

        serial_changed = true;
      } break;
      default:
        break;
    }

    if ( serial_changed ) {
      std::cout << "Original Serial Number: " << original_serial_number
                << std::endl;
      std::cout << "New Serial Number: " << new_serial_number << std::endl;
    }
  }

  std::wcout << TEXT( "hDevice: " ) << hDevice << std::endl;
  std::wcout << TEXT( "dwIoControlCode: " ) << dwIoControlCode << std::endl;
  std::wcout << TEXT( "lpInBuffer: " ) << lpInBuffer << std::endl;
  std::wcout << TEXT( "nInBufferSize: " ) << nInBufferSize << std::endl;
  std::wcout << TEXT( "lpOutBuffer: " ) << lpOutBuffer << std::endl;
  std::wcout << TEXT( "nOutBufferSize: " ) << nOutBufferSize << std::endl;
  std::wcout << TEXT( "lpBytesReturned: " ) << lpBytesReturned << std::endl;
  std::wcout << TEXT( "lpOverlapped: " ) << lpOverlapped << std::endl;
  std::wcout << TEXT( " - " ) << std::endl;

  return ret;
}

BOOL WINAPI DllMain( HINSTANCE instance_handle,
                     DWORD reason,
                     LPVOID reserved ) {
  constexpr int test = IOCTL_STORAGE_QUERY_PROPERTY;
  switch ( reason ) {
    case DLL_PROCESS_ATTACH: {
      AllocConsole();
      freopen( "CONOUT$", "w", stdout );

      std::wcout << TEXT( "Injected!" ) << std::endl;

      srand( static_cast<unsigned int>(time( 0 )) );

      DetourTransactionBegin();

      DetourUpdateThread( GetCurrentThread() );

      DetourAttach( reinterpret_cast<PVOID*>( &original_device_io_control ),
                    DeviceIoControlHooked );

      if ( DetourTransactionCommit() != NO_ERROR )
        return FALSE;
    } break;

    default:
      break;
  }
  return TRUE;
}