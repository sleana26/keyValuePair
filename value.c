/**
 * @file value.c
 * @author Sean Leana (smleana)
 * Value parses prints and destroys values. Values can be a string a double or an int
 */
#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


bool blankString( char const *str )
{
  // Skip spaces.
  while ( isspace( *str ) )
    ++str;

  // Return false if we see non-whiespace before the end-of-string.
  if ( *str )
    return false;
  return true;
}

// Generic destroy method, suitable for most types of values.  It
// assumes the data can be freed as a single block of heap memory.
static void destroyGeneric( Value *v )
{
  free( v->data );
  free( v );
}

// print method for Integer.
static void printInteger( Value const *v )
{
  // Print the data as an integer.
  printf( "%d\n", * (int *) v->data );
}

// print method for double.
static void printDouble( Value const *v )
{
  // Print the data as a double.
  printf( "%.2f\n", * (double *) v->data );
}

// print method for string.
static void printString( Value const *v )
{
  // Print the data as a double.
  printf( "%s\n", (char *)v->data );
}

Value *parseInteger( char const *str )
{
  // Parse an integer value from the given string.
  int val, n;
  if ( sscanf( str, "%d%n", &val, &n ) != 1 ) {
    return NULL;
  }
  // Make sure there's nothing extra after the integer value
  if ( ! blankString( str + n ) ) {
    return NULL;
  }

  // Allocate space for the value struct and the integer in its data field.
  Value *this = (Value *) malloc( sizeof( Value ) );
  this->data = malloc( sizeof( int ) );
  * (int *) this->data = val;

  // Fill in function pointers and return this value.
  this->print = printInteger;
  this->destroy = destroyGeneric;
  return this;
}

/**
 * parses a double from the line passed into it
 * @param str line to parse double from
 * @return a value with allocated memory and a string data value
 */
Value *parseDouble(char const *str)
{
  double val;
  int n;

  if( sscanf(str, "%lf%n", &val, &n) != 1 ) {
    return NULL;
  }
  // Make sure there's nothing extra after the double value
  if ( ! blankString( str + n ) )
    return NULL;
  
  // Allocate space for the value struct and the double in its data field.
  Value *this = (Value *) malloc( sizeof( Value ) );
  this->data = malloc( sizeof( double ) );
  * (double *) this->data = val;

  // Fill in function pointers and return this value.
  this->print = printDouble;
  this->destroy = destroyGeneric;
  return this;
}

/**
 * parses a string from input
 * @param str string to parse from
 * @return a value with allocated memory and a string data value 
 */
Value *parseString(char const *str)
{
  if(str == NULL || str[0] != '\"') {
    return NULL;
  }
  char *parsedStr;
  int capacity = 5;
  int len = 0;
  parsedStr = (char *)malloc(capacity * sizeof(char));

  parsedStr[len++] = '\"';

  while(str[len] != '\"') {
    if(str[len] == '\0' || str[len] == '\n') {
      return NULL;
    }
    if(len >= capacity - 1) {
      capacity *= 2;
      parsedStr = (char *)realloc(parsedStr, capacity);
    }
    parsedStr[len] = str[len];
    len++;
  }
  parsedStr[len++] = '\"';
  parsedStr[len] = '\0';

  // Make sure there's nothing extra after the string value
  if ( ! blankString( str + len) ) {
    return NULL;
  }

  Value *this = (Value *) malloc( sizeof( Value ) );
  this->data = parsedStr;
  this->print = printString;
  this->destroy = destroyGeneric;

  return this;
}