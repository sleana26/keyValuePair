/**
 * @file value.h
 * @author Sean Leana (smleana)
 * Value if a helper file of value.c which parses prints and destroys values.
 * Values can be a string a double or an int
 */
#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>

/** Abstract type used to represent an arbitrary type of value. */
typedef struct ValueStruct {
  /** Pointer to a function that prints this value to the terminal.
      @param v Pointer to the value object to print. */
  void (*print)( struct ValueStruct const *v );

  /** Pointer to a function that frees memory for this Value instance.
      @param v Pointer to the value object to free. */
  void (*destroy)( struct ValueStruct *v );

  /** Arbitrary data used to represent the integer/string/etc for this value. */
  void *data;
} Value;

/** Return true if the given string contains only whitespace.  This
    is useful for making sure there's nothing extra at the end of a line
    of user input.
    @paam str String to check for blanks.
    @return True if the string contains only blanks.
*/
bool blankString( char const *str );

/** If possible, parse an integer from the given string and return a
    Value instance containing it.  Return NULL if the string isn't in the
    proper format.
    @paam str String from which to parse an integer.
    @return new Value containing the integer, or NULL if it's not in the proper format.
*/
Value *parseInteger( char const *str );

/**
 * parses a double from the line passed into it
 * @param str line to parse double from
 * @return a value with allocated memory and a string data value
 */
Value *parseDouble(char const *str);

/**
 * parses a string from input
 * @param str string to parse from
 * @return a value with allocated memory and a string data value 
 */
Value *parseString(char const *str);

#endif
