#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Directory.h"
using namespace std;

class FileSystem
{
public:
  Directory* root;
  Directory* current;
  FileSystem();
  bool mkdir (string argument);
  bool cd (string argument);
  bool ls ();
  bool pwd ();
};

void FileSystem_init(FileSystem* fs);
