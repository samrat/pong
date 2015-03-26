#include "sdl_pong.h"

static void
opponent_move(game_input *opponent_input, player_info *opponent, ball_info *ball) {
  int ball_dir = ball->dx;
  int ball_dist = ball->y - opponent->y;

  if (ball_dir > 0) {
    if (ball_dist < 0) {
      opponent_input->move_up = true;
    }
    else if (ball_dist > 0) {
      opponent_input->move_down = true;
    }
  }
}
