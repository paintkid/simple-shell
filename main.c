#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFSIZE 64
#define TOKEN_DELIMITERS " \t\r\n\a"

/**
 * @brief Splits a line into tokens (arguments).
 * @param line The line to split.
 * @return Null-terminated array of tokens.
 */
char **shell_parse_line(char *line) {
  int bufsize = TOKEN_BUFSIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOKEN_DELIMITERS);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOKEN_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOKEN_DELIMITERS);
  }
  tokens[position] = NULL;
  return tokens;
}

void shell_loop(void) {
  char *line = NULL; 
  char **args;
  size_t len = 0;
  ssize_t nread;

  while (1) {
    printf("> ");
    nread = getline(&line, &len, stdin);

    if (nread == -1) {
      printf("\n");
      break;
    }
    
    args = shell_parse_line(line);

    for (int i = 0; args[i] != NULL; i++) {
        printf("arg[%d]: %s\n", i, args[i]);
    }

    free(args);
  }

  free(line);
}

int main(int argc, char **argv) {
  shell_loop();
  return EXIT_SUCCESS;
}