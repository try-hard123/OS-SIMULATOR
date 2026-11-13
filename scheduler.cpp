#include "scheduler.h"

#include <iostream>
#include <vector>

#include "ProcessManagement.h"

using namespace std;

void roundRobin(vector<process*>& arr, int quantum) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        arr[i]->remaining = arr[i]->BurstTime;
        arr[i]->start = -1;
        arr[i]->completion = 0;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j]->arrival > arr[j + 1]->arrival) {
                process* temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    int time = 0;
    int i = 0;
    int finished = 0;
    bool ran = false;
    double total_turn_around_time = 0, total_waiting_time = 0, total_response_time = 0;

    cout << "\nExecution order:\n";

    while (finished < n) {
        process* p = arr[i];

        if (p->remaining > 0 && p->arrival <= time) {
            ran = true;

            if (p->start == -1) p->start = time;

            int run = quantum;
            if (p->remaining < quantum) run = p->remaining;

            cout << "P" << p->PID << "(" << time << "-" << time + run << ") ";

            time = time + run;
            p->remaining = p->remaining - run;

            if (p->remaining == 0) {
                p->completion = time;
                finished++;

                int turn_around_time = p->completion - p->arrival;
                int waiting_time = turn_around_time - p->BurstTime;
                int response_time = p->start - p->arrival;

                total_turn_around_time += turn_around_time;
                total_waiting_time += waiting_time;
                total_response_time += response_time;

                cout << "\n  -> PID " << p->PID << " (" << p->name << ") finished at "
                     << p->completion << " | turnaround = " << turn_around_time
                     << " | waiting = " << waiting_time << " | response = " << response_time
                     << "\n";
            }
        }

        i = (i + 1) % n;
        if (i == 0) {
            if (ran == false && finished < n) {
                int nextArrival = -1;
                for (int k = 0; k < n; k++) {
                    if (arr[k]->remaining > 0 &&
                        (nextArrival == -1 || arr[k]->arrival < nextArrival))
                        nextArrival = arr[k]->arrival;
                }
                cout << "[idle " << time << "-" << nextArrival << "] ";
                time = nextArrival;
            }
            ran = false;
        }
    }

    cout << "\n\nAverage turnaround time = " << total_turn_around_time / n << "\n";
    cout << "Average waiting time    = " << total_waiting_time / n << "\n";
    cout << "Average response time   = " << total_response_time / n << "\n";
}

void RunScheduler(string algo, int quantum) {
    if (FRONT == nullptr) {
        cout << "ERROR : PROCESS QUEUE EMPTY\n";
        return;
    }

    vector<process*> arr;
    process* temp = FRONT;
    while (temp != nullptr) {
        arr.push_back(temp);
        temp = temp->right;
    }

    if (algo == "rr") {
        if (quantum <= 0) {
            cout << "schedule: quantum must be greater than 0\n";
            return;
        }
        roundRobin(arr, quantum);
    } else {
        cout << "schedule: unknown algorithm '" << algo << "'\n";
    }
}