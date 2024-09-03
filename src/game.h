#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <condition_variable>

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Renderer renderer,
           std::size_t target_frame_duration);
  int GetPlayer1Score() const;
  int GetPlayer2Score() const;
  int GetSize() const;
  void GamePauseRequest(void);
  void PoisonEffect(void);
  int GameStatus(void);
 private:
  std::shared_ptr<Snake> snake;
  std::shared_ptr<Snake> snake1;
  SDL_Point food;
  SDL_Point poison;
  SDL_Point shrinkfood;
  bool _pause;
  std::vector <std::thread> controllerthreads;
  bool _running;
  std::mutex pausemtx;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int Player1score{0};
  int Player2score{0};

  void PlaceFood();
  void PlacePoison();
  void PlaceShrinkFood();
  void Update(Renderer& renderer);
};

#endif