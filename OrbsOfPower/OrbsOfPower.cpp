#include <Windows.h>
#include <iostream>
#include <fstream>
#include "windivert.h"

HANDLE handle;
bool allow = true;

void flt() {
    std::cout << "### STARTED ###" << std::endl;
    WINDIVERT_ADDRESS addr;
    char* packet = (char*)malloc(WINDIVERT_MTU_MAX);
    if (packet == NULL) {
        std::cout << "Failed to allocate buffer" << std::endl;
        exit(0);
    }
    unsigned int packetLen;
    while (true) {
        WinDivertRecv(handle, packet, WINDIVERT_MTU_MAX, &packetLen, &addr);
        if (!allow) {
            std::cout << "### TRIGGERED ###" << std::endl;
            continue;
        }
        WinDivertSend(handle, packet, packetLen, NULL, &addr);
    }
}

int main(int argc, char** argv) {
    int threads = 0;
    int mode = 0;
    int delay = 0;
    int key = 0;
    std::string filter;
    if (argc > 5) {
        threads = atoi(argv[1]);
        mode = atoi(argv[2]);
        delay = atoi(argv[3]);
        key = atoi(argv[4]);
        filter = std::string(argv[5]);
    }
    else {
        std::cout << "USE: OrbsOfPower.exe [threads int] [mode 0|1] [delay int] [key vk_num] [filter \"string\"]" << std::endl;
        std::cout << "### MANUAL SETUP ###" << std::endl;
        std::cout << "### ENTER THREADS ###" << std::endl;
        std::cin >> threads;
        std::cout << "### ENTER MODE ###" << std::endl;
        std::cout << "### 0 - AUTO (AUTO DELAY AFTER BUTTON PRESS) ###" << std::endl;
        std::cout << "### 1 - MANUAL (HOLD DOWN=LAG) ###" << std::endl;
        std::cin >> mode;
        if (mode == 0) {
            std::cout << "### ENTER DELAY ###" << std::endl;
            std::cin >> delay;
            key = 0x46;
        }
        else if (mode == 1) {
            std::cout << "### ENTER KEYBIND ###" << std::endl;
            char t;
            std::cin >> t;
            key = VkKeyScanExA(t, LoadKeyboardLayoutA("00000409", 0x00000001));
        }
        std::cout << "### ENTER FILTER ###" << std::endl;
        std::cout << "udp.SrcPort == 3074" << std::endl;
        std::cout << "udp.SrcPort >= 27000 and udp.SrcPort <= 27100" << std::endl;
        std::cin >> filter;
    };
    std::cout << "THREADS SET " << threads << std::endl;
    std::cout << "MODE SET " << mode << std::endl;
    std::cout << "DELAY SET " << delay << std::endl;
    std::cout << "KEYBIND SET(INT,HEX) " << key << ",0x" << std::hex << key << std::endl;
    std::cout << "FILTER SET " << filter << std::endl;
    handle = WinDivertOpen(filter.c_str(), WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        std::cout << "Couldn't open handle" << std::endl;
        exit(0);
    }
    for (int i = 0; i < threads; i++) {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)flt, NULL, 0, NULL);
    }
    if (mode == 0) {
        while (true) {
            if (GetAsyncKeyState(key)) {
                Sleep(delay);
                allow = false;
                Sleep(3000);
                allow = true;
            }
        }
    }
    else if (mode == 1) {
        while (true) {
            if (GetAsyncKeyState(key)) {
                allow = false;
            }
            else {
                allow = true;
            }
        }
    }
}
