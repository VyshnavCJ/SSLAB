#include <stdio.h>

#include <math.h>

#include <stdlib.h>

int direction() {
  int opt;
  printf("\n****Direction of traversal****");
  printf("\n1.Increasing oreder");
  printf("\n2.Decreasing order");
  printf("\nEnter the direction of traversal : ");
  scanf("%d", & opt);
  return opt;
}
void sort(int qsize, int diskqueue[25]) {
  int t, i, j;
  for (i = 0; i < qsize; i++)
    for (j = i + 1; j < qsize; j++)
      if (diskqueue[i] > diskqueue[j]) {
        t = diskqueue[i];
        diskqueue[i] = diskqueue[j];
        diskqueue[j] = t;
      }
}
void fcfs(int qsize, int diskqueue[25], int st) {
  int i, s = 0;
  for (i = 0; i < qsize; i++) {
    s = s + abs(st - diskqueue[i]); // taking absolute value 
    st = diskqueue[i];
  }
  printf("\nDisk traversal order\n");
  for (i = 0; i < qsize; i++)
    printf(" %d ", diskqueue[i]);
  printf("\n Total seek time :%d", s);
}
void scan(int qsize, int diskqueue[25], int st, int size) {
  int i, dir, j, s = 0, index;
  sort(qsize, diskqueue);
  dir = direction();
  printf("\nDisk traversal order \n");
  
  for (i = 0; i < qsize; i++) {
    if (st < diskqueue[i]) {
      index = i;
      break;
    }
  }
  if (dir == 1) {
    // movement is towards high value 
    for (i = index; i < qsize; i++) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      printf("%d ", diskqueue[i]);
    }
    // last movement for max size 
    s = s + abs(size - diskqueue[i - 1] - 1);
    st = size - 1;
    for (i = index - 1; i >= 0; i--) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      printf("%d ", diskqueue[i]);
    }
  } else if (dir == 2) {
    for (i = index - 1; i >= 0; i--) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      printf("%d ", diskqueue[i]);
    }
    // last movement for smallest size 
    s = s + abs(0 - diskqueue[i + 1]);
    st = 0;
    for (i = index; i < qsize; i++) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      printf("%d ", diskqueue[i]);
    }
  }
  printf("\n Total seek time : %d", s);
}
void CSCAN(int qsize, int diskqueue[25], int st, int size) {
  
  int i, j, s = 0, index, dir;
  sort(qsize, diskqueue);
  dir = direction();
  printf("\nDisk traversal order \n");
  for (i = 0; i < qsize; i++) {
    if (st < diskqueue[i]) {
      index = i;
      break;
    }
  }
  if (dir == 1) {
    // movement is towards high value 
    for (i = index; i < qsize; i++) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      printf("%d ", diskqueue[i]);
    }
    // last movement for max size 
    s = s + abs(size - diskqueue[i - 1] - 1);
    /*movement max to min disk */
    s = s + abs(size - 1);
    st = 0;
    for (i = 0; i < index; i++) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      printf("%d ", diskqueue[i]);
    }
  } else if (dir == 2) {
    for (i = index - 1; i >= 0; i--) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      printf("%d ", diskqueue[i]);
    }
    // last movement for smallest size 
    s = s + abs(0 - diskqueue[i + 1]);
    /*movement min to max disk */
    s = s + abs(size - 1);
    st = size - 1;
    for (i = qsize - 1; i >= index; i--) {
      s = s + abs(diskqueue[i] - st);
      st = diskqueue[i];
      
      printf("%d ", diskqueue[i]);
    }
  }

  printf("\n Total seek time : %d", s);
}
int main() {
  int n, diskqueue[25], start, i, j, qsize, ch;
  printf("\n Enter the maximum number of cylinders : ");
  scanf("%d", & n);
  printf("\n Enter the number of disk queue elements :");
  scanf("%d", & qsize);
  printf("\n Enter the disk queue elements : ");
  for (i = 0; i < qsize; i++) {
    scanf("%d", & diskqueue[i]);
  }
  printf("\n Enter the disk start starting posision : ");
  scanf("%d", & start);
  while (1) {
    printf("\n\n-----MENU------ ");
    printf("\n 1. FCFS ");
    printf("\n 2. SCAN ");
    printf("\n 3. C-SCAN ");
    printf("\n 4. EXIT ");
    printf("\nEnter your choice: ");
    scanf("%d", & ch);
    switch (ch) {
    case 1:
      printf("\n FCFS disk scheduling \n");
      fcfs(qsize, diskqueue, start);
      break;
    case 2:
      printf("\n SCAN disk scheduling \n");
      scan(qsize, diskqueue, start, n);
      break;
    case 3:
      printf("\n C-SCAN disk scheduling \n");
      CSCAN(qsize, diskqueue, start, n);
      break;
    case 4:
      exit(0);
    }
  }
}