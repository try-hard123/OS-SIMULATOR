#pragma once
#include "FileSystem.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class CommandParser
{
private:
  FileSystem *fs;

public:
  CommandParser (FileSystem *filesystem) : fs (filesystem) {}
  bool execute (string command);
};
