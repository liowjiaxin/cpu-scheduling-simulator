
#include <stdio.h>
#include <stdlib.h>

// Structure to represent a process
struct Process {
    int processID;
    int arrivalTime;
    int burstTime;
    int remainingBurstTime;
    int turnaroundTime;
    int waitingTime;
    int completionTime; // Added field for completion time
    int originalArrivalTime; // New field for original arrival time
    int originalBurstTime;   // New field for original burst time
    int isFirstExecution; // 1 if the process has not been executed yet, 0 otherwise
    struct Process* next;
};

// Function prototypes
struct Process* createProcess(int id);
struct Process* addProcessByArrival(struct Process* head, int id, int burstTime, int arrivalTime, int originalArrivalTime, int originalBurstTime);
struct Process* removeFront(struct Process** head);
struct Process* rotateList(struct Process* head);
void fcfsScheduler(struct Process* processes, int n);
void roundRobinScheduler(struct Process* processes, int n, int timeQuantum);
void multiLevelFeedbackQueue(struct Process processes[], int n);
void calculateAverageTimesMLFQ(struct Process processes[], int n);
float calculateThroughput(int n, int totalTime);
void printGanttChart(int startTime, int endTime, int processID, const char* queueType);
void printGanttChartMLFQ(int startTime, int endTime, int processID, const char* queueType, const char* executionType);
void printTableHeader();
void printTableRow(int processID, int arrivalTime, int burstTime, int waitingTime, int turnaroundTime,int completionTime);
void printTableFooter();

int main() {
    int choice;
    float throughput;

    do {
        printf(" \n==== Choose a scheduling algorithm: =====\n");
        printf("1. First Come First Serve\n");
        printf("2. Round Robin\n");
        printf("3. Multi-Level Feedback Queue\n");
        printf("0. Exit\n");
        printf("\nEnter your choice: ");

        // Check if the input is an integer
        if (scanf("%d", &choice) != 1 || (choice < 0 || choice > 3)) {
            // If the input is not a valid integer or is outside the valid range, clear the input buffer
            while (getchar() != '\n');

            printf("\nInvalid choice. Please enter a valid option (0, 1, 2, or 3).\n");
            continue; // Go back to the beginning of the loop
        }

        if (choice == 0) {
            printf("\n ==== Exiting program. ====\n");
            return 0;
        }

        int n, burstTime, arrivalTime;

        printf("\nEnter the number of processes: ");
        scanf("%d", &n);

        struct Process processes[n];
        for (int i = 0; i < n; i++) {
            processes[i] = *createProcess(i + 1);

            // get user inputs for burst time and arrival time
            printf("\nEnter the arrival time for process %d: ", i + 1);
            scanf("%d", &arrivalTime);
            processes[i].arrivalTime = arrivalTime;
            processes[i].originalArrivalTime = arrivalTime;

            printf("Enter the burst time for process %d: ", i + 1);
            scanf("%d", &burstTime);
            processes[i].burstTime = burstTime;
            processes[i].remainingBurstTime = burstTime; // Set remaining burst time
            processes[i].originalBurstTime = burstTime;
        }

        switch (choice) {
            case 1:
                // FCFS Scheduler
                printf("\n # Results (First Come First Serve Scheduler): # \n");
                fcfsScheduler(processes, n);
                throughput = calculateThroughput(n, processes[n - 1].completionTime);
                break;

            case 2:
                // Round Robin Scheduler
                {
                    int timeQuantum;
                    printf("\nEnter the time quantum: ");
                    scanf("%d", &timeQuantum);
                    printf("\n # Results (Round Robin Scheduler): #\n");
                    roundRobinScheduler(processes, n, timeQuantum);
                    throughput = calculateThroughput(n, processes[n - 1].completionTime);
                }
                break;

            case 3:
                //Multi-Level Feedback Queue
                multiLevelFeedbackQueue(processes, n);
                //printf("\nResults (Multi-Level Feedback Queue):\n");
                calculateAverageTimesMLFQ(processes, n);
                throughput = calculateThroughput(n, processes[n - 1].completionTime);
                break;

            default:
                printf("Invalid choice.\n");
                return 1;
        }

        // Print throughput
        printf("\nThroughput: %.2f processes per unit of time.\n", throughput);

    } while (1); // Infinite loop until user chooses to exit

    return 0;
}

// Add the FCFS scheduler function (similar to the roundRobinScheduler function)
void fcfsScheduler(struct Process* processes, int n) {
    // Sorting processes based on arrival time (unchanged)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < (n - i - 1); j++) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                // Swap processes
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Calculating completion time (unchanged)
    processes[0].completionTime = processes[0].arrivalTime + processes[0].burstTime;

    for (int i = 1; i < n; i++) {
        if (processes[i - 1].completionTime < processes[i].arrivalTime) {
            processes[i].completionTime = processes[i].arrivalTime + processes[i].burstTime;
        } else {
            processes[i].completionTime = processes[i - 1].completionTime + processes[i].burstTime;
        }
    }

    // Calculating turnaround time and waiting time (unchanged)
    for (int i = 0; i < n; i++) {
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].originalBurstTime;
    }

    // Printing Gantt chart
    //Completion time - burst time = the time process starts executed
    printf("\n Gantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printGanttChart(processes[i].completionTime - processes[i].burstTime, processes[i].completionTime, processes[i].processID,"");
    }

    // Display the results
    printf("\n\n");
    printTableHeader();
    for (int i = 0; i < n; i++) {
        printTableRow(processes[i].processID,
                      processes[i].originalArrivalTime,
                      processes[i].originalBurstTime,
                      processes[i].waitingTime,
                      processes[i].turnaroundTime,
                      processes[i].completionTime);
    }
    printTableFooter();



    // Calculate and print average waiting time and turnaround time
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    float avgTurnaroundTime = (float)totalTurnaroundTime / n;

    printf("\nAverage Waiting Time: %.2f\n", avgWaitingTime);
    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime);
}
// Function to create a process
struct Process* createProcess(int id) {
    struct Process* newProcess = (struct Process*)malloc(sizeof(struct Process));
    newProcess->processID = id;
    newProcess->arrivalTime = 0;  // You can modify this based on your requirements
    newProcess->burstTime = 0;    // You can modify this based on your requirements
    newProcess->remainingBurstTime = newProcess->burstTime;
    newProcess->turnaroundTime = 0;
    newProcess->waitingTime = 0;
    newProcess->next = NULL;
    newProcess->originalArrivalTime = 0;  // Initialize with default value
    newProcess->originalBurstTime = 0;    // Initialize with default value

    return newProcess;
}

// Function to add a process to the linked list based on arrival time
struct Process* addProcessByArrival(struct Process* head, int id, int burstTime, int arrivalTime, int originalArrivalTime, int originalBurstTime) {
    // Create a new process
    struct Process* newProcess = (struct Process*)malloc(sizeof(struct Process));
    if (newProcess == NULL) {
        // Handle memory allocation failure
        return head;
    }

    newProcess->processID = id;
    newProcess->arrivalTime = arrivalTime;
    newProcess->burstTime = burstTime;
    newProcess->remainingBurstTime = burstTime;
    newProcess->turnaroundTime = 0;
    newProcess->waitingTime = 0;
    newProcess->originalArrivalTime = originalArrivalTime;
    newProcess->originalBurstTime = originalBurstTime;
    newProcess->next = NULL;

    // If the list is empty or the new process arrives before the current head, make it the new head
    if (head == NULL || arrivalTime < head->arrivalTime) {
        newProcess->next = head;
        return newProcess;
    }

    // Otherwise, find the appropriate position in the linked list to insert the new process
    struct Process* current = head;
    while (current->next != NULL && current->next->arrivalTime <= arrivalTime) {
        current = current->next;
    }

    newProcess->next = current->next;
    current->next = newProcess;

    return head;
}

struct Process* removeFront(struct Process** head) {
    if (*head == NULL) {
        return NULL;  // If the list is empty, nothing to remove
    }

    struct Process* front = *head;
    *head = (*head)->next;
    front->next = NULL;  // Disconnect the removed node from the list

    return front;
}

// Function to rotate the linked list
struct Process* rotateList(struct Process* head) {
    if (head == NULL || head->next == NULL) {
        return head; // No rotation needed if the list is empty or has only one element
    }

    struct Process* newHead = head->next;
    struct Process* tail = head;

    while (tail->next != NULL) {
        tail = tail->next;
    }

    tail->next = head;
    head->next = NULL;
    return newHead;
}

void printGanttChart(int startTime, int endTime, int processID, const char* queueType) {
    printf("| P%d from %d to %d %s", processID, startTime, endTime, queueType);
}

void roundRobinScheduler(struct Process* processes, int n, int timeQuantum) {
    struct Process* head = NULL;

    // Enqueue processes based on arrival time
    for (int i = 0; i < n; i++) {
        head = addProcessByArrival(head, processes[i].processID, processes[i].burstTime, processes[i].arrivalTime, processes[i].originalArrivalTime, processes[i].originalBurstTime);
    }

    int currentTime = 0; // Initialize current time

    printf("\nGantt Chart:\n");

    while (head != NULL) {
        struct Process* currentProcess = removeFront(&head);

        // Check if a valid process was dequeued
        if (currentProcess == NULL) {
            continue;
        }

        // Adjust current time if it's before the next process's arrival
        if (currentTime < currentProcess->arrivalTime) {
            currentTime = currentProcess->arrivalTime;
        }

        int timeSlice = (currentProcess->remainingBurstTime < timeQuantum) ? currentProcess->remainingBurstTime : timeQuantum;

        // Print the Gantt chart
        printGanttChart(currentTime, currentTime + timeSlice, currentProcess->processID,"");

        currentTime += timeSlice; // Update the current time
        currentProcess->remainingBurstTime -= timeSlice; // Decrease the remaining burst time

        // Check if the process still has remaining burst time
        if (currentProcess->remainingBurstTime > 0) {
            head = addProcessByArrival(head, currentProcess->processID, currentProcess->remainingBurstTime, currentTime, currentProcess->originalArrivalTime, currentProcess->originalBurstTime);
        }
        else {
            // Update completion time when the process finishes
            currentProcess->completionTime = currentTime;

            // Calculate turnaround time
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->originalArrivalTime;

            // Calculate waiting time
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->originalBurstTime;

            processes[currentProcess->processID - 1] = *currentProcess; // Update process information in the original array
            free(currentProcess); // Free the finished process
        }
    }

    // Display the results
    printf("\n\n");
    printTableHeader();
    for (int i = 0; i < n; i++) {
        printTableRow(processes[i].processID,
                      processes[i].originalArrivalTime,
                      processes[i].originalBurstTime,
                      processes[i].waitingTime,
                      processes[i].turnaroundTime,
                      processes[i].completionTime);
    }
    printTableFooter();

    // Calculate total waiting time and total turnaround time
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    // Calculate averages
    float avgWaitingTime = (float)totalWaitingTime / n;
    float avgTurnaroundTime = (float)totalTurnaroundTime / n;

    // Print averages
    printf("\nAverage Waiting Time (Round Robin): %.2f\n", avgWaitingTime);
    printf("Average Turnaround Time (Round Robin): %.2f\n", avgTurnaroundTime);
}

// Comparison function for qsort
int compareProcessesByArrivalTime(const void* a, const void* b) {
    return ((struct Process*)a)->arrivalTime - ((struct Process*)b)->arrivalTime;
}

void sortProcessesByArrivalTime(struct Process processes[], int n) {
    qsort(processes, n, sizeof(struct Process), compareProcessesByArrivalTime);
}
void multiLevelFeedbackQueue(struct Process processes[], int n) {
    int rrTimeQuantum;
    printf("\nEnter the time quantum for Round Robin queue: ");
    scanf("%d", &rrTimeQuantum);

    struct Process* fcfsQueue = NULL;
    struct Process* rrQueue = NULL;

    // Sort processes by arrival time for MLFQ
    sortProcessesByArrivalTime(processes, n);

    int currentTime = processes[0].arrivalTime; // Set initial time to the arrival time of the first process
    int i = 0;

    printf("\n # Results (Multi-Level Feedback Queue - RR & FCFS): #\n");
    printf("\nGantt Chart (Multi-Level Feedback Queue - RR & FCFS):\n");

    while (i < n || rrQueue != NULL || fcfsQueue != NULL) {
        // Enqueue processes to RR queue based on arrival time
        while (i < n && processes[i].arrivalTime <= currentTime) {
            rrQueue = addProcessByArrival(rrQueue, processes[i].processID,
                                           processes[i].burstTime, processes[i].arrivalTime,
                                           processes[i].originalArrivalTime, processes[i].originalBurstTime);
            ++i;
        }

        if (rrQueue != NULL) {
            struct Process* currentProcess = removeFront(&rrQueue);

            int timeSlice = (currentProcess->remainingBurstTime < rrTimeQuantum) ? currentProcess->remainingBurstTime : rrTimeQuantum;

            printGanttChartMLFQ(currentTime, currentTime + timeSlice, currentProcess->processID, "(RR)", "Executing");

            currentTime += timeSlice;
            currentProcess->remainingBurstTime -= timeSlice;

            if (currentProcess->remainingBurstTime > 0) {
                // If the process still has remaining burst time, move to FCFS queue
                fcfsQueue = addProcessByArrival(fcfsQueue, currentProcess->processID,
                                                currentProcess->remainingBurstTime, currentTime,
                                                currentProcess->originalArrivalTime, currentProcess->originalBurstTime);
            } else {
                currentProcess->completionTime = currentTime;
                currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->originalArrivalTime;
                currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->originalBurstTime;

                processes[currentProcess->processID - 1] = *currentProcess;
                free(currentProcess);
            }
        } else if (fcfsQueue != NULL) {
            struct Process* currentProcess = removeFront(&fcfsQueue);

            printGanttChartMLFQ(currentTime, currentTime + currentProcess->remainingBurstTime, currentProcess->processID, "(FCFS)", "Executing");

            currentTime += currentProcess->remainingBurstTime;
            currentProcess->completionTime = currentTime;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->originalArrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->originalBurstTime;

            processes[currentProcess->processID - 1] = *currentProcess;
            free(currentProcess);
        }
    }

    printf("\n\n");
    printTableHeader();
    for (int i = 0; i < n; i++) {
        printTableRow(processes[i].processID,
                      processes[i].originalArrivalTime,
                      processes[i].originalBurstTime,
                      processes[i].waitingTime,
                      processes[i].turnaroundTime,
                      processes[i].completionTime);
    }
    printTableFooter();
}

void printGanttChartMLFQ(int startTime, int endTime, int processID, const char* queueType, const char* executionType) {
    printf("| P%d from %d to %d %s %s | \n", processID, startTime, endTime, queueType, executionType);
}

// Function to calculate average waiting time and average turnaround time for MLFQ
void calculateAverageTimesMLFQ(struct Process processes[], int n) {
    // Calculate total waiting time and total turnaround time
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    // Calculate averages
    float avgWaitingTime = (float)totalWaitingTime / n;
    float avgTurnaroundTime = (float)totalTurnaroundTime / n;

    // Print averages
    printf("\nAverage Waiting Time (MLFQ): %.2f\n", avgWaitingTime);
    printf("Average Turnaround Time (MLFQ): %.2f\n", avgTurnaroundTime);
}

float calculateThroughput(int n, int totalTime) {
    return (float)n / totalTime;
}

void printTableHeader() {
    printf("+---------+--------------+------------+--------------+-----------------+-----------------+\n");
    printf("| Process | Arrival Time | Burst Time | Waiting Time | Turnaround Time | Completion Time |\n");
    printf("+---------+--------------+------------+--------------+-----------------+-----------------+\n");
}

void printTableRow(int processID, int arrivalTime, int burstTime, int waitingTime, int turnaroundTime,int completionTime) {
    printf("|   P%-3d  |      %-2d      |     %-2d     |      %-2d      |        %-2d       |        %-2d       |\n",
           processID, arrivalTime, burstTime, waitingTime, turnaroundTime, completionTime);
}

void printTableFooter() {
    printf("+---------+--------------+------------+--------------+-----------------+-----------------+\n");
}


