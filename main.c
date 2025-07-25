#include <stdio.h>
#include <stdlib.h>

void shell_loop(void) {
  char *line = NULL; 
  size_t len = 0;
  ssize_t nread;

  while (1) {
    printf("> ");
    nread = getline(&line, &len, stdin);

    if (nread == -1) {
      printf("\n");
      break;
    }
    
    printf("%s", line);
  }

  free(line);
}

int main(int argc, char **argv) {
  shell_loop();
  return EXIT_SUCCESS;
}