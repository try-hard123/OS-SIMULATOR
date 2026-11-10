#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef struct Directory{
  string name;
  Directory* parent;
  Directory* child;
  Directory* sibling;
}Directory;

void Directory_init(Directory* root);
bool haschild(const string& argument, Directory* root);
bool getlist(Directory* root);
bool getparent(Directory* current, Directory* root);
Directory* changedirectory(const string& argument, Directory* root);
