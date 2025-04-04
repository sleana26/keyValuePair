/**
 * @file driver.c
 * @author Sean Leana (smleana)
 * The driver file is responsible with handling whether input is coming from a terminal or not and parsing command line arguments
 * It also handles commands from the user and makes edits to the map or getting information from the map when 
 * prompted. The driver file contains main
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>
#include <ctype.h>
#include <unistd.h> // Unix-specific isatty() function.

#include "map.h"
#include "value.h"
#include "input.h"

#define MAX_COMMAND_SIZE 7
#define ERROR_JMP 1
/** Print out a usage message and exit unsuccessfully. */
static void usage()
{
  fprintf( stderr, "Usage: driver [-term]\n" );
  exit( EXIT_FAILURE );
}

/**
 * handle Command parses a command from the given line of text and either updates the given map or prints a line to the terminal.
 * If the given line contains a quit command, the function will return true so main knows its time to exit. Otherwise it should
 * return false. The given jump_buf provides a way for this function to jump back to error handling in main, if the given line isnt
 * a valid command
 * @param map the map to update or get info from
 * @param line the line of text (command) to be handled
 * @param env provides a way for the function to jump back to error handling in main
 * @return true if a quit command is handled
 */
bool handleCommand( Map *map, char const *line, jmp_buf *env )
{
  if(line == NULL) {
    freeMap(map);
    exit(1);
  }
  //get first word which should be command word
  char command[MAX_COMMAND_SIZE];
  char key[KEY_LIMIT + 1];

  int n = 0;
  int offset = 0;
  sscanf(line, "%s%n", command, &n);
  offset += n;
  //if command is set
  if(strcmp(command, "set") == 0) {
    //get second word which is key
    sscanf(line + offset, "%24s%n", key, &n);
    offset += n;
    //checks if key is last word???
    if(line[offset] != ' ' && line[offset] != '\n' && line[offset] != EOF) {
      longjmp(*env, ERROR_JMP);
    }
    //create value array size of line - offset of previously read in chars plus a space for \0
    char value[strlen(line) - offset + 1];
    //get third word which is value
    //sscanf(line + offset, "%24s%n", value, &n);
    while (line[offset] == ' ') {
      offset++;
    }
    strcpy(value, line + offset);
    //parse int
    Value *val = NULL;
    val = parseInteger(value);
    if (val == NULL) {
      val = parseDouble(value);
    }
    if(val == NULL) {
      val = parseString(value);
    }
    if(val == NULL) {
      longjmp(*env, ERROR_JMP);
    }
    //set map
    mapSet(map, key, val);
  } else if(strcmp(command, "remove") == 0) {
    //get second word which is key
    sscanf(line + offset, "%s%n", key, &n);
    offset += n;
    //remove key from map
    if(!mapRemove(map, key)) {
      longjmp(*env, ERROR_JMP);
    }
  //if command is get
  } else if (strcmp(command, "get") == 0) {
    //get second word which is key
    sscanf(line + offset, "%s%n", key, &n);
    offset += n;
    for(int i = 0; i < strlen(key); i++) {
      if(isspace(key[i])) {
        longjmp(*env, ERROR_JMP);
      }
    }
    if(line[offset] != '\000' && line[offset] != '\n' && line[offset] != EOF) {
      longjmp(*env, ERROR_JMP);
    }
    if(strlen(key) == 0) {
      longjmp(*env, ERROR_JMP);
    }
    Value *val = mapGet(map, key);
    if(val == NULL) {
      longjmp(*env, ERROR_JMP);
    }
    val->print(val);
  } else if(strcmp(command, "size") == 0) {
    fprintf(stdout, "%d\n", mapSize(map));
    return false;
  } else if (strcmp(command, "quit") == 0) {
    return true;
  }
  return false;
}

/**
   Starting point for the program.
   @param argc number of command-line arguments.
   @param argv array of strings given as command-line arguments.
   @return exit status for the program.
 */
int main( int argc, char *argv[] )
{
  // See if our input is from a terminal.
  bool interactive = isatty( STDIN_FILENO );

  // Parse command-line arguments.
  int apos = 1;
  while ( apos < argc ) {
    // The -term option makes the program behave as if it's in interactive mode,
    // even if it's reading from a file.
    if ( strcmp( argv[ apos ], "-term" ) == 0 ) {
      interactive = true;
      apos += 1;
    } else
      usage();
  }
  if (interactive) {
    fprintf(stdout, "cmd> ");
  }
  //create map
  Map *map = makeMap(10);

  //where does this file come from????
  char *line;
  jmp_buf env;
  //READ IN line
  while((line = readLine(stdin)) != NULL) {
    if (setjmp(env) == 0) {
      if (handleCommand(map, line, &env)) {
        freeMap(map);
        free(line);
        return EXIT_SUCCESS;
      }
      free(line);
      if (interactive) {
        fprintf(stdout, "cmd> ");
      }
    } else {
      if(!interactive) {
        fprintf(stderr, "Invalid command: %s\n", line);
        free(line);
        exit(1);
      } else {
        fprintf(stdout, "Invalid command\n");
        fprintf(stdout, "cmd> ");
      }
    }
  }
  freeMap(map);
  return EXIT_SUCCESS;
}
