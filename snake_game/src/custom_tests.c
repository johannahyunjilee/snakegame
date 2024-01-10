#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x)
{
  return x > 42;
}

bool is_vowel(char c)
{
  char *vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++)
  {
    if (c == vowels[i])
    {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two()
{
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1))
  {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2))
  {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3))
  {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel()
{
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1))
  {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2))
  {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3))
  {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4))
  {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5))
  {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6))
  {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail()
{
  char w = 'w';
  if (!assert_true("w is tail", is_tail(w)))
  {
    return false;
  }

  char a = 'a';
  if (!assert_true("a is tail", is_tail(a)))
  {
    return false;
  }
  char s = 's';
  if (!assert_true("s is tail", is_tail(s)))
  {
    return false;
  }

  char d = 'd';
  if (!assert_true("d is tail", is_tail(d)))
  {
    return false;
  }
  return true;
}

bool test_is_head()
{
  // TODO: Implement this function.
  char W = 'W';
  if (!assert_true("W is head", is_head(W))) {
    return false;
  }
  char A = 'A';
  if (!assert_true("A is head", is_head(A))) {
    return false;
  }
  char S = 'S';
  if (!assert_true("S is head", is_head(S))) {
    return false;
  }
  char D = 'D';
  if (!assert_true("D is head", is_head(D))) {
    return false;
  }
  char x = 'x';
  if (!assert_true("x is head", is_head(x))) {
    return false;
  }
  return true;
}

bool test_is_snake()
{

  char valid_snake[] = {'w','a','s','d','^','<','v','>','W','A','S','D','x'};
  for (int i = 0; i < sizeof(valid_snake); i++) {
    if (!assert_true("part of snake", is_snake(valid_snake[i]))) {
      return false;
    }
  }
  return true;
}

bool test_body_to_tail()
{
  char up = '^';
  char left = '<';
  char right = '>';
  char down = 'v';

  if (!assert_equals_char("up", 'w', body_to_tail(up))) {return false;}
  if (!assert_equals_char("left", 'a', body_to_tail(left))) {return false;}
  if (!assert_equals_char("down", 's', body_to_tail(down))) {return false;}
  if (!assert_equals_char("right", 'd', body_to_tail(right))) {return false;}
  return true;
}

bool test_head_to_body()
{
  char up = 'W';
  char left = 'A';
  char down = 'S';
  char right = 'D';

  if (!assert_equals_char("up", '^', head_to_body(up))) {return false;}
  if (!assert_equals_char("left", '<', head_to_body(left))) {return false;}
  if (!assert_equals_char("down", 'v', head_to_body(down))) {return false;}
  if (!assert_equals_char("right", '>', head_to_body(right))) {return false;}
  return true;
}

bool test_get_next_row()
{
  char next[] = {'v','s','S'};

  for (int i = 0; i < sizeof(next); i++) {
    if (!assert_equals_int("plus one", 4, get_next_row(3, next[i]))) {
      return false; 
    }
  }

  char prev[] = {'^', 'w', 'W'};

  for (int i = 0; i < sizeof(prev); i++) {
    if (!assert_equals_int("minus one", 2, get_next_row(3, prev[i]))) {
      return false;
    }
  }
  
  char random = 'd';
  if (!assert_equals_int("return row", 3, get_next_row(3, random))) {
      return false; 
    }

  return true;
}

bool test_get_next_col()
{
  char next[] = {'>','d','D'};

  for (int i = 0; i < sizeof(next); i++) {
    if (!assert_equals_int("plus one", 4, get_next_col(3, next[i]))) {
      return false; 
    }
  }

  char prev[] = {'<', 'a', 'A'};

  for (int i = 0; i < sizeof(prev); i++) {
    if (!assert_equals_int("minus one", 2, get_next_col(3, prev[i]))) {
      return false;
    }
  }
  
  char random = 's';
  if (!assert_equals_int("return row", 3, get_next_col(3, random))) {
      return false; 
    }

  return true;
}

bool test_customs()
{
  if (!test_greater_than_forty_two())
  {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel())
  {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail())
  {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head())
  {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake())
  {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail())
  {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body())
  {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_row())
  {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col())
  {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  init_colors();

  if (!test_and_print("custom", test_customs))
  {
    return 0;
  }

  return 0;
}
