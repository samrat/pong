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
} game_input;

typedef struct game_state {
  bool running;
  player_info player1;
  player_info player2;
  ball_info ball;
} game_state;
