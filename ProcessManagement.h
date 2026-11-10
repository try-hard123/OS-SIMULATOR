#pragma once
#include <iostream>
#include <string>
#include<vector>

using namespace std;

typedef struct PCB
{
  struct PCB *left;
  int PID;
  string name;
  int BurstTime;
  int MemoryNeeded;
  string state;
  struct PCB *right;
} process;


bool ProcessManagement(string cmd, vector<string>& argument);

extern process *FRONT;
extern process *REAR;
