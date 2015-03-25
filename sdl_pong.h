#pragma once

#define BALL_VELOCITY 10

typedef struct sdl_buffer {
  SDL_Texture *texture;
  SDL_Window *window;
  SDL_Renderer *renderer;
  void *pixels;
  int texture_width;
  int texture_height;
  int bytes_per_pixel;
} sdl_buffer;

typedef struct player_info {
  int x;
  int y;
  int dx;
  int dy;
  int width;
  int height;
  int score;

  bool last_touch;
} player_info;

// TODO have a generic "object" struct ??
typedef struct ball_info {
  int x;
  int y;
  int dx;
  int dy;
  int width;
  int height;
} ball_info;

typedef struct game_input {
  bool move_up;
  bool move_right;
  bool move_left;
  bool move_down;

  bool serve;
} game_input;

typedef struct game_state {
  bool running;
  player_info player1;
  player_info player2;
  ball_info ball;
  int court_width;
  int court_height;
} game_state;

static void draw_rect(int x1, int y1, int x2, int y2,
                      int8_t R, int8_t G, int8_t B);
static void clear_buffer();
