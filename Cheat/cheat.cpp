#pragma once


#include "../globals.h"
#include "../ConsoleColor.h"

#include <iostream>
#include <Psapi.h>
#include <string>
#include <thread>
#include <TlHelp32.h>
#include "../Game/game.h"
#include "../Game/game.cpp"

using namespace std;

std::uint32_t find_process_by_id(const std::string& name)
{
	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 proc_entry{};
	proc_entry.dwSize = sizeof proc_entry;

	auto found_process = false;
	if (!!Process32First(snap, &proc_entry)) {
		do {
			if (name == proc_entry.szExeFile) {
				found_process = true;
				break;
			}
		} while (!!Process32Next(snap, &proc_entry));
	}

	CloseHandle(snap);
	return found_process
		? proc_entry.th32ProcessID
		: 0;
}

uintptr_t GetProcessBaseAddress(HANDLE Handle)
{
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(Handle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(Handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				std::string wstrModName = szModName;
				std::string wstrModContain = "RainbowSix.exe";
				if (wstrModName.find(wstrModContain) != std::string::npos)
				{
					base_address = (uintptr_t)hMods[i];
					return base_address;
				}
			}
		}
	}

	return false;
}



int cheatjit()
{
	SetConsoleTitle("Possible Paste v2 ★");

	{
		pid = find_process_by_id("RainbowSix.exe");
		if (!pid)
		{
			MessageBox(NULL, "Error, Code: 1", NULL, MB_ICONHAND);
			exit(-1);
			return pid;
		}

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
		if (hProcess == INVALID_HANDLE_VALUE || !hProcess)
		{
			MessageBox(NULL, "Error, Code: 0", NULL, MB_ICONHAND);
			exit(-1);

		}

		base_address = GetProcessBaseAddress(hProcess);
		if (!base_address)
		{
			MessageBox(NULL, "Error, Code: 2", NULL, MB_ICONHAND);
			exit(-1);
			return base_address;
		}
	}
	
	RainbowSix::ns();
	RainbowSix::AmbientColor();
	RainbowSix::NoSky();


	while (true)
		{
			for (int i = 0; i < RainbowSix::ControllerCount(RainbowSix::GetGameManager()); i++)
			{
				const auto PlayerController = RainbowSix::CPlayerController{ RPM<std::uintptr_t>(RainbowSix::ControllerList(RainbowSix::GetGameManager()) + (i * sizeof(std::uintptr_t))) };
				if (!PlayerController.Address)
					continue;




				const auto PawnComponent = RainbowSix::CPlayerController::CPawnComponent{ _rotl64(RPM<std::uintptr_t>(PlayerController.Address + 0x38) - 0x211C0F86ED634100i64, 0x3B) - 0x2F };
				if (!PawnComponent.Address)
					continue;

				const auto Actor = RainbowSix::CPlayerController::CPawnComponent::CActor{ (_rotl64(RPM<std::uintptr_t>(PawnComponent.Address + 0x18), 0x1A) ^ 0xBCC79864C12C2B1Fui64) - 0x5C };
				if (!Actor.Address)
					continue;

				RainbowSix::OutlineActor(Actor);
			}

		}


	



	return 0;
}
