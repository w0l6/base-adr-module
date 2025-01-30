#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <fstream>

void listModules(DWORD pid) {
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!process) {
        std::cerr << "Pid pas trouvé, force" << pid << std::endl;
        return;
    }

    std::ofstream outputFile("module_adresse_base.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Impossible d'écrire dans le fichier " << std::endl;
        CloseHandle(process);
        return;
    }

    HMODULE modules[1024];
    DWORD needed;

    if (EnumProcessModules(process, modules, sizeof(modules), &needed)) {
        int count = needed / sizeof(HMODULE);
        for (int i = 0; i < count; i++) {
            char moduleName[MAX_PATH];
            MODULEINFO modInfo;
            if (GetModuleFileNameEx(process, modules[i], moduleName, sizeof(moduleName)) &&
                GetModuleInformation(process, modules[i], &modInfo, sizeof(modInfo))) {
            
                std::cout << "Module: " << moduleName << " | base adr: " << modInfo.lpBaseOfDll << std::endl;

        
                outputFile << "Module: " << moduleName << " | base adr: " << modInfo.lpBaseOfDll << std::endl;
            }
        }
    }

    outputFile.close();
    CloseHandle(process);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <PID>" << std::endl;
        return 1;
    }

    DWORD pid = std::stoi(argv[1]);
    listModules(pid);
    return 0;
}
