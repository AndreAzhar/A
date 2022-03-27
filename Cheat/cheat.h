#pragma once
#include <cstdint>
#include <winnt.h>
#include <string>



namespace CMain
{
	int cheatjit();
	uintptr_t GetProcessBaseAddress(HANDLE Handle);
	std::uint32_t find_process_by_id(const std::string& name);

}