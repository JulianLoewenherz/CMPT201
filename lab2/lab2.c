#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
// program must recieve user input from the keyboard that is the path of a
// program executes the command the user typed in
//  repeates the above two steps forever

// large scale idea: I'd have a process going and then whenever the user enters
// input I fork with this fork I then do execl() and execute the users input. I
// have the parent wait till this fork (or any child is done). when using
// waitpid I have to check if the result was not -1. wstatus stores the exit
// status of the process.

int main(void) {
  while (true) {

    char *buffptr = NULL;
    size_t size = 0;

    printf("Enter programs to run.\n> ");
    ssize_t len = getline(&buffptr, &size, stdin);

    if (len == -1) { // error handling
      free(buffptr);
      break;
    }

    // stripping \n
    if (len > 0 && buffptr[len - 1] == '\n') {
      buffptr[len - 1] = '\0';
    }

    // forking
    pid_t pid = fork();

    if (pid == -1) {
      // erro
      perror("error forking");
      free(buffptr);
      break;
    } else if (pid == 0) {
      // must be the child
      execl(buffptr, buffptr, (char *)NULL);
      printf("Exec failure\n");
      free(buffptr);
      exit(EXIT_FAILURE);

    } else {
      // must be the parent and pid is the childs pid
      // waiting for the child process to finish
      int wstatus;
      if (waitpid(pid, &wstatus, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
    }
    // I have to free buffptr
    free(buffptr);
  }

  return 0;
}
