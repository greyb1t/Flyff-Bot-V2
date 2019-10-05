#include "pch.h"
#include "flyff_client_ignite.h"
#include "..\bot\logic_specialization\local_player_ignite.h"
#include "../bot/bot_initializer.h"

#include "gwinmem/process_memory_internal.h"

#include "gwinguiv2/message_box.h"

#include "../utils/trace.h"

using namespace bot;

HHOOK FlyffClientIgniteFlyff::wnd_proc_hook_handle_ = 0;
tCryptHashData FlyffClientIgniteFlyff::crypt_hash_data_original_ = 0;
uint8_t* FlyffClientIgniteFlyff::copied_buffer_ = nullptr;
uint32_t FlyffClientIgniteFlyff::text_section_virtual_size_ = 0;

FlyffClientIgniteFlyff::FlyffClientIgniteFlyff()
    : FlyffClient( TEXT( "Neuz.exe" ) ),
      collision_bytes_saved_{},
      collision_check_addr_( 0 ) {
  AddSearchFunction( MemoryContants::kD3dVec3ProjectAddress, [=]() {
    /*
      How to get this value:
      Follow the instructions in the file on desktop (Flyff Matrix.txt) to get
      the address of D3DXVec3Project
    */

    // The patterns for Ignite Flyff. Seems to be using a newer version of
    // D3dx9.lib
    int addr = pattern_matcher_.FindHexPattern(
        "\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x44\x8B\x4D\x1C\x33\xC0\x85",
        "xxxxxxxxxxxxxxx" );

    return addr;
  } );

  AddSearchFunction( MemoryContants::kPlayerBaseAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x0D\x00\x00\x00\x00\x85\xC9\x0F\x84\x00\x00\x00\x00\xA1\x00\x00"
        "\x00\x00\xF3",
        "xx????xxxx????x????x" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kScrollDistanceAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xF3\x0F\x11\x0D\x00\x00\x00\x00\x76\x63\xC7\x05\x00\x00\x00\x00\x00"
        "\x00\x00\x00\xB8\x00\x00\x00\x00",
        "xxxx????xxxx????????x????" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kAccountNameAddress, [=]() {
    //         uint32_t addr = pattern_matcher_.FindHexPattern(
    //                           "\x68\x00\x00\x00\x00\x8B\xCB\xC6\x80\xC1\x00\x00\x00\x01",
    //                           "x????xxxxxxxxx" );

    uint32_t addr =
        pattern_matcher_.FindIdaSignature( "80 3D ? ? ? ? ? 74 06 8B 03" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kSelectedEntityAddress, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xA1\x00\x00\x00\x00\x83\xB8\xDC\x00\x00\x00\x00", "x????xxxxxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 1 );

    return address;
  } );

  /*
    Finding Entity List Address on Ignite:
    Search for the string ClosestPlayer in IDA.
    That function iterates the entity list for entities.
  */
  AddSearchFunction( MemoryContants::kEntityListAddress, [=]() {
    /*
      int addr = pattern_matcher_.FindHexPattern(
      "\x8B\x34\xBD\x00\x00\x00\x00\x66\x39\x86\xC0\x02\x00\x00",
      "xxx????xxxxxxx");
    */

    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x00\x00\x00\x00\x00\x00\x85\xC9\x74\x32", "x??????xxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 3 );

    return address;
  } );

  AddSearchFunction( MemoryContants::kMovementOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "8B ? ? ? ? ? 8B 40 04 F7 D0 A8 01 74 08" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

#if 0
  AddSearchFunction( MemoryContants::kCollisionAddress, [=]() {
    // TODO: Won't work after the function has been patched

    // IF IT STOPS WORKING, TRY THE SIGNATURE BELOW SINCE update 2.2.0
    // I make the jmp's ? instead of x
    // \x81\x3D\x00\x00\x00\x00\x00\x00\x00\x00\x74\x66\x85\xC0\x74\x62
    // xx??????????xx??

    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x81\x3D\x00\x00\x00\x00\x00\x00\x00\x00\x74\x66", "xx????????xx" );

    ////uint32_t addr2 = pattern_matcher_.FindHexPattern(
    ////    "\x83\x3D\x00\x00\x00\x00\x00\x90\x90\x90\x0F\x57\xED",
    ////    "xx?????xxxxx?" );

    ////// If the first pattern is invalid, the user has attached the bot more
    ////// than two times to the same client because the bot has already
    /// patched
    ////// the collision. So we have to use another pattern for the patched
    ////// memory.
    ////if ( IsAddressInvalid( addr ) )
    ////  addr = addr2;

    collision_check_addr_ = addr;

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t address = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return address;
  } );
#endif

  AddSearchFunction( MemoryContants::kModelOffset, [=]() { return 0x17c; } );

  AddSearchFunction( MemoryContants::kBoundBoxOffset, [=]() { return 0xc; } );

  AddSearchFunction( MemoryContants::kWorldMatrixOffset,
                     [=]() { return 0xe4; } );

  AddSearchFunction( MemoryContants::kPositionOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xF3\x00\x00\x00\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\x8B\x00\x00"
        "\x00\x00\x00\x0F\xB6\x00",
        "x???????xx????x?????xxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return offset;
  } );

#if 0
  AddSearchFunction( MemoryContants::kSpeedOffset, [=]() {
    /*
      uint32_t addr = pattern_matcher_.FindHexPattern(
                      "\x8B\x00\x00\x00\x00\x00\x85\xFF\x74\x67\x53",
                      "x?????xxxxx" );
    */

    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8B\x00\x00\x00\x00\x00\x85\xDB\x74\x63\x57", "x?????xxxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );
#endif

  AddSearchFunction( MemoryContants::kHealthOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x83\xB9\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xF6\x81",
        "xx?????xx????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kManaOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x83\xB9\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xF6\x81",
        "xx?????xx????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset + 0x4;
  } );

  AddSearchFunction( MemoryContants::kFatigueOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x83\xB9\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xF6\x81",
        "xx?????xx????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset + 0x8;
  } );

  AddSearchFunction( MemoryContants::kLevelOffset, [=]() {
    // uint32_t addr = pattern_matcher_.FindHexPattern(
    //     "\xFF\xB1\x00\x00\x00\x00\x6A\x00", "xx????xx" );

    // offset is 0x434

    uint32_t addr = pattern_matcher_.FindIdaSignature(
        "8B ? ? ? ? ? 6A 00 68 ? ? ? ? FF B1 ? ? ? ? C1 E6 05 FF B1" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kNameOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x8D\x00\x00\x00\x00\x00\x75\x10\x8D\x47\x04", "x?????xxxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kObjectFlagsOffset,
                     [=]() { return 0x4; } );

  AddSearchFunction( MemoryContants::kSelectedEntityOffset,
                     [=]() { return 0x20; } );

  // Find a steady offset by logging in and out while finding out what is
  // accessing the value
  AddSearchFunction( MemoryContants::kSpeedMultiplierOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xF3\x00\x00\x00\x00\x00\x00\x00\x0F\x2F\x00\x00\x00\x00\x00\x76\x74"
        "\xB9",
        "x???????xx?????x?x" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 4 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kIgniteEntityIdOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\xFF\xB7\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85"
        "\xC0\x74\x0D",
        "xx????x????x????xxxx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );

  AddSearchFunction( MemoryContants::kMoveOffset, [=]() { return 0x4; } );

  /*
    AddSearchFunction(kSendMeleeAttackAddress, [=]() {
    int addr = pattern_matcher_.FindHexPattern(
      "\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x50\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xF0\x53\x56\x57\x50\x8D\x45\xF4\x00\x00\x00\x00\x00\x00\x8B\xD9\x8B\x75\x0C\x8D\x00\x00\x00\x00\x00\x6A\x00\x6A\x00\xE8\x00\x00\x00\x00\x6A\x04\x8D\x00\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x8D\x00\x00\x00\x00\x00\x6A\x04\xC7\x00\x00\x00\x00\x00\x83\x85\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x8D\x00\x00\x00\x00\x00\x6A\x04\xC7\x00\x00\x00\x00\x00\x83\x85\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x8B\x45\x08\x6A\x04\x89\x01\x8D\x00\x00\x00\x00\x00\x83\x85\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x8D\x00\x00\x00\x00\x00\x6A\x04\x89\x30\x83\x85\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x8B\x45\x10\x6A\x04\x89\x01\x8D\x00\x00\x00\x00\x00\x83\x85\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\x8B\x45\x14\x6A\x04\x89\x01\x8D\x00\x00\x00\x00\x00\x83\x85\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\xB9",
      "xxxxxx??????????xx????x????x????xxxxxxxxxxxx??????xxxxxx?????xxxxx????xxx?????xx?????x????x?????x?????xxxx????xx?????x????x?????x?????xxxx????xx?????x????x?????xxxxxxxx?????xx?????x????x?????x?????xxxxxx?????x????x?????xxxxxxxx?????xx?????x????x?????xxxxxxxx?????xx?????x????x?????x");

    return addr;
    });
  */

  AddSearchFunction( MemoryContants::kObjectTypeOffset, [=]() {
    uint32_t addr = pattern_matcher_.FindHexPattern(
        "\x83\xB9\x00\x00\x00\x00\x00\x75\x29", "xx?????xx" );

    if ( IsAddressInvalid( addr ) )
      return addr;

    uint32_t offset = gwinmem::CurrentProcess().Read<uint32_t>( addr + 2 );

    return offset;
  } );
}

LRESULT FlyffClientIgniteFlyff::CallWndProcHook( int code,
                                                 WPARAM wParam,
                                                 LPARAM lParam ) {
  auto botcore = Initializer().GetBotCore();

  if ( code < 0 )
    CallNextHookEx( wnd_proc_hook_handle_, code, wParam, lParam );

  if ( code == HC_ACTION ) {
    CWPSTRUCT* cwp = reinterpret_cast<CWPSTRUCT*>( lParam );

    switch ( cwp->message ) {
      case WM_KILLFOCUS: {
        SendMessage( botcore->GetTargetWindow(), WM_ACTIVATE, WA_ACTIVE, 0 );
      } break;

        /*
        case WM_CLOSE:
        {
        gWin::logger::DisplayError(TEXT("dickdikc"));
        } break;
        */

      default:
        break;
    }
  }

  return CallNextHookEx( 0, code, wParam, lParam );
}

bool ReadFileData( const std::wstring& filename,
                   std::vector<uint8_t>* file_data ) {
  gwinmem::SafeHandle handle =
      CreateFile( filename.c_str(), GENERIC_READ, FILE_SHARE_READ, 0,
                  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

  if ( handle.GetValue() == INVALID_HANDLE_VALUE )
    return false;

  DWORD file_size = GetFileSize( handle.GetValue(), 0 );

  std::vector<uint8_t> buf;

  buf.resize( file_size );

  DWORD bytes_read = 0;

  if ( !ReadFile( handle.GetValue(), &buf[ 0 ], file_size, &bytes_read, 0 ) )
    return false;

  if ( bytes_read != file_size )
    return {};

  *file_data = buf;

  return true;
}

bool FlyffClientIgniteFlyff::SnapshotTextSection() {
  auto get_pe_text_section =
      []( const IMAGE_NT_HEADERS* nt_headers ) -> IMAGE_SECTION_HEADER {
    for ( int i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i ) {
      IMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION( nt_headers )[ i ];

      if ( strcmp( reinterpret_cast<const char*>( section.Name ), ".text" ) ==
           0 ) {
        return section;
      }
    }

    return {};
  };

  wchar_t ignite_bin_file_path[ MAX_PATH ]{ 0 };

  // Get the ignite binary file path
  if ( !GetModuleFileName( 0, ignite_bin_file_path, MAX_PATH ) ) {
    gwingui::messagebox::Error(
        TEXT( "Unable to get the ignite binary file path" ) );
    return false;
  }

  GWIN_TRACE( "ignite_bin_file_path: %ls\n", ignite_bin_file_path );

  std::vector<uint8_t> ignite_binary_data;
  ReadFileData( ignite_bin_file_path, &ignite_binary_data );

  const auto dos_headers =
      reinterpret_cast<const IMAGE_DOS_HEADER*>( ignite_binary_data.data() );

  const auto nt_headers =
      reinterpret_cast<const IMAGE_NT_HEADERS*>( reinterpret_cast<uint8_t*>(
          ignite_binary_data.data() + dos_headers->e_lfanew ) );

  const auto text_section = get_pe_text_section( nt_headers );

  // Calculate the the text section relative virtual address (0x00410000 -
  // 0x00400000) = 0x1000
  const auto text_section_rva = text_section.VirtualAddress;

  const auto ignite_base_addr = GetModuleHandle( TEXT( "Neuz.exe" ) );

  if ( !ignite_base_addr ) {
    gwingui::messagebox::Error( TEXT( "Ignite base address is null." ) );
    return false;
  }

  // Add the relative virtual address to the dynamic ignite base address
  const auto text_section_in_memory =
      reinterpret_cast<uint32_t>( ignite_base_addr ) + text_section_rva;

  const auto text_section_size = text_section.Misc.VirtualSize;

  copied_buffer_ = new uint8_t[ text_section_size ]{ 0 };

  memcpy( copied_buffer_, ( const void* )text_section_in_memory,
          text_section_size );

  text_section_virtual_size_ = text_section_size;

  return true;
}

bool FlyffClientIgniteFlyff::Initialize() {
  return SnapshotTextSection();
}

void FlyffClientIgniteFlyff::PreAddressSearch() {
  /*
  Ignite Flyff has protection against keypresses that is not 1-9 or F-keys
  when the window is not in focus. They detect if the window is in focus or
  not by handling WM_ACTIVATE I detect when the window goes out of focus and
  then send a message with WM_ACTIVATE to say we are in focus again but in
  reality we are not.
  */

  // Do it once to avoid having window out of focus when it never gets into
  // focus
  auto botcore = Initializer().GetBotCore();
  SendMessage( botcore->GetTargetWindow(), WM_ACTIVATE, WA_ACTIVE, 0 );

  const auto target_window = botcore->GetTargetWindow();

  assert( target_window );

  uint32_t window_thread_id = GetWindowThreadProcessId( target_window, 0 );

  wnd_proc_hook_handle_ = SetWindowsHookEx( WH_CALLWNDPROC, CallWndProcHook,
                                            NULL, window_thread_id );

  if ( !wnd_proc_hook_handle_ )
    gwingui::messagebox::Error( TEXT( "failed" ) );
}

void FlyffClientIgniteFlyff::OnExit() {
  UnhookWindowsHookEx( wnd_proc_hook_handle_ );
}

std::unique_ptr<LocalPlayer> FlyffClientIgniteFlyff::CreateLocalPlayer() {
  return std::unique_ptr<LocalPlayer>( new LocalPlayerIgnite( this ) );
}

void FlyffClientIgniteFlyff::PostAddressSearch() {
  /*
    Bypass GM value 90 protection:
    Find what accesses the value (70)
    Several addresses in memory pops up, you want one of the two that occurs
    the slowest. (or once per second) It looks like = mov ecx, [esi+00000760]
    If you disassemble the function in IDA, you'll find that it sends the
    value to the server. Including the collision value???

    Bypass No Collision protection:
    Find the no collision the usual way descibed in the IMPORTANT STUFF.txt
    Note that, the no collision is sent in the same function as the gm value
    above. In other words, you can find the no collision address in the same
    function. If not, you can find what accesses the value and find the
    function that way. Also, Ignite Flyff has modified the value from 1 to
    0x00ABCDEF when enabling the no collision. This can also be seen in the
    what accessed the address.

    Since Ignite Flyff sends the value of the collision address to their
    server. There are two approaches to achieve no collision, either patch the
    function to send another value to their server. Or, change the approach to
    enable no collision. By modifying the code where the check for no
    collision occurs. https://i.imgur.com/omBvou8.png On the image, is the
    code of no collision when I find out what accessed it. The red rectangle
    is the one I should change to enable no collision.

    On the image, change the right green rectangle to
    // (cmp [Neuz.exe+something], 00ABCDEF) to cmp [Neuz.exe+something], 0
    // 81 3D 30124801 EFCDAB00 to 83 3D 30124801 00909090

    0116930D

    Modify the 00ABCDEF value to 1 and then get the collision address and
    modify that to 1 to enable and 0 to disable.


    Before
    \x81\x3D\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x57\xED\xF3
    xx????????xxxx

    After
    \x83\x3D\x00\x00\x00\x00\x00\x90\x90\x90\x0F\x57\xED\xF3
    xx?????xxxxxxx

    UPDATE: 2.2.0
    When finding what accesses the collision value, now two addresses appears
    instead of one. It is the first one that happens the most often.
  */

  // The pattern for the instruction that compares the collision with the
  // value 0x00ABCDEF
  // int addr = pattern_matcher_.FindHexPattern(
  //             "\x81\x3D\x00\x00\x00\x00\x00\x00\x00\x00\x0F\x57\xED",
  //             "xx????????xx?" );

  // if ( !IsAddressInvalid( addr ) ) {
  //  DWORD oldprot;
  //  VirtualProtect( ( LPVOID )addr, 100,
  //                  PAGE_READWRITE, &oldprot );

  //  uint8_t byte1 = 0x83;
  //  gWin::ProcessInternal().Write( addr, byte1 );

  //  uint8_t bytes[] = {
  //    0x00, 0x90, 0x90, 0x90
  //  };
  //  gWin::ProcessInternal().Write( addr + 6, bytes );

  //  VirtualProtect( ( LPVOID )addr, 100, oldprot,
  //                  &oldprot );
  //}
}

BOOL __stdcall FlyffClientIgniteFlyff::CryptHashDataHooked( HCRYPTHASH hHash,
                                                            BYTE* pbData,
                                                            DWORD dwDataLen,
                                                            DWORD dwFlags ) {
  const auto crypt_hash_data_orig =
      FlyffClientIgniteFlyff::crypt_hash_data_original_;

  GWIN_TRACE( "CryptHashDataHooked - dwDataLen: %d\n", dwDataLen );

  // If the hashed data size is the same size as the ignite text section
  // Or I could simply log the last checksum and return that in the future.
  if ( dwDataLen == FlyffClientIgniteFlyff::text_section_virtual_size_ ) {
    GWIN_TRACE( "Bypassed the Intergrity checks\n" );
    return crypt_hash_data_orig( hHash, FlyffClientIgniteFlyff::copied_buffer_,
                                 dwDataLen, dwFlags );
  } else {
    std::string ss = std::string( reinterpret_cast<const char*>( pbData ),
                                  static_cast<const size_t>( dwDataLen ) );

    std::string s = "Pw: " + ss + "\n";

    // Bypass the HDD Serial Ban
    /*
    if ( ss.find( "WD-" ) != std::wstring::npos ) {
    // strcpy((char *)(pbData + 3), "748");
    const auto length = ss.size() - 3;  // - 3 because we want to keep the
    WD- strcpy( ( char* )( pbData + 3 ),
    gWin::StringUtils::WideStringToString( GetRandomString( length )
    ) .c_str() ); std::cout << "Change the hardware: " << pbData << std::endl;
    }
    */

    return crypt_hash_data_orig( hHash, pbData, dwDataLen, dwFlags );
  }
}

void FlyffClientIgniteFlyff::PreHookInitialization() {
  // Hook CryptHashData before hooking anything else to ensure that the Ignite
  // Flyff integrity checks are bypassed before modifying anything else in the
  // memory

  crypt_hash_data_original_ = CryptHashData;

  DetourTransactionBegin();

  DetourUpdateThread( GetCurrentThread() );

  DetourAttach( reinterpret_cast<PVOID*>( &crypt_hash_data_original_ ),
                FlyffClientIgniteFlyff::CryptHashDataHooked );

  if ( DetourTransactionCommit() != NO_ERROR ) {
    gwingui::messagebox::Error( TEXT( "Failed to hook CryptHashData." ) );
    return;
  }
}

inline void FlyffClientIgniteFlyff::PostHookDeinitialization() {
  // Reset the integrity check bypass last

  DetourTransactionBegin();

  DetourUpdateThread( GetCurrentThread() );

  DetourDetach( reinterpret_cast<PVOID*>( &crypt_hash_data_original_ ),
                FlyffClientIgniteFlyff::CryptHashDataHooked );

  if ( DetourTransactionCommit() != NO_ERROR ) {
    gwingui::messagebox::Error(
        TEXT( "Error while trying to detach CryptHashData." ) );
    return;
  }
}

void FlyffClientIgniteFlyff::PostHookInitialization() {
#if 0
  if ( !IsAddressInvalid( collision_check_addr_ ) ) {
    DWORD oldprot;
    VirtualProtect( ( LPVOID )collision_check_addr_, 20, PAGE_READWRITE,
                    &oldprot );

    // This permanently modifies the instructions in the memory, therefore,
    // when you detach the bot, the integrity checks bypass gets unhooked and
    // crashes the client
    // Solution: Save the bytes and use OnExit() to restore
    // the bytes before detaching.

    // Save original bytes
    memcpy( collision_bytes_saved_, ( void* )collision_check_addr_,
            sizeof( collision_bytes_saved_ ) / sizeof( uint8_t ) );

    uint8_t byte1 = 0x83;
    gwinmem::CurrentProcess().Write( collision_check_addr_, byte1 );

    uint8_t bytes[] = { 0x00, 0x90, 0x90, 0x90 };
    gwinmem::CurrentProcess().Write( collision_check_addr_ + 6, bytes );

    VirtualProtect( ( LPVOID )collision_check_addr_, 20, oldprot, &oldprot );
  } else {
    gwingui::messagebox::Error(
        TEXT( "Incorrect collision address, unable to write any memory." ) );
  }
#endif
}

void FlyffClientIgniteFlyff::PreHookDeInitialization() {
#if 0
  // Reset the bytes for the collision bypass

  if ( !IsAddressInvalid( collision_check_addr_ ) ) {
    DWORD oldprot;
    VirtualProtect( ( LPVOID )collision_check_addr_, 20, PAGE_READWRITE,
                    &oldprot );

    // Restore original bytes
    memcpy( ( void* )collision_check_addr_, collision_bytes_saved_,
            sizeof( collision_bytes_saved_ ) / sizeof( uint8_t ) );

    VirtualProtect( ( LPVOID )collision_check_addr_, 20, oldprot, &oldprot );
  } else {
    // TODO: If the address was not found, we cannot reset the changes
    // Instead we terminate the whole client to avoid any info sent to the
    // server about us trying to modify memory due to the integrity check
    // failing.
    ExitProcess( 0 );
  }
#endif
}
