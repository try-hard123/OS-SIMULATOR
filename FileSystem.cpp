#include "FileSystem.h"

#include "Directory.h"
#include "File.h"

FileSystem::FileSystem() {
    root = new Directory;
    Directory_init(root);
    current = root;
}

bool FileSystem::mkdir(string argument) {
    if (haschild(argument, current)) { return false; }

    Directory* create = new Directory;
    Directory_init(create);

    create->name = argument;
    create->parent = current;

    if (current->child == nullptr) {
        current->child = create;
    } else {
        Directory* temp = current->child;

        while (temp->sibling != nullptr) { temp = temp->sibling; }

        temp->sibling = create;
    }

    return true;
}

bool FileSystem::cd(string argument) {
    Directory* temp = changedirectory(argument, current);

    if (temp->isFile) {
        cout << "bash: cd: " << argument << ": Not a directory\n";
        return true;
    }
    if (!temp) {
        cout << "bash: cd: " << argument << ": No such file or directory\n";
        return true;
    }

    current = temp;

    return true;
}

bool FileSystem::ls() {
    getlist(current);

    return true;
}

bool FileSystem::pwd() {
    getparent(current, root);
    return true;
}
