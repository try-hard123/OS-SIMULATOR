#include "ProcessManagement.h"

#include "scheduler.h"
static int PID = 0;

process* FRONT = nullptr;
process* REAR = nullptr;

typedef enum { PROCESS_CREATION_OK, PROCESS_CREATION_ERROR } process_result;

void helper() {
    cout << "COMMANDS:\n"
         << "newproc <name> <burst> <memory> [arrival] [priority]\n"
         << "list/ps \n"
         << "kill <pid>\n"
         << "info <pid>\n"
         << "state <pid> <newstate>\n";
}

process_result CreateProcess(string name, int BurstTime, int MemoryNeeded, int arrival) {
    process* createprocess = new process;

    if (createprocess == nullptr) {
        cout << "MemoryAllocation Failed" << endl;
        return PROCESS_CREATION_ERROR;
    }

    createprocess->left = nullptr;
    createprocess->PID = ++PID;
    createprocess->name = name;
    createprocess->BurstTime = BurstTime;
    createprocess->remaining = BurstTime;
    createprocess->MemoryNeeded = MemoryNeeded;
    createprocess->arrival = arrival;
    createprocess->state = "new";
    createprocess->start = -1;
    createprocess->completion = 0;
    createprocess->right = nullptr;

    if (FRONT == nullptr) {
        FRONT = REAR = createprocess;
    }

    else {
        REAR->right = createprocess;
        createprocess->left = REAR;
        REAR = createprocess;
    }

    return PROCESS_CREATION_OK;
}

void DisplayProcess() {
    process* temp = FRONT;

    if (temp == nullptr) {
        cout << "ERROR : PROCESS QUEUE EMPTY\n";
        return;
    }

    while (temp) {
        cout << "--------------------" << endl;
        cout << "=> PID: " << temp->PID << endl;
        cout << "=> Name: " << temp->name << endl;
        cout << "=> BurstTime: " << temp->BurstTime << endl;
        cout << "=> Memory: " << temp->MemoryNeeded << endl;
        cout << "=> Arrival: " << temp->arrival << endl;
        cout << "=> State: " << temp->state << endl;
        cout << "--------------------" << endl;

        temp = temp->right;
    }

    return;
}

void kill(int PID) {
    process* temp = FRONT;

    if (temp == nullptr) {
        cout << "ERROR : PROCESS QUEUE IS EMPTY\n";
        return;
    }

    while (temp) {
        if (temp->PID == PID) {
            if (temp == FRONT)
                FRONT = temp->right;
            else
                temp->left->right = temp->right;

            if (temp == REAR)
                REAR = temp->left;
            else
                temp->right->left = temp->left;

            cout << "=>PID: " << temp->PID << " Deletion Successful" << endl;

            delete temp;
            return;
        }

        temp = temp->right;
    }

    cout << "NO PROCESS FOUND OF PID:" << PID << endl;
    return;
}

void ShowInfo(int PID) {
    process* temp = FRONT;

    if (temp == nullptr) {
        cout << "ERROR : PROCESS QUEUE EMPTY\n";
        return;
    }

    while (temp) {
        if (temp->PID == PID) {
            cout << "--------------------" << endl;
            cout << "Process Name:" << temp->name << endl;
            cout << "Burst Time:" << temp->BurstTime << endl;
            cout << "Memory:" << temp->MemoryNeeded << endl;
            cout << "=> Arrival: " << temp->arrival << endl;
            cout << "--------------------" << endl;
            return;
        }

        temp = temp->right;
    }

    cout << "NO PROCESS FOUND OF PID:" << PID << endl;

    return;
}

void ChangeState(int PID, string NewState) {
    if (NewState != "new" && NewState != "ready" && NewState != "running" &&
        NewState != "terminated") {
        cout << "Invalid argument\t" << "Avilable States <new> <ready> <running> <terminated>"
             << endl;
        return;
    }

    process* temp = FRONT;

    if (temp == nullptr) {
        cout << "PROCESS QUEUE EMPTY\n";
        return;
    }

    while (temp) {
        if (temp->PID == PID) {
            if (temp->state == "new" && NewState != "ready" ||
                temp->state == "ready" && NewState != "running" ||
                temp->state == "running" && NewState != "terminated") {
                cout << "Invalid state transition! ";
                cout << "new -> ready -> running -> terminated\n";
                return;
            }
            temp->state = NewState;
            return;
        }

        temp = temp->right;
    }

    cout << "NO PROCESS FOUND OF PID:" << PID << endl;

    return;
}

bool ProcessManagement(string cmd, vector<string>& argument) {
    if (cmd == "newproc") {
        if (argument.size() < 3 || argument.size() > 4) {
            cout << "newproc: usage: newproc [name] [burst] [memory] [arrival]\n";
            return true;
        }

        string name = argument[0];
        int BurstTime, MemoryNeeded, arrival = 0;

        try {
            BurstTime = stoi(argument[1]);
            MemoryNeeded = stoi(argument[2]);
            if (argument.size() == 4) arrival = stoi(argument[3]);
        } catch (...) {
            cout << "newproc: arguments must be numbers\n";
            return true;
        }

        if (BurstTime <= 0 || arrival < 0) {
            cout << "newproc: burst must be > 0 and arrival must be >= 0\n";
            return true;
        }

        CreateProcess(name, BurstTime, MemoryNeeded, arrival);
        return true;
    }

    else if (cmd == "list" || cmd == "ps") {
        DisplayProcess();
        return true;
    }

    else if (cmd == "kill") {
        if (argument.size() != 1) {
            cout << "kill: usage: kill [PID]\n";
            return true;
        }

        int PID = stoi(argument[0]);

        kill(PID);

        return true;
    }

    else if (cmd == "info") {
        if (argument.size() != 1) {
            cout << "info: usage: info [PID]\n";
            return true;
        }

        int PID = stoi(argument[0]);

        ShowInfo(PID);

        return true;
    }

    else if (cmd == "state") {
        if (argument.size() != 2) {
            cout << "state: usage: state [PID] [NewState]\n";
            return true;
        }

        int PID = stoi(argument[0]);
        string NewState = argument[1];

        ChangeState(PID, NewState);

        return true;
    } else if (cmd == "schedule") {
        if (argument.size() != 2) {
            cout << "schedule: usage: schedule rr [quantum]\n";
            return true;
        }

        int quantum;
        try {
            quantum = stoi(argument[1]);
        } catch (...) {
            cout << "schedule: invalid quantum\n";
            return true;
        }

        RunScheduler(argument[0], quantum);
        return true;
    } else {
        cout << "Invalid Command" << endl;
        helper();

        return true;
    }
}
