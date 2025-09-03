# cpu-scheduling-simulator
cpu-scheduling-simulator: A comprehensive C program that implements and simulates fundamental CPU scheduling algorithms, including FCFS, Round Robin, and Multi-Level Feedback Queue. It calculates and displays key performance metrics like waiting time, turnaround time, and throughput, and visualizes process execution with Gantt charts.

## Overview

This program simulates CPU scheduling algorithms and provides detailed analysis including:
- Gantt charts for process execution visualization
- Waiting time and turnaround time calculations
- Average performance metrics
- Throughput analysis

## Implemented Algorithms

### 1. First Come First Serve (FCFS)
- **Type**: Non-preemptive
- **Description**: Processes are executed in the order they arrive
- **Characteristics**: Simple but can lead to convoy effect

### 2. Round Robin (RR)
- **Type**: Preemptive
- **Description**: Each process gets a fixed time quantum in circular order
- **Characteristics**: Fair allocation, good for time-sharing systems
- **Parameter**: Time quantum (user-defined)

### 3. Multi-Level Feedback Queue (MLFQ)
- **Type**: Preemptive with multiple queues
- **Description**: Combines Round Robin and FCFS with priority levels
- **Structure**: 
  - Higher priority queue: Round Robin
  - Lower priority queue: FCFS
- **Behavior**: Processes start in RR queue, move to FCFS if not completed

## Program Features

### Data Structure
```c
struct Process {
    int processID;
    int arrivalTime;
    int burstTime;
    int remainingBurstTime;
    int turnaroundTime;
    int waitingTime;
    int completionTime;
    int originalArrivalTime;
    int originalBurstTime;
    int isFirstExecution;
    struct Process* next;
};
```

### Key Metrics Calculated
- **Waiting Time**: Time a process waits in the ready queue
- **Turnaround Time**: Total time from arrival to completion
- **Completion Time**: Time when process finishes execution
- **Throughput**: Number of processes completed per unit time

## How to Use

### Compilation
```bash
gcc cpu_scheduler_simulator.c -o scheduler.exe
```

### Execution
```bash
./scheduler.exe
```

### Interactive Menu
1. Choose scheduling algorithm (1-3)
2. Enter number of processes
3. For each process, enter:
   - Arrival time
   - Burst time
4. For Round Robin: Enter time quantum
5. For MLFQ: Enter time quantum for RR queue

## Sample Output

### FCFS Example
```
Gantt Chart:
| P1 from 1 to 3 | P2 from 3 to 6 | P3 from 6 to 9

+---------+--------------+------------+--------------+-----------------+-----------------+
| Process | Arrival Time | Burst Time | Waiting Time | Turnaround Time | Completion Time |
+---------+--------------+------------+--------------+-----------------+-----------------+
|   P1    |      1       |     2      |      0       |        2        |        3        |
|   P2    |      3       |     3      |      0       |        3        |        6        |
|   P3    |      3       |     3      |      3       |        6        |        9        |
+---------+--------------+------------+--------------+-----------------+-----------------+

Average Waiting Time: 1.00
Average Turnaround Time: 3.67
Throughput: 0.33 processes per unit of time
```

## Algorithm Comparison

| Algorithm | Advantage | Disadvantage | Best Use Case |
|-----------|-----------|--------------|---------------|
| FCFS | Simple, no starvation | Convoy effect, poor response time | Batch processing |
| Round Robin | Fair, good response time | Context switching overhead | Interactive systems |
| MLFQ | Adaptive priority, efficient | Complex implementation | General-purpose OS |

## Implementation Details

### Memory Management
- Dynamic allocation for linked list nodes
- Proper memory deallocation after process completion
- Memory leak prevention

### Error Handling
- Input validation for menu choices
- Buffer clearing for invalid inputs
- Graceful handling of edge cases

### Sorting and Queue Management
- Processes sorted by arrival time
- Linked list implementation for ready queues
- Efficient queue operations (enqueue/dequeue)

## Time Complexity Analysis

| Operation | FCFS | Round Robin | MLFQ |
|-----------|------|-------------|------|
| Sorting | O(n²) | O(n²) | O(n log n) |
| Scheduling | O(n) | O(n×q) | O(n×q) |
| Total | O(n²) | O(n×q) | O(n×q) |

*Where n = number of processes, q = average queue length*

## Educational Value

This simulator helps understand:
- Process scheduling concepts
- Performance trade-offs between algorithms
- Impact of time quantum on Round Robin
- Queue management in multi-level systems
- CPU utilization and system throughput

## Requirements

- **Compiler**: GCC or any C99 compatible compiler
- **OS**: Windows/Linux/macOS
- **Memory**: Minimal (dynamic allocation based on process count)

## Future Enhancements

Potential improvements could include:
- Shortest Job First (SJF) algorithm
- Priority-based scheduling
- Graphical Gantt chart visualization
- File input/output for process data
- Statistical analysis and comparison tools

## Author Notes

This implementation demonstrates fundamental concepts in operating systems and provides a practical tool for studying CPU scheduling algorithms. The code emphasizes clarity and educational value while maintaining efficiency.
