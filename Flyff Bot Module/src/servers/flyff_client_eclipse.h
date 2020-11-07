#pragma once

#include <Windows.h>

#include "flyff_client.h"

/*
  Eclipse Flyff Analysis

  Entity List Protection:

	  Multiple lists exists, the list curerntly used changes constantly.
	  All lists lay in the CWorld structure.

	  // Esi holds the entity pointer
	  mov esi,[ebx+eax*4+18]

	  Meaning that:
	  ebx: CWorld
	  eax * 4: Some offset into the list
	  + 18: just offset by 0x18 in the CWorld structure.

	  How to get real entity list:
	  Hook 
		  add eax,ecx (which adds ecx which is a simple index for the entity in the list)
	  which is the instruction just above 
		  mov esi,[ebx+eax*4+18]

	  Now simply calculate the way they calculate.
	  ebx+eax*4+18

	  https://i.imgur.com/KIZAmU9.png
	  How to find the instructions above?
	  Simply do as usual when trying to find entity list:
	  Search a monster, hit it, de-select it, then search for new hp.
	  Then you will find the monster in memory.
	
  Selected Entity Protection:

	  The address the current selected entity is stored in changes when clicking somewhere
	  else.
	
	  Simply find the address that has the same selected entity address all the time.
	  Which is basically, one of the normal 2 addresses you can normally search for.
	
	  Same idea as above, find code that gets the correct seöected entity address. 
	  Then simply hook it and log and save it.
*/

/*
  Eclipse Flyff Bypass VEH Debugger Detection:
  Attach CE debugger on the logon screen in flyff.

  ---

  Playerbase Address: Neuz.exe+A7BB6 + 1
  A1 ? ? ? ? 75 08 3B C7

  HP offset: 0x770 (Neuz.exe+17CC58 + 2)
  8B ? ? ? ? ? 0B ? ? ? ? ? 74 40

  Position: 0x190 (Neuz.exe+1162AE + 4)
  F3 ? ? ? ? ? ? ? 8B ? ? ? ? ? 6A 01 83 EC 0C 8B CC 66 0F D6 01 89 41 08 8B CE

  scroll distance: Neuz.exe+22EBC8 + 4
  C7 05 ? ? ? ? ? ? ? ? 80 3D ? ? ? ? ? 74 55 (+2)

  Account name: neuz.exe+2116E9 + 1
  BA ? ? ? ? 8B ? ? ? ? ? 8A 01 8D 49 01

  selected entity address: neuz.exe+3AAA2 + 2
  8B ? ? ? ? ? 85 F6 74 4A 8B 56 20

  selected entiy offset (0x20): neuz.exe+3AAA3 + 2
  8B 56 20 8B C2 8B 4E 24

  // found by searching for hp of aibatt when not selected, then hitting, and searching again when not selected
  // when found, find what accesses and find the entity pointer, then find what accesses that entity pointer,
  // look back and see in the assembly that it was retrieved from [esi], then go up in that function and find what wrote that value to esi.
  // you should find something like mov esi,[ebx*4+00AAAE40]
  entity list: neuz.exe+11AEB7 + 3
  8B ? ? ? ? ? ? 85 F6 0F 84 ? ? ? ? 83 BF

  movement offset (0x38C): neuz.exe+17CC49 + 2
  8B ? ? ? ? ? F7 40 ? ? ? ? ? 75 4E

  move offset: 0x4

  model offset (0x19C): neuz.exe+11A7BD + 2
  C7 81 ? ? ? ? ? ? ? ? 8B ? ? ? ? ? 40

  level offset (0x734): neuz.exe+17D94C + 2
  FF B0 ? ? ? ? 8D 45 F0

  name offset (0x1B74): neuz.exe+A7E4C + 2
  8D ? ? ? ? ? 8D ? ? ? ? ? 8B C8 2B D0

  speed multiplier(0x8C0): neuz.exe+A04B0 + 4
  F3 ? ? ? ? ? ? ? F3 0F 59 C8 F3 ? ? ? ? ? ? ? 0F 2E

  object type(0x168): neuz.exe+119955 + 2
  8B ? ? ? ? ? 8B 4D 28
*/

namespace bot {

class FlyffClientEclipseFlyff : public FlyffClient {
 public:
  FlyffClientEclipseFlyff();
  ~FlyffClientEclipseFlyff() {}

  static LRESULT CALLBACK CallWndProcHook( int code,
                                           WPARAM wParam,
                                           LPARAM lParam );

  std::unique_ptr<LocalPlayer> CreateLocalPlayer() override;

  void PreAddressSearch();

  void OnExit() override;

  void PostAddressSearch();

 private:
  static HHOOK wnd_proc_hook_handle_;
};

}  // namespace bot