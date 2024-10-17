#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *waitingRoomCust;

int main(int argc, char const *argv[]) {
  waitingRoomCust = mmap(NULL, sizeof *waitingRoomCust, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  *waitingRoomCust = 01;

  srand(time(NULL));
  int barber = fork();
  printf("%d\n", barber);
  fflush(stdout);
  if (barber == 0) {
    while (1) {
      if (*waitingRoomCust > 0) {
        (*waitingRoomCust)--;
        printf("Customer has been given a haircut.\n");
        fflush(stdout);
        sleep((rand() % 12));
      }
      else {
        printf("Barber is now sleeping %d\n", *waitingRoomCust);
        fflush(stdout);
        sleep(1);
      }

    }
  }

  if (barber > 0) {
    while (1) {
      sleep(7);
      if (*waitingRoomCust <= 3) {
        (*waitingRoomCust)++;
        printf("The waiting room has now %i customers.\n", *waitingRoomCust);
        fflush(stdout);
      }
      else {
        printf("Waiting room is full, customer has left.\n");
        fflush(stdout);
      }
    }
  }
  if (barber < 0) {
    printf("failed %i ", barber);
    fflush(stdout);
  }
  return 0;
}