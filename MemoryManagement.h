#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define TOTAL_MEMORY 1024

typedef struct MemBlock {
    int start;
    int size;
    int pid;
    struct MemBlock* next;
} MemBlock;

bool AllocateMemory(int pid, int size);
bool FreeMemory(int pid);
void ShowMemory();
bool MemoryManagement(string cmd, vector<string>& argument);