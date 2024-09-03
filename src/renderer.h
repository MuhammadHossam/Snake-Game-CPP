#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();
  Renderer(const Renderer &source);
  Renderer &operator=(const Renderer &source);
  Renderer(Renderer &&source);
  Renderer &operator=(Renderer &&source);
  void Render(Snake const &snake, Snake const &snake1, SDL_Point const &food, SDL_Point const &poison, SDL_Point const &shrinkfood);
  void UpdateWindowTitle(int score,int score2, int fps);
  void UpdatePauseMessage(void); 
 private:
  SDL_Window *sdl_window;
  SDL_Window *menu_window;
  SDL_Renderer *sdl_renderer;
  SDL_Renderer *menu_renderer;

  std::size_t screen_width;
  std::size_t screen_height;
  std::size_t grid_width;
  std::size_t grid_height;
};

#endif