#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Directory.h"

using namespace std;

bool TouchFile(Directory* current, const string& name);
bool WriteToFile(Directory* current, const string& name, const string& text);
bool CatFile(Directory* current, const string& name);
bool RemoveFile(Directory* current, const string& name);
bool RemoveDir(Directory* current, const string& name);

bool FileCommands(const string& cmd, vector<string>& argument, Directory* current);