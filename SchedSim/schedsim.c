// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
#include <stdbool.h>

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 


// Function to find the waiting time for all  
// processes
// Function to find the waiting time for all processes using Round Robin
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) {
    int rem_bt[n];
    
    // Initialize remaining burst times
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0; // Initialize waiting times to 0
    }

    int t = 0; // Current time

    // Keep traversing processes in a round-robin manner until all are done
    while (1) {
        bool done = true;

        for (int i = 0; i < n; i++) {
            // If burst time of a process is greater than 0, then only need to process further
            if (rem_bt[i] > 0) {
                done = false; // There is a pending process

                if (rem_bt[i] > quantum) {
                    // Increase the value of current time i.e. indicates how much time a process has been processed
                    t += quantum;
                    // Decrease the burst_time of current process by quantum
                    rem_bt[i] -= quantum;
                }
                // If burst time is smaller than or equal to quantum. Last cycle for this process
                else {
                    // Increase the value of current time with the remaining burst time
                    t += rem_bt[i];
                    // Waiting time is current time minus burst time of the process
                    plist[i].wt = t - plist[i].bt;
                    // As the process gets fully executed, make its remaining burst time = 0
                    rem_bt[i] = 0;
                }
            }
        }

        // If all processes are done
        if (done == true)
            break;
    }
}

// Function to find the waiting time for all  
// processes 
// Function to find waiting time for Shortest Remaining Time First (SRTF - Preemptive SJF)
void findWaitingTimeSJF(ProcessType plist[], int n) {
    int rem_bt[n];
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }

    int completed = 0;
    int current_time = 0;
    int min_bt = 9999;
    int shortest = 0;
    bool check = false;

    // Traverse until all processes get completely executed
    while (completed != n) {
        int min_bt = 9999;
        // Find process with minimum remaining time among the ones that have arrived by current_time
        for (int j = 0; j < n; j++) {
            if ((plist[j].art <= current_time) && (rem_bt[j] < min_bt) && (rem_bt[j] > 0)) {
                min_bt = rem_bt[j];
                shortest = j;
                check = true;
            }
        }

        if (check == false) {
            current_time++;
            continue;
        }

        // Reduce remaining time by 1
        rem_bt[shortest]--;

        // Update minimum burst time
        min_bt = rem_bt[shortest];
        if (min_bt == 0) {
            min_bt = 9999;
        }

        // If a process gets completely executed
        if (rem_bt[shortest] == 0) {
            completed++;
            check = false;

            // Finish time of current process is current_time + 1
            int finish_time = current_time + 1;

            // Calculate waiting time = finish_time - arrival_time - burst_time
            plist[shortest].wt = finish_time - plist[shortest].art - plist[shortest].bt;

            if (plist[shortest].wt < 0) {
                plist[shortest].wt = 0;
            }
        }

        // Increment time lap by one
        current_time++;
    }
}

// Function to find the waiting time for all  
// processes 
// Function to find the waiting time for all processes (FCFS with arrival time support)
// Function to find the waiting time for all processes (FCFS with proper arrival time and service tracking)
void findWaitingTime(ProcessType plist[], int n) {
    int service_time = plist[0].art; // Start at the first process's arrival time
    
    plist[0].wt = 0;
    service_time += plist[0].bt;

    for (int i = 1; i < n; i++) {
        // If the CPU is idle (current time is less than the process's arrival time)
        if (service_time < plist[i].art) {
            service_time = plist[i].art;
        }

        // Waiting time = current service time - process's arrival time
        plist[i].wt = service_time - plist[i].art;

        // Update service time for the next process
        service_time += plist[i].bt;
    }
}
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  


//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
// Comparator function used by qsort to sort processes by priority
int comparePriority(const void *a, const void *b) {
    ProcessType *p1 = (ProcessType *)a;
    ProcessType *p2 = (ProcessType *)b;
    return p2->pri - p1->pri;
}

// Function to calculate average time using Priority Scheduling
void findavgTimePriority( ProcessType plist[], int n) 
{
    /* 
    * 1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
    * 2- Now simply apply FCFS algorithm.
    */

    // Step 1: Sort processes by priority using qsort
    qsort(plist, n, sizeof(ProcessType), comparePriority);

    // Step 2: Apply FCFS waiting time logic on the sorted processes
    plist[0].wt = 0;
    for (int i = 1; i < n; i++) {
        plist[i].wt = plist[i - 1].wt + plist[i - 1].bt;
    }

    // Function to find turn around time for all processes
    findTurnAroundTime(plist, n);

    // Display processes along with all details
    printf( "\n*********\nPriority\n");
}



ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 