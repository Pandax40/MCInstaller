#include "ConsoleHandler.h"

ConsoleHandler::ConsoleHandler()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!isHandled)
    {
        isHandled = true;
        int left_pos = int(GetSystemMetrics(SM_CXSCREEN) * 0.16f);
        SetWindowPos(GetConsoleWindow(), NULL, left_pos, 0, 1260, 300, SWP_SHOWWINDOW);
        SetConsoleTextAttribute(hConsole, BLUE);
        std::cout << " /$$$$$$$$/$$                       /$$$$$$$$/$$                     /$$       /$$$$$$                      /$$              /$$ /$$                    " << "\n";
        std::cout << "|__  $$__/ $$                      | $$_____/__/                    | $$      |_  $$_/                     | $$             | $$| $$                    " << "\n";
        std::cout << "   | $$  | $$$$$$$   /$$$$$$       | $$      /$$ /$$$$$$$   /$$$$$$ | $$        | $$   /$$$$$$$   /$$$$$$$/$$$$$$   /$$$$$$ | $$| $$  /$$$$$$   /$$$$$$ " << "\n";
        std::cout << "   | $$  | $$__  $$ /$$__  $$      | $$$$$  | $$| $$__  $$ |____  $$| $$        | $$  | $$__  $$ /$$_____/_  $$_/  |____  $$| $$| $$ /$$__  $$ /$$__  $$" << "\n";
        std::cout << "   | $$  | $$  \\ $$| $$$$$$$$      | $$__/  | $$| $$  \\ $$  /$$$$$$$| $$        | $$  | $$  \\ $$|  $$$$$$  | $$     /$$$$$$$| $$| $$| $$$$$$$$| $$  \\__/" << "\n";
        std::cout << "   | $$  | $$  | $$| $$_____/      | $$     | $$| $$  | $$ /$$__  $$| $$        | $$  | $$  | $$ \\____  $$ | $$ /$$/$$__  $$| $$| $$| $$_____/| $$      " << "\n";
        std::cout << "   | $$  | $$  | $$|  $$$$$$$      | $$     | $$| $$  | $$|  $$$$$$$| $$       /$$$$$$| $$  | $$ /$$$$$$$/ |  $$$$/  $$$$$$$| $$| $$|  $$$$$$$| $$      " << "\n";
        std::cout << "   |__/  |__/  |__/ \\_______/      |__/     |__/|__/  |__/ \\_______/|__/      |______/|__/  |__/|_______/   \\___/  \\_______/|__/|__/ \\_______/|__/      " << "\n" << "\n";
        SetConsoleTextAttribute(hConsole, LIGHT_BLUE);
        std::cout << "                                        <|> Instalador del servidor 'The Final Frontier' creado por ";
        SetConsoleTextAttribute(hConsole, CYAN);
        std::cout << "Pandax40";
        SetConsoleTextAttribute(hConsole, LIGHT_BLUE);
        std::cout << " <|>\n\n";
        SetConsoleTextAttribute(hConsole, WHITE);
    }
    else
        delete this;
}

void ConsoleHandler::Print(std::string_view text, messageType type) const
{
    switch (type)
    {
    case ConsoleHandler::tINFO:
        std::cout << "[INFO] ";
        break;
    case ConsoleHandler::tWARN:
        SetConsoleTextAttribute(hConsole, PURPLE);
        std::cout << "[WARN] ";
        break;
    case ConsoleHandler::tERROR:
        SetConsoleTextAttribute(hConsole, RED);
        std::cout << "[ERROR] ";
        break;
    default:
        break;
    }
    std::cout << text << '\n';
    SetConsoleTextAttribute(hConsole, WHITE);
    if (type == tERROR)
        system("PAUSE");
}