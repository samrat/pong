#include "sdl_pong.h"

static void
draw_paddle(player_info player) {
  int x = player.x;
  int y = player.y;
  int half_width = player.width / 2;
  int half_height = player.height / 2;

  draw_rect(x - half_width, y - half_height,
            x + half_width, y + half_height,
            255, 0, 0);
}

static void
draw_ball(ball_info ball) {
  int x = ball.x;
  int y = ball.y;
  int half_width = ball.width / 2;
  int half_height = ball.height / 2;

  draw_rect(x-half_width, y-half_height,
            x+half_width, y+half_height,
            0, 255, 0);
}

static bool
ball_paddle_collision(ball_info ball, player_info paddle) {
  int x_diff_abs = abs(ball.x - paddle.x);
  int y_diff_abs = abs(ball.y - paddle.y);

  if ((x_diff_abs < (ball.width + paddle.width)/2) &&
      (y_diff_abs < (ball.height + paddle.height)/2)) {
    return true;
  }
  else {
    return false;
  }
}

static void
reset_ball(ball_info *ball, game_state *global_state) {
  ball->x = global_state->court_width / 2;
  ball->y = global_state->court_height / 2;
  ball->dx = 0;
  ball->dy = 0;
}

static bool
ball_out_court(ball_info ball, game_state *global_state) {
  int width = global_state->court_width;
  int height = global_state->court_height;

  if (ball.x < 0) {
    global_state->player2.score++;
    return true;
  }
  else if (ball.x >= width) {
    global_state->player1.score++;
    return true;
  }

  if ((ball.y < 0) || (ball.y >= height)) {
    if (global_state->player1.last_touch) {
      global_state->player2.score++;
    }
    else if (global_state->player2.last_touch) {
      global_state->player1.score++;
    }

    return true;
  }

  return false;

}


static void
update_positions(game_state *global_state) {
  global_state->player1.y += global_state->player1.dy;
  global_state->player2.y += global_state->player2.dy;

  global_state->ball.y += global_state->ball.dy;
  global_state->ball.x += global_state->ball.dx;
}

#define PADDLE_VELOCITY 10
static void
update_velocities(game_state *global_state, game_input *input1, game_input *input2) {

  if ((input1->serve) && (global_state->ball.dx == 0) &&
      (global_state->ball.dy == 0)) {
    int r = rand() % 5;

    global_state->ball.dx = (r > 2) ? (-1 * BALL_VELOCITY) : BALL_VELOCITY;
    global_state->ball.dy = r / 2;
  }


  if (ball_paddle_collision((global_state->ball), (global_state->player1))) {
    global_state->player1.last_touch = true;
    global_state->player2.last_touch = false;

    global_state->ball.dx = -global_state->ball.dx;
    global_state->ball.dy = global_state->ball.dy + (global_state->player1.dy/3);
  }

  if (ball_paddle_collision((global_state->ball), (global_state->player2))) {
    global_state->player1.last_touch = false;
    global_state->player2.last_touch = true;

    global_state->ball.dx = -global_state->ball.dx;
    global_state->ball.dy = global_state->ball.dy + (global_state->player2.dy/3);
  }

  global_state->player1.dx = 0;
  global_state->player1.dy = 0;

  global_state->player2.dx = 0;
  global_state->player2.dy = 0;

  /* move player 1 */
  if (input1->move_up) {
    // down is growing y
    global_state->player1.dy = -PADDLE_VELOCITY;
  }
  else if (input1->move_down) {
    global_state->player1.dy = PADDLE_VELOCITY;
  }

  /* move player 2 */
  if (input2->move_up) {
    global_state->player2.dy = -PADDLE_VELOCITY;
  }
  else if (input2->move_down) {
    global_state->player2.dy = PADDLE_VELOCITY;
  }

}

static void
game_update_and_render(game_state *global_state, game_input *input1,
                       game_input *input2) {
  if (ball_out_court(global_state->ball, global_state)) {
    reset_ball(&global_state->ball, global_state);
  }

  update_velocities(global_state, input1, input2);
  update_positions(global_state);

  clear_buffer();
  draw_paddle(global_state->player1);
  draw_paddle(global_state->player2);
  draw_ball(global_state->ball);

}
