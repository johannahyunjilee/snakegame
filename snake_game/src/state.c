#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state()
{
  // TODO: Implement this function.
  struct game_state_t *game_state;
  struct snake_t snake1;

  // allocating memory space to each struct
  game_state = malloc(sizeof(game_state_t));

  // assigning default board game
  game_state->num_rows = 18;
  game_state->board = malloc(sizeof(char *) * 19);

  // creating the board and assigning ch to the board row and columns
  for (int i = 0; i < game_state->num_rows; i++)
  {
    game_state->board[i] = malloc(sizeof(char) * 21);

    if (i == 0 || i == 17)
    {
      strcpy(game_state->board[i], "####################");
    }
    else
    {
      strcpy(game_state->board[i], "#                  #");
    }
  }

  game_state->board[2][2] = 'd';
  game_state->board[2][3] = '>';
  game_state->board[2][4] = 'D';
  game_state->board[2][9] = '*';

  game_state->num_snakes = 1;
  game_state->snakes = malloc(sizeof(snake_t));

  snake1.tail_row = 2;
  snake1.tail_col = 2;
  snake1.head_row = 2;
  snake1.head_col = 4;
  snake1.live = true;

  game_state->snakes[0] = snake1;

  return game_state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
  for (int i = 0; i < state->num_rows; i++)
  {
    free(state->board[i]);
  }

  free(state->snakes);
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{

  fprintf(fp, "%s", state->board[0]);
  for (int i = 1; i < state->num_rows; i++)
  {
    fprintf(fp, "\n%s", state->board[i]);
  }
  fprintf(fp, "\n");
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename)
{
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col)
{
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch)
{
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
  return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
  return c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x';
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
  const char snake_characters[] = "wasd^<v>WASDx";
  for (int i = 0; snake_characters[i] != '\0'; i++)
  {
    if (c == snake_characters[i])
    {
      return true;
    }
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
  // TODO: Implement this function.
  switch (c)
  {
  case '^':
    return 'w'; // Up
  case '<':
    return 'a'; // Left
  case 'v':
    return 's'; // Down
  case '>':
    return 'd'; // Right
  default:
    return c; // output may be undefined for characters that are not a snake's body
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c)
{
  // TODO: Implement this function.
  switch (c)
  {
  case 'W':
    return '^'; // Up
  case 'A':
    return '<'; // Left
  case 'S':
    return 'v'; // Down
  case 'D':
    return '>'; // Right
  default:
    return c; // output may be undefined for characters that are not a snake's body
  }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
  if (c == 'v' || c == 's' || c == 'S')
  {
    return cur_row + 1;
  }
  if (c == '^' || c == 'w' || c == 'W')
  {
    return cur_row - 1;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
  if (c == '>' || c == 'd' || c == 'D')
  {
    return cur_col + 1;
  }
  if (c == '<' || c == 'a' || c == 'A')
  {
    return cur_col - 1;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
  unsigned int s_head_row = state->snakes[snum].head_row;
  unsigned int s_head_col = state->snakes[snum].head_col;

  char head_char = get_board_at(state, s_head_row, s_head_col);

  if (!is_head(head_char))
  {
    return '?';
  }

  unsigned int next_row = get_next_row(s_head_row, head_char);
  unsigned int next_col = get_next_col(s_head_col, head_char);

  return get_board_at(state, next_row, next_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
  unsigned int s_head_row = state->snakes[snum].head_row;
  unsigned int s_head_col = state->snakes[snum].head_col;
  char head_char = get_board_at(state, s_head_row, s_head_col);

  // update board
  state->board[s_head_row][s_head_col] = head_to_body(head_char);

  unsigned int next_row = get_next_row(s_head_row, head_char);
  unsigned int next_col = get_next_col(s_head_col, head_char);
  state->board[next_row][next_col] = head_char;

  // update snake struct
  state->snakes[snum].head_row = next_row;
  state->snakes[snum].head_col = next_col;
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
  unsigned int s_tail_row = state->snakes[snum].tail_row;
  unsigned int s_tail_col = state->snakes[snum].tail_col;
  char tail_char = get_board_at(state, s_tail_row, s_tail_col);

  // update board
  state->board[s_tail_row][s_tail_col] = ' ';

  unsigned int next_row = get_next_row(s_tail_row, tail_char);
  unsigned int next_col = get_next_col(s_tail_col, tail_char);
  char body_char = get_board_at(state, next_row, next_col);

  state->board[next_row][next_col] = body_to_tail(body_char);

  // update snake struct
  state->snakes[snum].tail_row = next_row;
  state->snakes[snum].tail_col = next_col;
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  // TODO: Implement this function.

  // check the next tile following head
  // if next tile = snake or wall, snake die
  // if next tile = fruit, update head without tail
  // after fruit consumed, generate new fruit

  for (unsigned int i = 0; i < state->num_snakes; i++)
  {
    unsigned int head_row = state->snakes[i].head_row;
    unsigned int head_col = state->snakes[i].head_col;
    // unsigned int tail_row = state->snakes[i].tail_row;
    // unsigned int tail_col = state->snakes[i].head_col;
    char next = next_square(state, i);
    // char head_char = get_board_at(state, head_row, head_col);
    // char tail_char = get_board_at(state, tail_row, tail_col);

    // unsigned int next_row_head = get_next_row(head_row, head_char);
    // unsigned int next_col_head = get_next_col(head_col, head_char);
    // unsigned int next_row_tail = get_next_row(tail_row, tail_char);
    // unsigned int next_col_tail = get_next_col(tail_row, tail_char);

    if (is_snake(next) || next == '#')
    {
      // update head to be x
      set_board_at(state, head_row, head_col, 'x');
      state->snakes[i].live = false;
    }

    else if (next == '*')
    {
      // update head to grow one unit
      update_head(state, i);
      // generate new fruit - what do we do with int?
      add_food(state);
    }
    else
    { // updating snake if no wall or fruit
      // head grows
      update_head(state, i);
      // tail deletes and moves
      update_tail(state, i);
    }
  }

  return;
}

/* Task 5 */
game_state_t *load_board(FILE *fp)
{
  // TODO: Implement this function.
  // use fgetc(fp)
  // terminate each line of board with null terminator for inner array
  // to know at end of each line: each line should end w/ a new line character

  if (fp == NULL)
  {
    return NULL;
  }
  struct game_state_t *game_state;
  unsigned int col_malloc = 20;

  game_state = malloc(sizeof(game_state_t));
  game_state->board = malloc(sizeof(char *));               // board (vertical column)
  game_state->board[0] = malloc(sizeof(char) * col_malloc); // set the first row memory space

  game_state->num_snakes = 0;
  game_state->snakes = NULL;

  // variables to keep track of col and row
  int code;
  unsigned int current_col = 0;
  unsigned int current_row = 0;

  while (feof(fp) == 0)
  {
    code = fgetc(fp);

    if (current_col >= col_malloc) // resizing the column malloc memory space
    {
      col_malloc = col_malloc * 2;
      game_state->board[current_row] = realloc(game_state->board[current_row], sizeof(char) * (col_malloc) + 1);
    }

    if (code == 10) // new line
    {
      set_board_at(game_state, current_row, current_col, '\0');

      current_row++;
      game_state->board = realloc(game_state->board, sizeof(char *) * (current_row + 1));

      col_malloc = 20;
      game_state->board[current_row] = malloc(sizeof(char) * (col_malloc + 1));
      current_col = 0;
    }

    else if (code == 35) // if its hashtag ya
    {
      set_board_at(game_state, current_row, current_col, '#');
      current_col++;
    }

    else if (code == 32) // if space detected
    {
      set_board_at(game_state, current_row, current_col, ' ');
      current_col++;
    }
    else if (is_snake((char)code))
    {
      set_board_at(game_state, current_row, current_col, (char)code);
      current_col++;
    }

    else if (code == 42)
    {
      set_board_at(game_state, current_row, current_col, '*');
      current_col++;
    }
  }
  game_state->num_rows = current_row;
  return game_state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.

  unsigned int tail_row = state->snakes[snum].tail_row;
  unsigned int tail_col = state->snakes[snum].tail_col;

  while (!is_head(get_board_at(state, tail_row, tail_col)))
  {
    char c = get_board_at(state, tail_row, tail_col);
    tail_row = get_next_row(tail_row, c);
    tail_col = get_next_col(tail_col, c);
  }

  state->snakes[snum].head_row = tail_row;
  state->snakes[snum].head_col = tail_col;

  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
  // TODO: Implement this function.
  state->num_snakes = 0;
  state->snakes = malloc(sizeof(snake_t)); // snake array initiated

  for (unsigned int i = 0; i < state->num_rows; i++)
  {
    for (unsigned int j = 0; j < strlen(state->board[i]); j++)
    {
      if (is_tail(get_board_at(state, i, j)))
      {
        state->snakes[state->num_snakes].tail_row = i;
        state->snakes[state->num_snakes].tail_col = j;
        find_head(state, state->num_snakes);

        state->snakes[state->num_snakes].live = true;
        state->num_snakes++;
        state->snakes = realloc(state->snakes, sizeof(snake_t) * (state->num_snakes + 1));
      }
    }
  }
  return state;
}
