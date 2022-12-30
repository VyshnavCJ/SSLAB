#include<stdio.h>

#include<stdlib.h>

typedef struct {
  int at;
  int bt;
  int wt, tat;
  int pid, priority;
}
process;

//Accept sorted by arrival Time 

void FCFSinput(process p[], int n) {
  int i, j;
  process temp;
  printf("\nEnter Process Arrival Time\n");
  for (i = 0; i < n; i++) {
    printf("P[%d]:", i + 1);
    scanf("%d", & p[i].at);
    p[i].pid = i + 1;
  }
  printf("\nEnter Process Burst Time\n");
  for (i = 0; i < n; i++) {
    printf("P[%d]:", i + 1);
    scanf("%d", & p[i].bt);
  }
  for (i = 1; i < n; i++) {
    temp = p[i];
    j = i - 1;
    while (j >= 0 && temp.at < p[j].at) {
      p[j + 1] = p[j];
      j = j - 1;
    }
    p[j + 1] = temp;
  }
}

//Accept sorted by burst Time and internally by arrival Time (non-preemptive SJF) 

void SJFinput(process p[], int n) {
  int i, j;
  process temp;
  printf("\nEnter Process details :-");
  for (i = 0; i < n; i++) {
    temp.pid = i + 1;
    printf("\nProcess [%d]", i + 1);
    printf("\nArrival Time: ");
    scanf("%d", & temp.at);
    printf("Burst Time: ");
    scanf("%d", & temp.bt);
    for (j = i - 1; j >= 0 && (temp.bt < p[j].bt || (temp.bt == p[j].bt && temp.at < p[j].at)); j--) p[j + 1] = p[j];
    p[j + 1] = temp;
  }
}

//Accept sorted by Priority and internally by arrival Time (non-preemptive Priority) 

void Priorityinput(process p[], int n) {
  int i, j;
  process temp;
  printf("\nEnter Process details:\n(Low Priority value implies Higher priority)");
  for (i = 0; i < n; i++) {
    temp.pid = i + 1;
    printf("\nProcess [%d]", i + 1);
    printf("\nArrival Time: ");
    scanf("%d", & temp.at);
    printf("Burst Time: ");
    scanf("%d", & temp.bt);
    printf("Priority: ");
    scanf("%d", & temp.priority);
    for (j = i - 1; j >= 0 && (temp.priority < p[j].priority || (temp.priority == p[j].priority &&
        temp.at < p[j].at)); j--)
      p[j + 1] = p[j];
    p[j + 1] = temp;
  }
}

void displaytable1(process p[], int n) {
  int i;
  printf("\nProcess\t\tArrival Time\tBurst Time \tWaiting Time\tTurnaround Time");
  for (int i = 0; i < n; i++) {
    printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d", p[i].pid, p[i].at, p[i].bt, p[i].wt, p[i].tat);
  }
}

void displaytable2(process p[], int n) {
  int i;
  printf("\nProcess\t\tArrival Time\tBurst Time\tPriority \tWaiting Time\tTurnaround Time");
  for (int i = 0; i < n; i++) {
    printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d", p[i].pid, p[i].at, p[i].bt, p[i].priority, p[i].wt, p[i].tat);
  }
}

//printing average waiting and turnaround time 

void printaverage(process p[], int n) {
  int i;
  float avgwt = 0, avgtat = 0;
  for (i = 0; i < n; i++) {
    avgwt += p[i].wt;
    avgtat += p[i].tat;
  }
  avgwt /= n;
  avgtat /= n;
  printf("\n\nAverage Waiting Time: %f\nAverage Turnaround Time: %f\n", avgwt, avgtat);
}

//clock represents current time of the system 

void FCFS(process p[], int n) {
  int clock, i;
  clock = p[0].at;
  for (i = 0; i < n; i++) {
    p[i].wt = clock - p[i].at;
    p[i].tat = p[i].wt + p[i].bt;
    if (i < n - 1 && clock + p[i].bt < p[i + 1].at)
      clock = p[i + 1].at; //taking idle time into consideration     else 
    clock += p[i].bt;
  }
}

void SJF(process p[], int n) {
  int selected_process, clock, i, j;
  process temp;
  clock = 0;
  for (i = 0; i < n; i++) {
    selected_process = -1;
    for (j = i; j < n; j++) //Searching for a Ready process of smallest burst time     if(p[j].at <= clock) 
    {
      selected_process = j;
      break;
    }
    if (selected_process == -1) //Search failed 
    {
      selected_process = i;
      for (j = i + 1; j < n; j++) //Searching for the earliest Available process         if(p[j].at < p[selected_process].at)           selected_process = j; 
        clock = p[selected_process].at;
    }
    p[selected_process].wt = clock - p[selected_process].at;
    p[selected_process].tat = p[selected_process].wt + p[selected_process].bt;
    clock += p[selected_process].bt;
    temp = p[selected_process];
    for (j = selected_process - 1; j >= i; j--) //moving the completed process       p[j + 1] = p[j];     p[i] = temp; 
  }
}

void PrioritySchedule(process p[], int n) {
    SJF(p, n); //only change is in condition of ordering the set during the input } 

    void RRS(process p[], int n, int tq) {
      int i, clock, timeTaken;
      int PC, queue, bt[20];
      process temp;
      for (i = 0; i < n; i++) bt[i] = p[i].bt;
      clock = p[0].at;
      PC = n;
      queue = 1;
      while (PC) {
        if (p[0].bt <= tq) {
          timeTaken = p[0].bt;
          p[0].tat = clock + timeTaken - p[0].at;
          p[0].wt = p[0].tat - bt[p[0].pid - 1];
        } else
          timeTaken = tq; //Set no of ready processes 
        while (queue < PC && clock + timeTaken >= p[queue].at) queue++;
        //Remove completed process 
        if (timeTaken - p[0].bt == 0) {
          PC--;
          queue--;
          p[0].bt = 0;
          //Insert p[0] at p[PC]       temp = p[0];       for(i = 1; i <= PC; i++)         p[i - 1] = p[i]; 
          p[PC] = temp;
        } else //Move p[0] to end of Queue 
        {
          p[0].bt -= tq;
          //Insert p[0] at p[queue - 1]       temp = p[0];       for(i = 1; i < queue; i++)       p[i - 1] = p[i]; 
          p[queue - 1] = temp;
        }
        if (queue) clock += timeTaken;
        else
          clock = p[queue].at;
      }
    }

    void main() {
      int option, n, tq, i;
      process p[20];
      while (1) {
        printf("------MENU------:\n1.FCFS\n2.SJF\n3.Priority\n4.Round Robin\n5.Exit");
        printf("\nEnter the option: ");
        scanf("%d", & option);
        while (option < 1 || option > 5) {
          printf("Invalid option");
          break;
        }
        if (option == 1) //FCFS 
        {
          printf("Enter the number of processes: ");
          scanf("%d", & n);
          FCFSinput(p, n);
          FCFS(p, n);
          displaytable1(p, n);
          printaverage(p, n);
        } else if (option == 2) //SJF 
        {
          printf("Enter the number of processes: ");
          scanf("%d", & n);
          SJFinput(p, n);
          SJF(p, n);
          displaytable1(p, n);
          printaverage(p, n);
        } else if (option == 3) // Priority Scheduling 
        {
          printf("Enter the number of processes: ");
          scanf("%d", & n);
          Priorityinput(p, n);
          PrioritySchedule(p, n);
          displaytable2(p, n);
          printaverage(p, n);
        } else if (option == 4) // Round Robin Scheduling 
        {
          printf("Enter the number of processes: ");
          scanf("%d", & n);
          printf("Enter the Time Quantum: ");
          scanf("%d", & tq);
          FCFSinput(p, n);
          RRS(p, n, tq);
          printaverage(p, n);
        } else break;
        printf("\n");
      }
    }