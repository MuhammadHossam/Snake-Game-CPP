#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);
  

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

Renderer::Renderer(const Renderer &source):screen_width(source.screen_width),
      screen_height(source.screen_height),
      grid_width(source.grid_width),
      grid_height(source.grid_height) 
{
  std::cout << "Copy constructor is called"<< std::endl;
  sdl_window = source.sdl_window;
  menu_window = source.menu_window;
  sdl_renderer = source.sdl_renderer;
  menu_renderer = source.menu_renderer;

  std::cout<<"Copy constructor"<<std::endl;
}

Renderer &Renderer::operator=(const Renderer &source)
{
  std::cout << "Copy assignment constructor is called"<< std::endl;
  if(this == &source)
    return *this;
  sdl_window = source.sdl_window;
  menu_window = source.menu_window;
  sdl_renderer = source.sdl_renderer;
  menu_renderer = source.menu_renderer;
  screen_width = source.screen_width;
  screen_height = source.screen_height;
  grid_height = source.grid_height;
  grid_width = source.grid_width;
  return *this;
}

Renderer::Renderer(Renderer &&source)
{
  std::cout << "Move constructor is called"<< std::endl;
  sdl_window = source.sdl_window;
  menu_window = source.menu_window;
  sdl_renderer = source.sdl_renderer;
  menu_renderer = source.menu_renderer;
  screen_width = source.screen_width;
  screen_height = source.screen_height;
  grid_height = source.grid_height;
  grid_width = source.grid_width;

  source.sdl_window = nullptr;
  source.menu_window = nullptr;
  source.sdl_renderer = nullptr;
  source.menu_renderer = nullptr;
  source.screen_height = 0;
  source.screen_width = 0;
  source.grid_height = 0;
  source.grid_width = 0;
}

Renderer &Renderer::operator=(Renderer &&source)
{
  std::cout << "Move assignment constructor is called"<< std::endl;
  if(this == &source)
    return *this;
  sdl_window = source.sdl_window;
  menu_window = source.menu_window;
  sdl_renderer = source.sdl_renderer;
  menu_renderer = source.menu_renderer;
  screen_width = source.screen_width;
  screen_height = source.screen_height;
  grid_height = source.grid_height;
  grid_width = source.grid_width;

  source.sdl_window = nullptr;
  source.menu_window = nullptr;
  source.sdl_renderer = nullptr;
  source.menu_renderer = nullptr;
  source.screen_height = 0;
  source.screen_width = 0;
  source.grid_height = 0;
  source.grid_width = 0;
  return *this;

}

void Renderer::Render(Snake const &snake,Snake const &snake1, SDL_Point const &food, SDL_Point const &poison, SDL_Point const &shrinkfood, int game) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render poison
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  block.x = poison.x * block.w;
  block.y = poison.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render shrink food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0xFF, 0xFF);
  block.x = shrinkfood.x * block.w;
  block.y = shrinkfood.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);


  if(game == 2)
  {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake1.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake1.head_x) * block.w;
  block.y = static_cast<int>(snake1.head_y) * block.h;
  if (snake1.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x7A, 0xCC, 0x00, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
 
  }
   
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score1, int score2, int fps, int gamemode) {
  std::string title;
  if (gamemode == 1)
    title = "Snake1 Score: " + std::to_string(score1) + " FPS: " + std::to_string(fps);
  else if (gamemode == 2)
    title = "Snake1 Score: " + std::to_string(score1) + " Snake2 Score " + std::to_string(score2) + " FPS: " + std::to_string(fps);

  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::UpdatePauseMessage(void) {
  std::string title{"Game is paused!, press ESCAPE button to resume the game. "};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}