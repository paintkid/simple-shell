#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>

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

/**
 * @brief Forks a new process to execute the given command.
 * @param args Null-terminated list of arguments (command and its parameters).
 * @return 1 to continue the shell loop, 0 to terminate.
 */
int shell_launch(char **args) {
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("shell");
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

void shell_loop(void) {
  char *line = NULL; 
  char **args;
  size_t len = 0;
  ssize_t nread;
  int status;

  do {
    printf("> ");
    nread = getline(&line, &len, stdin);

    if (nread == -1) {
      printf("\n");
      status = 0; 
      break;
    }
    
    args = shell_parse_line(line);
    status = shell_launch(args);

    free(args);
  } while (status);

  free(line);
}

int main(int argc, char **argv) {
  shell_loop();
  return EXIT_SUCCESS;
}