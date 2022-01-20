





double TWRate { 1 };
void DrawChangeMenu(bool isHD, bool isFL, bool isTW, double rate) {
    system("CLS");
    std::cout << "osu tw and hdfl remover\n\n";
    std::string hd = (isHD) ? "Remove HD [o]\n" : "Remove HD []\n"
    std::cout << hd;
    std::string fl = (isFL) ? "Remove FL [o]\n" : "Remove FL []\n";
    std::cout << fl;
    if (isTW) std::cout << "Timewarp ENABLED [" << rate << "x]\n\n";
    else std::cout << "Timewarp DISABLED [" << rate << "x]\n\n";
    std::cout << "NUMPAD1 - DISABLE HD\nNUMPAD2 - ENABLE HD\nNUMPAD3 - DISABLE FL\nNUMPAD4 - ENABLE FL\nNUMPAD5 - ENABLE TW\nNUMPAD6 - DISABLE TW\nNUMPAD9 - PANIC KEY\n";
}


int main()
{

    bool isHD{ false }, isFL{ false }, isTW{ false }, bLoaded{ false }, bAsync{ false };
    
    int currentMods{};
    SetConsoleTitle(L"OSU EASY GAME");
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 14);
    HANDLE hProc{ 0 };
    uintptr_t bassBase{};
    DWORD procId { getProcID(L"osu!.exe") };
    
    if (procID) {
        bassBase =GetModuleBaseAddress(procId, L"bass.dll");
       hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    }
    else {
        system("CLS");
        std::cout << "open osu! first!\n";
        getchar();
        return 0;
    }
    std::cout << "Initializing Data...\n";
    uintptr_t flAddress{}, hdApproach{}, hdVisible{}, playerOffsets{};
    
    flAddress= findSig(hProc, sigs::loadFlashlight, "xxxxxxxxxxxxx?x????xxx?xxxx?xx?xx??x?x??x");
    playerOffsets= findSig(hProc, sigs::playerOffsets, "xxxxxxx") + 0x7;
    uintptr_t playerPtr1 = findDMAAddy(hProc, playerOffsets, { 0x0 });
    hdApproach=findSig(hProc, sigs::hdApproach, "xx??x?xx");
    hdVisible=findSig(hProc, sigs::hdVisible, "x?x????x????xxxx");
    
    /*twAddr = findSig(hProc, sigs::twFunc, "xxxxxx????xxx?xxx??xx");
        typedef void(__thiscall* _timewarp)(double value);
        _timewarp timewarp;
        timewarp = (_timewarp)(twAddr);*/
        
    system("CLS");
    std::cout << "osu tw and hdfl remover\n\n";
    std::cout << "Remove HD []\nRemove FL []\nTimewarp DISABLED ["<<TWRate<<"x]\n\n";
    std::cout << "NUMPAD1 - DISABLE HD\nNUMPAD2 - ENABLE HD\nNUMPAD3 - DISABLE FL\nNUMPAD4 - ENABLE FL\nNUMPAD5 - ENABLE TW\nNUMPAD6 - DISABLE TW\nNUMPAD9 - PANIC KEY\n";
    while (true) {
        uintptr_t playerPtr = findDMAAddy(hProc, playerPtr1, { 0x0 });
                uintptr_t audioBypass = playerPtr + 0x14C;
                uintptr_t async = playerPtr + 0x182;
                uintptr_t TWaddr = findDMAAddy(hProc, bassBase+ 0x00034268, { 0x8,0x10,0xC,0x40 });
                ReadProcessMemory(hProc, (LPVOID)(async), &bAsync, sizeof(bAsync), nullptr);
                
                if (playerPtr != NULL && bAsync) bLoaded = true;
                else bLoaded = false;
                
                writeMem(audioBypass, 0, hProc);
                
                if (isTW && bLoaded) {
                    writeMem(TWaddr, TWRate, hProc);
                    writeMem(TWaddr+0x8, 1147.0 * TWRate, hProc);
                }
                if (!bLoaded) {
                    writeMem(TWaddr, 1.0, hProc);
                    writeMem(TWaddr + 0x8, 1147.0, hProc);
                }
                
                if (GetAsyncKeyState(VK_NUMPAD1) & 1) { //disable hd
                    nop((BYTE*)hdVisible, 2, hProc);
                    nop((BYTE*)(hdApproach+0x4), 2, hProc); 
                    isHD = true;
                    DrawChangedMenu(isHD, isFL, isTW, TWRate);
                }
                if (GetAsyncKeyState(VK_NUMPAD2) & 1) { //enable hd
                            patch((BYTE*)hdVisible, (BYTE*)"\x75\x5F", 2, hProc);
                            patch((BYTE*)(hdApproach + 0x4), (BYTE*)"\x74\x10",2, hProc);
                            isHD = false;
                            DrawChangedMenu(isHD, isFL, isTW, TWRate);
                }
    }
    

}