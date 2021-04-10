#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

int
main(int argc, char *argv[]) {
  int x1 = getusagecount(0, SYS_read);
  int x2 = getusagecount(0, SYS_read);
  char buf[100];
  (void) read(4, buf, 1);
  int x3 = getusagecount(0, SYS_read);
  int i;

  for (i = 0; i < 1000; i++) {
    (void) read(4, buf, 1);
  }

  int x4 = getusagecount(0, SYS_read);

  int x5 = getusagecount(0, SYS_getpid);

  for (i = 0; i < 500; i++)
    (void) getpid();

  int x6 = getusagecount(0, SYS_getpid);

  int x7 = getusagecount(1, SYS_getpid);

  printf(1, "XV6_TEST_OUTPUT x1: %d, x2: %d, x3: %d, x4: %d, x5: %d, x6: %d, x7: %d\n", x1, x2, x3, x4, x5, x6, x7);
  exit();
}
