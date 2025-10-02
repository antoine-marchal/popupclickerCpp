// popup_clicker.cpp
// Antoine Marchal (CEA 2025/09)

#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#pragma comment(lib, "user32.lib")

// Valeurs par défaut
std::wstring targetTitle = L"Warning";
std::wstring buttonText  = L"OK";

// Logger simple
void logmsg(const std::wstring &msg) {
    OutputDebugStringW(msg.c_str());
    std::wcout << msg;
}

// Parser ini ultra-simple
void loadConfig(const std::wstring &path) {
    std::wifstream file(path.c_str()); 
    if (!file.is_open()) {
        logmsg(L"[popup_clicker] No ini, using defaults\n");
        return;
    }

    std::wstring line, section;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == L';' || line[0] == L'#') continue;
        if (line.front() == L'[' && line.back() == L']') {
            section = line.substr(1, line.size() - 2);
            continue;
        }
        auto pos = line.find(L'=');
        if (pos == std::wstring::npos) continue;

        std::wstring key = line.substr(0, pos);
        std::wstring val = line.substr(pos + 1);
        // trim espaces
        while (!key.empty() && iswspace(key.front())) key.erase(key.begin());
        while (!key.empty() && iswspace(key.back())) key.pop_back();
        while (!val.empty() && iswspace(val.front())) val.erase(val.begin());
        while (!val.empty() && iswspace(val.back())) val.pop_back();

        if (section == L"popup" || section.empty()) {
            if (_wcsicmp(key.c_str(), L"targetTitle") == 0) targetTitle = val;
            if (_wcsicmp(key.c_str(), L"buttonText")  == 0) buttonText  = val;
        }
    }
    logmsg(L"[popup_clicker] Config loaded\n");
}

// Procédure WinEvent
void CALLBACK WinEventProc(HWINEVENTHOOK, DWORD, HWND hwnd,
                           LONG, LONG, DWORD, DWORD) {
    if (!hwnd) return;

    wchar_t title[256];
    GetWindowTextW(hwnd, title, 256);
    if (title[0] == 0) return;

    if (_wcsicmp(title, targetTitle.c_str()) != 0) return;

    logmsg(L"[popup_clicker] Matched window\n");

    HWND child = NULL;
    while ((child = FindWindowExW(hwnd, child, L"Button", NULL)) != NULL) {
        wchar_t bt[256];
        GetWindowTextW(child, bt, 256);
        if (bt[0] == 0) continue;

        std::wstring cleanBt = bt[0] == L'&' ? bt + 1 : bt;
        if (_wcsicmp(cleanBt.c_str(), buttonText.c_str()) == 0 ||
            _wcsicmp(bt, buttonText.c_str()) == 0) {
            SendMessageW(child, BM_CLICK, 0, 0);
            logmsg(L"[popup_clicker] BM_CLICK sent\n");
            return;
        }
    }

    PostMessageW(hwnd, WM_COMMAND, 1, 0);
    SetForegroundWindow(hwnd);
    Sleep(50);
    PostMessageW(hwnd, WM_KEYDOWN, VK_RETURN, 0);
    PostMessageW(hwnd, WM_KEYUP, VK_RETURN, 0);
    logmsg(L"[popup_clicker] ENTER sent\n");
}

int wmain() {
    std::cout << "[popup_clicker] - Antoine Marchal for CEA 2025/09\n";
    
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring iniPath = exePath;
    iniPath = iniPath.substr(0, iniPath.find_last_of(L'\\') + 1) + L"popup_clicker.ini";

    loadConfig(iniPath);

    std::wstringstream ss;
    ss << L"[popup_clicker] targetTitle=\"" << targetTitle
       << L"\" buttonText=\"" << buttonText << L"\"\n";
    logmsg(ss.str());

    SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_CREATE,
                    NULL, WinEventProc, 0, 0,
                    WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
                    NULL, WinEventProc, 0, 0,
                    WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    logmsg(L"[popup_clicker] Hook installed, waiting...\n");

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}