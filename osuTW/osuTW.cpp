





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

}