#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "FileSystem.h"
using namespace std;

class CommandParser {
   private:
    FileSystem* fs;

   public:
    CommandParser(FileSystem* filesystem) : fs(filesystem) {}
    bool execute(string command);
};
