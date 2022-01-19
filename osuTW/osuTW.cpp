





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
    
>

}