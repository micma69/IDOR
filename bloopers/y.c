#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

int id = 0;
int passwd = 777;

int flag() {
  char flag[48];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(flag, sizeof(flag), file);
  printf("%s", flag);
  return 0;
}

void read_flag() {
  if (!id && !passwd) {
    printf("Sorry, you are not our Emperor! Go and never return, you liar!\n");
  }
  else {
    printf("This way, Your Highness.\n");
    flag();
  }

}

int main(int argc, char **argv) {

  setvbuf(stdout, NULL, _IONBF, 0);

  char buf[64];
  
  // Set the gid to the effective gid
  // this prevents /bin/sh from dropping the privileges
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  
  printf("You said that you're our Emperor, but you just lost your crown while you were hunting \n");
  printf("As a matter of fact, every nobles have their own ID and password \n");
  printf("And, you have to tell us your ID and Password to prove your claim. Would you? (yes/no)\n");

  fgets(buf, sizeof(buf), stdin);
  
  if (strstr(buf, "no") != NULL) {
    printf("LIAR!...\n");
    exit(1);
  }
  else if (strstr(buf, "yes") == NULL) {
    puts("What are you mumbling about:\n");
    printf(buf);
  }
  
  read_flag();

}
