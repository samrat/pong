#include "SDL.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "sdl_pong.h"
#include "game.c"
#include "opponent.c"
#include "font.c"


static sdl_buffer backbuffer;
static game_state global_state;

static unsigned char *
get_pixel(int x, int y) {
  int width = backbuffer.texture_width;
  int height = backbuffer.texture_height;

  int pitch = width * backbuffer.bytes_per_pixel;
  uint8_t *row = (uint8_t *)(backbuffer.pixels +
                             (y * width * backbuffer.bytes_per_pixel) +
                             (x * backbuffer.bytes_per_pixel));

  unsigned char *pixel = (unsigned char *)row;

  return pixel;
}


static void
SDLResizeTexture(SDL_Renderer *renderer, int width, int height)
{
  if (backbuffer.pixels)
    {
      free(backbuffer.pixels);
    }
  if (backbuffer.texture)
    {
      SDL_DestroyTexture(backbuffer.texture);
    }

  backbuffer.texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         width,
                                         height);
  backbuffer.texture_width = width;
  backbuffer.texture_height = height;

  backbuffer.pixels = malloc(width * height * backbuffer.bytes_per_pixel);
}

static void
SDLUpdateWindow(SDL_Window *window, SDL_Renderer *renderer)
{

  SDL_UpdateTexture(backbuffer.texture,
                    0,
                    backbuffer.pixels,
                    backbuffer.texture_width * backbuffer.bytes_per_pixel);
  SDL_RenderCopy(renderer,
                 backbuffer.texture,
                 0,
                 0);

  SDL_RenderPresent(renderer);
}

static void
clear_buffer()
{
  int width = backbuffer.texture_width;
  int height = backbuffer.texture_height;

  int pitch = width * backbuffer.bytes_per_pixel;
  uint8_t *row = (uint8_t *)backbuffer.pixels;

  for (int Y = 0;
       Y < height;
       ++Y)
    {
      uint32_t *pixel = (uint32_t *)row;
      for (int X = 0;
           X < width;
           ++X)
        {
          *pixel++ = 0;
        }

      row += pitch;
    }
}


static void
handle_keyboard_input(SDL_Event *event, game_input *input1, game_input *input2) {
  SDL_Keycode key_code = event->key.keysym.sym;
  bool is_down = (event->key.state == SDL_PRESSED);
  bool was_down = false;

  if ((event->key.state == SDL_RELEASED) ||
      (event->key.repeat != 0))
    {
      was_down = true;
    }

  switch (key_code) {
  case SDLK_w: {
    if (is_down) {
      input1->move_up = true;
    }
  } break;
  case SDLK_s: {
    if (is_down) {
      input1->move_down = true;
    }
  } break;

  case SDLK_UP: {
    if (is_down) {
      input2->move_up = true;
    }
  } break;

  case SDLK_DOWN: {
    if (is_down) {
      input2->move_down = true;
    }
  } break;

  case SDLK_SPACE: {
    if (is_down) {
      input1->serve = true;
    }
  } break;

  case SDLK_l:
    {
      if (is_down)
        {
          // recording = true;
          // start_recording();
        }
    } break;
  case SDLK_o:
    {
      if (is_down)
        {
          /*
            recording = false;
            stop_recording();
            playing_back = true;
            start_playback();
          */
        }
    } break;
  }
}

static void
handle_event(SDL_Event *event, game_input *input1, game_input *input2)
{
  switch (event->type)
    {
    case SDL_QUIT:
      {
        printf("SDL_QUIT\n");
        global_state.running = false;
      } break;

    case SDL_WINDOWEVENT:
      {
        switch (event->window.event)
          {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            {
              SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
              SDL_Renderer *renderer = SDL_GetRenderer(window);

              printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n",
                     event->window.data1, event->window.data2);
              SDLResizeTexture(renderer, event->window.data1, event->window.data2);
            } break;

          case SDL_WINDOWEVENT_EXPOSED:
            {
              SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
              SDL_Renderer *renderer = SDL_GetRenderer(window);

              SDLUpdateWindow(window, renderer);

            } break;
          }
      } break;

    case SDL_KEYDOWN:
    case SDL_KEYUP:
      {
        handle_keyboard_input(event, input1, input2);
      }

    }
}


static void
draw_rect(int x1, int y1, int x2, int y2,
          int8_t R, int8_t G, int8_t B) {
  int width = backbuffer.texture_width;
  int height = backbuffer.texture_height;

  int pitch = width * backbuffer.bytes_per_pixel;
  uint8_t *row = (uint8_t *)(backbuffer.pixels +
                             (y1 * width * backbuffer.bytes_per_pixel) +
                             (x1 * backbuffer.bytes_per_pixel));

  for (int Y = y1;
       Y < y2;
       Y++)
    {
      uint32_t *pixel = (uint32_t *)row;

      for (int X = x1;
           X < x2;
           X++)
        {
          *pixel++ = (R << 16) | (G << 8) | B;
        }

      row += pitch;
    }
}


static float
SDLGetSecondsElapsed(uint64_t old_counter, uint64_t current_counter) {
  return ((float)(current_counter - old_counter) / (float)(SDL_GetPerformanceFrequency()));
}


int main(int argc, char **argv) {
  int game_update_hz = 30;
  float target_seconds_per_frame = 1.0f / (float)game_update_hz;

  FILE *font_file = init_font("UbuntuMono-B.ttf");
  SDL_Window *window;
  window = SDL_CreateWindow("Pong Manager",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            640,
                            480,
                            SDL_WINDOW_RESIZABLE);

  if (window) {
    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                0);

    if (renderer) {
        backbuffer.bytes_per_pixel = 4;
        backbuffer.window = window;
        backbuffer.renderer = renderer;
        global_state.running = true;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        SDLResizeTexture(renderer, width, height);

        global_state.court_width = width;
        global_state.court_height = height;

        /* Initialize players */
        global_state.player1.x = 5;
        global_state.player1.y = 20;
        global_state.player1.width = 5;
        global_state.player1.height = 80;

        global_state.player2.x = width-5;
        global_state.player2.y = 20;
        global_state.player2.width = 5;
        global_state.player2.height = 80;

        /* Initialize ball */
        global_state.ball.x = width / 2;
        global_state.ball.y = height / 2;
        global_state.ball.dx = -BALL_VELOCITY;
        global_state.ball.width = 8;
        global_state.ball.height = 8;


        uint64_t last_counter;
        while (global_state.running) {
          SDL_Event event;

          /* TODO store both inputs in a single struct */
          game_input input1 = {
            .move_up = false,
            .move_down = false,
            .move_left = false,
            .move_right = false,
          };

          game_input input2 = {
            .move_up = false,
            .move_down = false,
            .move_left = false,
            .move_right = false,
          };

          while (SDL_PollEvent(&event)) {
            handle_event(&event, &input1, &input2);
          }

          opponent_move(&input2, &global_state.player2, &global_state.ball);
          // opponent_move(&input1, &global_state.player1, &global_state.ball);

          game_update_and_render(&global_state, &input1, &input2);

          SDLUpdateWindow(window, renderer);

          while (SDLGetSecondsElapsed(last_counter, SDL_GetPerformanceCounter()) <
                 target_seconds_per_frame) {
            SDL_Delay((target_seconds_per_frame -
                       SDLGetSecondsElapsed(last_counter, SDL_GetPerformanceCounter())) * 1000);
          }

          // printf("MPF: %f\n", SDLGetSecondsElapsed(last_counter, SDL_GetPerformanceCounter())*1000);
          last_counter = SDL_GetPerformanceCounter();
        }

      }
  }

  free(backbuffer.pixels);
  fclose(font_file);
  SDL_Quit();
  return 0;
}

static void
draw_text(int x, int y, unsigned char *text) {
  font_draw_text(&backbuffer, x, y, text);
}
