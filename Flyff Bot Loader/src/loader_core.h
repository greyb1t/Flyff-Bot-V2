#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include <Windows.h>
#include <CommCtrl.h>

namespace loader {

struct Process {
  std::wstring process_name;
  uint32_t process_id;
  std::wstring window_name;
};

std::vector<Process> GetRunningProcessesByNames(
    const std::initializer_list<std::wstring> process_names );

void SuspendOrResumeProcess( const uint32_t process_id, const bool suspend );

void Inject( const uint32_t process_id );
void InjectHwidBypass( const uint32_t process_id );

}  // namespace loader