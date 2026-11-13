#include "MemoryManagement.h"

#include "ProcessManagement.h"

static MemBlock* head = nullptr;

static void MemoryInit() {
    head = new MemBlock;
    head->start = 0;
    head->size = TOTAL_MEMORY;
    head->pid = 0;
    head->next = nullptr;
}

static bool HasMemory(int pid) {
    MemBlock* temp = head;

    while (temp) {
        if (temp->pid == pid) return true;
        temp = temp->next;
    }
    return false;
}

static process* FindProcess(int pid) {
    process* temp = FRONT;

    while (temp) {
        if (temp->PID == pid) return temp;
        temp = temp->right;
    }
    return nullptr;
}

bool AllocateMemory(int pid, int size) {
    if (head == nullptr) MemoryInit();

    if (size <= 0) {
        cout << "ERROR : INVALID MEMORY SIZE\n";
        return false;
    }

    if (HasMemory(pid)) {
        cout << "ERROR : PID " << pid << " ALREADY HAS MEMORY\n";
        return false;
    }

    int totalFree = 0;
    MemBlock* temp = head;

    while (temp) {
        if (temp->pid == 0) {
            totalFree = totalFree + temp->size;

            if (temp->size >= size) {
                if (temp->size > size) {
                    MemBlock* rest = new MemBlock;
                    rest->start = temp->start + size;
                    rest->size = temp->size - size;
                    rest->pid = 0;
                    rest->next = temp->next;

                    temp->next = rest;
                    temp->size = size;
                }

                temp->pid = pid;
                return true;
            }
        }
        temp = temp->next;
    }

    if (totalFree >= size)
        cout << "ERROR : EXTERNAL FRAGMENTATION (enough free memory, but not in one piece)\n";
    else
        cout << "ERROR : NOT ENOUGH MEMORY\n";

    return false;
}

bool FreeMemory(int pid) {
    if (head == nullptr) MemoryInit();

    MemBlock* temp = head;

    while (temp) {
        if (temp->pid == pid) break;
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << "ERROR : PID " << pid << " HAS NO MEMORY\n";
        return false;
    }

    temp->pid = 0;

    temp = head;
    while (temp && temp->next) {
        if (temp->pid == 0 && temp->next->pid == 0) {
            MemBlock* del = temp->next;
            temp->size = temp->size + del->size;
            temp->next = del->next;
            delete del;
        } else
            temp = temp->next;
    }

    return true;
}

void ShowMemory() {
    if (head == nullptr) MemoryInit();

    int used = 0, freeMem = 0, largestHole = 0;
    MemBlock* temp = head;

    cout << "--------------------------------------\n";
    cout << "START\tEND\tSIZE\tOWNER\n";
    cout << "--------------------------------------\n";

    while (temp) {
        cout << temp->start << "\t" << temp->start + temp->size - 1 << "\t" << temp->size << "\t";

        if (temp->pid == 0) {
            cout << "FREE\n";
            freeMem = freeMem + temp->size;
            if (temp->size > largestHole) largestHole = temp->size;
        } else {
            cout << "P" << temp->pid << "\n";
            used = used + temp->size;
        }
        temp = temp->next;
    }

    cout << "--------------------------------------\n";
    cout << "Total: " << TOTAL_MEMORY << "  Used: " << used << "  Free: " << freeMem
         << "  Largest hole: " << largestHole << "\n";
}

bool MemoryManagement(string cmd, vector<string>& argument) {
    if (cmd == "mem") {
        ShowMemory();
        return true;
    }

    else if (cmd == "alloc") {
        if (argument.size() != 1) {
            cout << "alloc: usage: alloc [PID]\n";
            return true;
        }

        int pid;
        try {
            pid = stoi(argument[0]);
        } catch (...) {
            cout << "alloc: PID must be a number\n";
            return true;
        }

        process* p = FindProcess(pid);
        if (p == nullptr) {
            cout << "NO PROCESS FOUND OF PID:" << pid << endl;
            return true;
        }

        if (AllocateMemory(pid, p->MemoryNeeded))
            cout << "=> " << p->MemoryNeeded << " allocated to PID " << pid << endl;

        return true;
    }

    else if (cmd == "free") {
        if (argument.size() != 1) {
            cout << "free: usage: free [PID]\n";
            return true;
        }

        int pid;
        try {
            pid = stoi(argument[0]);
        } catch (...) {
            cout << "free: PID must be a number\n";
            return true;
        }

        if (FreeMemory(pid)) cout << "=> memory of PID " << pid << " freed\n";

        return true;
    }

    return false;
}