#include "Directory.h"

void Directory_init(Directory* root) {
    root->name = "/";
    root->parent = nullptr;
    root->child = nullptr;
    root->sibling = nullptr;
    root->isFile = false;
    root->content = "";
}

bool haschild(const string& argument, Directory* current) {
    Directory* temp = current->child;

    while (temp != nullptr) {
        if (temp->name == argument) { return true; }
        temp = temp->sibling;
    }
    return false;
}

bool getlist(Directory* current) {
    if (current->child == nullptr) return false;

    int cnt = 0;
    for (Directory* t = current->child; t != nullptr; t = t->sibling) {
        cout << t->name ;
        if (!t->isFile) cout << "/";
        cout << "\t";
        if (++cnt == 9) {
            cnt = 0;
            cout << "\n";
        }
    }
    cout << "\n";
    return true;
}

bool getparent(Directory* current, Directory* root) {
    if (current == root) {
        cout << "/\n";
        return true;
    }
    string path;
    for (Directory* t = current; t != root; t = t->parent) path = "/" + t->name + path;
    cout << path << endl;
    return true;
}

Directory* changedirectory(const string& argument, Directory* current) {
    if (argument == "..") return current->parent ? current->parent : current;

    for (Directory* t = current->child; t != nullptr; t = t->sibling)
        if (t->name == argument) return t;

    return nullptr;
}
