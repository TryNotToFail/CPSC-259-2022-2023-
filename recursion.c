/*
 File:          recursion.c
 Purpose:       Exercises for CPSC259 lab 4
 Author:		Duc Thang Huynh	
 Student #s:	22946222
 CWLs:		    thangpro@student.ubc.ca
 Date:			Nov 4th, 2022
 */

 /******************************************************************
	PLEASE EDIT THIS FILE

	Comments that start with // should be replaced with code
	Comments that are surrounded by * are hints
	******************************************************************/


	/* Preprocessor directives */
#include "recursion.h"

/*
 Calculates the power.
 PARAM:     base is an integer
 PARAM:     power is an integer
 PRE:       power >= 0
 PRE:       base != 0
 RETURN:    base^power
 */
int calculate_power(int base, int power)
{
	// Replace this return statement with your own code
	int answer = 0;
	if (power == 0) {
		answer = 1;
	}
	else {
		answer = calculate_power(base, power - 1);
		answer *= base;
	}
	return answer;
}

/*
 Returns the number of digits in an integer
 PARAM:     number is an integer
 PRE:       0 < number <= INT_MAX
 RETURN:    the number of digits in the number
 */
int count_digits(int number)
{
	// Replace this return statement with your own code
	int count = 0;
	if (number == 0) {
		count = 0;
	}
	else {
		count = count_digits(number/10);
		count++;
	}
	return count;
}

/*
 Returns the length of the specified string.
 PARAM:  string, a pointer to an array of char
 PRE:    the string pointer is not a dangling pointer
 RETURN: the length of the string passed as a parameter
 */
int string_length(char* string)
{
	int count = 0;
	if (*string == '\0') {
		count = 0;
	}
	else {
		count = 1 + string_length(string + 1);
	}
	return count;
}

/*
 Determines if a string is a palindrome.  DOES NOT skip spaces!
 For example,
 ""     -> returns 1 (an empty string is a palindrome)
 "the"  -> returns 0
 "abba" -> returns 1
 "Abba" -> returns 0
 "never odd or even" -> returns 0
 For the recursive call, be aware that both parameters can be modified.
 PARAM:  string, a pointer to an array of char
 PARAM:  string_length, the length of the string
 PRE:    the string pointer is not a dangling pointer
 PRE:    string_length is the correct length of the string
 RETURN: IF string is a palindrome
				 THEN 1
		 END 0
 */
int is_palindrome(char* string, int string_length)
{
	// Replace this return statement with your own code
	int result = 1;
	if (string_length == 1 || string_length == 0) {
		result = 1;
	}
	else {
		char first = string[0];
		char last = string[string_length - 1];
		if (first != last) {
			result = 0;
		}
		else {
			result = is_palindrome(string + 1, string_length - 2);
		}
	}
	return result;
}

/*
 Draws a ramp.  The length of the longest rows is specified by the parameter.
 For the recursive call, be aware that both parameters can be modified.
 PARAM:     number, an integer
 PARAM:     buffer, a character array of sufficient length
 PRE:       number >= 1
						buffer has sufficient length and contains null characters from the buffer
						address to the end of its length
 POST:      draws a ramp whose height is the specified number into buffer
 RETURN:    the number of characters written into buffer. Don't forget to count your
						newline characters!
 */
int draw_ramp(int number, char* buffer)
{
	int digit = 0;
	int total = 0;
	if (number == 1) {
		draw_row(number, buffer);
		return 1;
	}
	else {
		digit = draw_row(number, buffer);
		buffer[digit] = '\n';
		total = 1 + digit + draw_ramp(number - 1, buffer + digit + 1);
		digit = draw_row(number, buffer + total + 1);
		buffer[total] = '\n';
		return total += 1 + digit;
	}
}

/*
 Draws a row of asterisks of the specified length
 For the recursive call, be aware that both parameters can be modified.
 PARAM:  size, an integer
 PARAM:  buffer, a character array of sufficient length
 PRE:    size >= 1
				 buffer has sufficient length and contains null characters from the buffer
				 address to the end of its length
 POST:   draws a row of asterisks of specified length to buffer
 RETURN: the number of characters drawn
 */
int draw_row(int size, char* buffer)
{
	int count = 0;
	if (size == 0) {
		if (size == 1)
			count = 1;
	}
	else {
		buffer[0] = '*';
		count = 1 + draw_row(size - 1, buffer + 1);
	}
	return count;
}