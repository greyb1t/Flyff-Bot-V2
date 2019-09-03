#pragma once

#include <Windows.h>

#include "flyff_client.h"

using tCryptHashData = BOOL( WINAPI* )( HCRYPTHASH hHash,
                                        const BYTE* pbData,
                                        DWORD dwDataLen,
                                        DWORD dwFlags );

namespace bot {

/*
  Ignite Flyff Notes:

  Automatically bans the account if you manually modify the selected entity
  value. The selected entity value is no longer a direct pointer to the entity,
  it is now an id that represents the entity. The id is stored in the entity
  class at offset 0x2F4.

  You can find it by selecting a player scanning for unknown value and changed
  value until you found the entity id. Afterwards, take the id and search for 4
  byte value and you'll get 3 results. Find what accessed all of the values and
  you should find the correct offset for the id in the entity class.

  Bypass Integrity Checks:
  Use CE to find what accesses any address in the memory
  That lead me to a CPGetHashParam function which is a internal function of the
  CryptGetHashParam Knowing that it uses that crypto API, I can hook
  CryptHashData to continue onwards. Look in the CryptHashDataHooked to see how
  I bypassed it.

  Bypass computer ban:
  Previous method was just changing MachineGuid, does not work anymore.
  Now they use DeviceIoControl to get the HDD SerialNumber, they have used this
  link: https://www.winsim.com/diskid32/diskid32.cpp
  They hash the hdd serial number, therefore we can hook CryptHashData and
  change it before sending it to the server

  Manual computer bypass ban:
  Run spoof.bat in the folder HDD Spoofer in this project dir through CMD with
  admin rights. ONLY ONCE (otherwise BSOD), THEN RESTART THE COMPUTER WHEN YOU
  DONT WANT ANYMORE.

  Run the command wmic path win32_physicalmedia get SerialNumber
  to see if it has changed
 */

/*
  Ignite Flyff AC Analysis:

  Stopping execution of the AC thread by hooking a function that is used inside
  of that thread. Example: CryptHashData { Sleep(1000 * 60 * 60 * 24) }

  Open packet editor and you'll notice that constant packets with size 37
  are sent.

  When blocking the AC thread, those packets stops being sent.
  Those packets are the heartbeat.

  Packet 1 (Timestamp: 11:50:56):
  23 C5 37 7E CF 18 00 00 00 74 57 4B 7C 13 13 13
  13 E7 EF EC EC 0F EA 64 EC AA EC EC EC ED EC EC
  EC DC 18 D3 3E

  Packet 2 (Timstamp: 12:50:58):
  23 C5 37 7E CF 18 00 00 00 74 57 4B 7C 13 13 13
  13 E7 EF EC EC 0D E6 64 EC AA EC EC EC ED EC EC
  EC 84 14 F2 D8

  Packet 3 (Timestamp: 12:50:59):
  23 C5 37 7E CF 18 00 00 00 74 57 4B 7C 13 13 13
  13 E7 EF EC EC 33 E2 64 EC AA EC EC EC ED EC EC
  EC 1C 82 33 95

  Packet 4 (Timestamp: 12:51:00):
  23 C5 37 7E CF 18 00 00 00 74 57 4B 7C 13 13 13
  13 E7 EF EC EC 31 FE 64 EC AA EC EC EC ED EC EC
  EC 8F 4A E4 DD

  Packet 5 (Timestamp: 12:51:01):
  23 C5 37 7E CF 18 00 00 00 74 57 4B 7C 13 13 13
  13 E7 EF EC EC 37 FA 64 EC AA EC EC EC ED EC EC
  EC ED 9C 00 82

  Packet Header (always same):
  23 C5 37 7E CF 18 00 00 00 74 57 4B 7C 13 13 13
  13 E7 EF EC EC

  After stopping the heartbeat, the client will disconnect you
  if it does not receive another heartbeat in 60 seconds.

  As long as we are sending a copy of a heartbeat that is
  intact within a 60 second interval, then we will never
  get disconnected.

  23 9B 34 9D CE 18 00 00 00 2A 54 A8 7D 13 13 13
  13 E7 EF EC EC B3 E9 49 EC AA EC EC EC ED EC EC
  EC D8 E2 76 6C

  After relog:
  23 3E 50 9A CE 18 00 00 00 8F 30 AF 7D 13 13 13
  13 E7 EF EC EC 17 34 49 EC AA EC EC EC ED EC EC
  EC 03 C8 4D D3

  The packet header seems to change after a re-log.
  Meaning that we cannot spoof with any packet.

  18 00 00 00 part of the packet seems to be the packet id

  Bypassing:
  * Hook WSASend(), look for packet of size 37 and id of 18 00 00 00
  * When the correct packet is found, save the socket handle and the packet.
  * Unhook WSASend() and start a thread that sends that packet each 30 seconds
  with the saved socket using WSASend().
  * Now the heartbeat is spoofed and you can hook any function that is used by
  the AC thread e.g. CryptHashData or Sleep.
  * Inside the hook check if the current thread ID is equals to the AC thread,
  if true, Sleep(INFINITE).
  * Boom, bypassed.
*/

class FlyffClientIgniteFlyff : public FlyffClient {
 public:
  FlyffClientIgniteFlyff();

  static LRESULT CALLBACK CallWndProcHook( int code,
                                           WPARAM wParam,
                                           LPARAM lParam );

  // Saves a buffer of the ignite .text section
  // Required to spoof the integrity checks by Ignite Flyff
  bool SnapshotTextSection();

  virtual bool Initialize();

  virtual void PreAddressSearch();

  void OnExit() override;

  virtual UniquePtrLocalPlayer CreateLocalPlayer();

  void PostAddressSearch() override;

  static BOOL WINAPI CryptHashDataHooked( HCRYPTHASH hHash,
                                          BYTE* pbData,
                                          DWORD dwDataLen,
                                          DWORD dwFlags );

  virtual void PreHookInitialization();

  void PostHookDeinitialization() override;

  virtual void PostHookInitialization();

  virtual void PreHookDeInitialization();

  ~FlyffClientIgniteFlyff() {}

 private:
  static HHOOK wnd_proc_hook_handle_;
  uint32_t collision_check_addr_;
  uint8_t collision_bytes_saved_[ 20 ];

  static tCryptHashData crypt_hash_data_original_;

  static uint8_t* copied_buffer_;
  static uint32_t text_section_virtual_size_;
};

}  // namespace bot