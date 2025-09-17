#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMD 4096
#define MAX_FMT 1024

void replace_and_write(FILE* fp, const char* format, double elapsed, double user, double sys) {
    const char* p = format;
    while (*p) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 'e': fprintf(fp, "%.3f", elapsed); break;  // elapsed real time
                case 'U': fprintf(fp, "%.3f", user); break;     // user CPU time
                case 'S': fprintf(fp, "%.3f", sys); break;      // system CPU time
                case '%': fprintf(fp, "%%"); break;
                default: fprintf(fp, "%%%c", *p); break;
            }
            p++;
        } else {
            fputc(*p++, fp);
        }
    }
    fputc('\n', fp);
}

int main(int argc, char *argv[]) {
    int i, arg_start = 1;
    char cmdLine[MAX_CMD] = {0};
    char fmt[MAX_FMT] = "real\t%e\nuser\t%U\nsys \t%S\n";
    const char* outFile = NULL;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    FILETIME ftCreate, ftExit, ftKernel, ftUser;
    LARGE_INTEGER freq, start, end;
    __int64 tKernel = 0, tUser = 0;
    double elapsed = 0.0;
    DWORD exitCode = 0;

    // Parse options
    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outFile = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            strncpy(fmt, argv[++i], MAX_FMT - 1);
        } else {
            break;
        }
    }
    arg_start = i;

    if (arg_start >= argc) {
        printf("Usage: gtime [-o file] [-f format] <command> [args...]\n");
        return 1;
    }

    // Build command line
    for (i = arg_start; i < argc; ++i) {
        strcat(cmdLine, argv[i]);
        strcat(cmdLine, " ");
    }

    // Setup
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    if (!CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Failed to start process: %s\n", cmdLine);
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    QueryPerformanceCounter(&end);

    if (GetProcessTimes(pi.hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser)) {
        tKernel = ((__int64)ftKernel.dwHighDateTime << 32) | ftKernel.dwLowDateTime;
        tUser   = ((__int64)ftUser.dwHighDateTime   << 32) | ftUser.dwLowDateTime;
    }

    GetExitCodeProcess(pi.hProcess, &exitCode);
    elapsed = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart;

    FILE* fp = stdout;
    if (outFile) {
        fp = fopen(outFile, "w");
        if (!fp) {
            fprintf(stderr, "Error opening output file: %s\n", outFile);
            return 2;
        }
    }

    replace_and_write(fp, fmt, elapsed, tUser / 1e7, tKernel / 1e7);

    if (outFile && fp) fclose(fp);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return (int)exitCode;
}
