#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

DWORD getProcId(const wchar_t* procName) {
	DWORD procId{ 0 };
	HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);
		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					procId = procEntry.th32ProcessID;
					break;
				}

			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t findDMAAddy(HANDLE hProcess, uintptr_t pointer, std::vector<unsigned int> offsets) {
	uintptr_t addr = pointer;
	for (unsigned int i{ 0 }; i < offsets.size(); ++i) {
		ReadProcessMemory(hProcess, (BYTE*)addr, &addr, sizeof(addr), nullptr);
		addr += offsets[i];
	}
	return addr;
}

void patch(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProc) {
	DWORD oldProtect;
	VirtualProtectEx(hProc, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProc, dst, src, size, nullptr);
	VirtualProtectEx(hProc, dst, size, oldProtect, &oldProtect);
}

void nop(BYTE* dst, unsigned int size, HANDLE hProc) {
	BYTE* nopArr{ new BYTE[size] };
	memset(nopArr, 0x90, size);
	patch(dst, nopArr, size, hProc);
	delete[] nopArr;
}

template <typename T>
bool writeMem(unintptr_t address, T value, HANDLE hProcess) {
	return WriteProcessMemory(hProcess, (LPVOID)(address), &value, sizeof(value), nullptr);
}

template <typename N>
N readMem(uintptr_t address, HANDLE hProcess) {
	N value{};
	ReadProcessMemory(hProcess, (LPVOID)(address), &value, sizeof(value), nullptr);
	return value;
}