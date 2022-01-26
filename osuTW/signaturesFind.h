#pragma once
#include <Windows.h>

uintptr_t findSig(HANDLE hProc, const unsigned char pattern[], const char* mask) {
	const size_t signature_size = strlen(mask);
	bool isFound = false;

	unsigned char chunk[4096]{};

	for (size_t i = 0x04000000; i < 0x7F000000; i += 4096 - signature_size) {
		ReadProcessMemory(hProc, LPCVOID(i), &chunk, 4096, nullptr);

		for (size_t a = 0; a < 4096; a++) {
			isFound = true;

