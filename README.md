# OS Simulator

A command-line operating system simulator written in C++. It runs a small shell where you can create processes, schedule them with Round Robin, manage simulated memory, and work with a virtual file system. The goal of the project is to understand how the main parts of an OS work by building them from scratch.

## Features

| Module | What it does |
|---|---|
| **Shell** | Interactive prompt (`[user@OS_SIMULATOR /]$`) that reads and runs commands |
| **Process management** | Create, list, inspect, change state of, and kill processes (stored in a doubly linked list) |
| **CPU scheduling** | Round Robin with a configurable time quantum, with turnaround, waiting, and response times |
| **Memory management** | Contiguous allocation with First Fit, block splitting, and merging of free blocks |
| **File system** | In-memory tree of folders and files (`mkdir`, `cd`, `touch`, `write`, `cat`, `rm` and more) |

## Project structure

```
OS/
├── main.cpp                 entry point
├── Shell.h / Shell.cpp      prompt and input loop
├── CommandParser.h / .cpp   splits input into command + arguments and dispatches it
├── ProcessManagement.h / .cpp   PCB (process control block) and process commands
├── Scheduler.h / .cpp       Round Robin scheduler
├── MemoryManagement.h / .cpp    First Fit memory allocator
├── FileSystem.h / .cpp      current directory handling (mkdir, cd, ls, pwd)
├── Directory.h / .cpp       tree node for folders and files
└── File.h / .cpp            file commands (touch, write, cat, rm, rmdir)
```

## Build and run

Requires a C++ compiler (g++ 7 or newer).

```bash
g++ *.cpp -o os
./os
```

Type `help` inside the simulator to see the command list, and `exit` to quit.

## Commands

### File system
| Command | Description |
|---|---|
| `mkdir <name> ...` | Create folder(s) |
| `cd <name>` / `cd ..` | Change directory |
| `ls` | List the current directory (folders are shown with a trailing `/`) |
| `pwd` | Print the current path |
| `touch <name> ...` | Create empty file(s) |
| `write <file> <text>` | Put text in a file (replaces old content) |
| `cat <file> ...` | Print file content |
| `rm <file> ...` | Delete file(s) |
| `rmdir <name> ...` | Delete empty folder(s) |

### Processes
| Command | Description |
|---|---|
| `newproc <name> <burst> <memory> [arrival]` | Create a process (arrival defaults to 0) |
| `list` / `ps` | Show all processes |
| `info <pid>` | Show details of one process |
| `state <pid> <newstate>` | Change state: `new → ready → running → terminated` |
| `kill <pid>` | Remove a process |

### Scheduling
| Command | Description |
|---|---|
| `schedule rr <quantum>` | Run Round Robin on the current processes |

### Memory
| Command | Description |
|---|---|
| `mem` | Show the memory map (blocks, owners, free space, largest hole) |
| `alloc <pid>` | Allocate the process's `MemoryNeeded` using First Fit |
| `free <pid>` | Release that process's memory |

Total simulated memory is 1024 units (`TOTAL_MEMORY` in `MemoryManagement.h`).

## How it works

**Process management.** Every process has a PCB with PID, name, burst time, memory needed, arrival time, state, and scheduling fields (`remaining`, `start`, `completion`). PCBs are kept in a doubly linked list.

**Round Robin.** Processes are sorted by arrival time and visited in a circle. A process is skipped if it is finished or has not arrived yet. Otherwise it runs for `min(quantum, remaining)`. When a process finishes, its turnaround, waiting, and response times are printed. If a full round runs nobody, the CPU is idle and time jumps to the next arrival.

```
turnaround = completion - arrival
waiting    = turnaround - burst
response   = first start - arrival
```

**Memory (First Fit).** Memory is a linked list of blocks. Allocation takes the first free block that is big enough and splits it if it is larger than needed. Freeing a block merges it with free neighbours. If an allocation fails, the simulator says whether memory is really full or whether it is **external fragmentation** (enough free memory in total, but not in one piece).

**File system.** Folders and files are nodes in one tree (`child` / `sibling` pointers). A file is a node marked `isFile` that holds text content and has no children.

## Example session

```
[nitin@OS_SIMULATOR /]$ newproc P1 5 300 0
[nitin@OS_SIMULATOR /]$ newproc P2 3 200 1
[nitin@OS_SIMULATOR /]$ newproc P3 8 300 2
[nitin@OS_SIMULATOR /]$ schedule rr 2

Execution order:
P1(0-2) P2(2-4) P3(4-6) P1(6-8) P2(8-9)
  -> PID 2 (P2) finished at 9 | turnaround = 8 | waiting = 5 | response = 1
P3(9-11) P1(11-12)
  -> PID 1 (P1) finished at 12 | turnaround = 12 | waiting = 7 | response = 0
P3(12-14) P3(14-16)
  -> PID 3 (P3) finished at 16 | turnaround = 14 | waiting = 6 | response = 2

Average turnaround time = 11.3333
Average waiting time    = 6
Average response time   = 1
```

Memory fragmentation demo (processes with memory 300, 200, 300, 400):

```
alloc 1
alloc 2
alloc 3
alloc 4      → ERROR : NOT ENOUGH MEMORY
free 2
alloc 4      → ERROR : EXTERNAL FRAGMENTATION (enough free memory, but not in one piece)
mem
```

## Limitations

- The circular order in Round Robin is fixed by arrival order, so results can differ from textbook answers when arrival times are staggered (they match when all arrivals are 0).
- Memory allocation is a separate command and is not yet linked to process states or `kill`.
- File and folder names cannot contain spaces.
- The file system is in memory only; nothing is saved when the simulator exits.

## Planned work

- [ ] Link memory to processes (allocate on `new → ready`, free on `kill` and `terminated`)
- [ ] Paging with page replacement (FIFO, LRU, Optimal)
- [ ] Process synchronization (semaphores, producer-consumer, dining philosophers)
- [ ] More schedulers: FCFS, SJF, SRTF, Priority
- [ ] Best Fit and Worst Fit memory allocation
- [ ] Deadlock detection and Banker's algorithm
- [ ] Disk scheduling (FCFS, SSTF, SCAN)