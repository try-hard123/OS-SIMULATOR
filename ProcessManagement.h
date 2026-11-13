#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct PCB {
    struct PCB* left;
    int PID;
    string name;
    int MemoryNeeded;
    string state;
    int BurstTime;
    int arrival;
    int start;
    int remaining;
    int completion;
    struct PCB* right;
} process;

bool ProcessManagement(string cmd, vector<string>& argument);

extern process* FRONT;
extern process* REAR;
