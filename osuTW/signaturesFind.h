#pragma once
#include <Windows.h>

uintptr_t findSig(HANDLE hProc, const unsigned char pattern[], const char* mask) {
	const size_t signature_size = strlen(mask);
	bool isFound = false;

