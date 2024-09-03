#include "game.h"
#include <iostream>
#include "SDL.h"
#include <thread>
#include <algorithm>
#include <future>
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(new Snake(grid_width, grid_height)),
      snake1(new Snake(grid_width, grid_height)),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      _pause(false),
      _running(true) {
  snake->SetHeadPosition(random_w(engine), random_h(engine));
  snake1->SetHeadPosition(random_w(engine), random_h(engine));
  PlaceFood();
}

Game::~Game(){
  std::cout<<"Game Destructor"<<std::endl;
  std::for_each(controllerthreads.begin(), controllerthreads.end(), [](std::thread &t) {
        t.join();
    });
}
void Game::Run( Renderer renderer,
               std::size_t target_frame_duration, int &gamemode) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  Controller player1(SDLK_UP,SDLK_DOWN,SDLK_RIGHT,SDLK_LEFT,1);
  Controller player2(SDLK_w,SDLK_s,SDLK_d,SDLK_a,2);
  controllerthreads.emplace_back(std::thread(&Controller::HandleInput,player1,std::ref(_running),std::ref(*snake.get()),
    std::ref(this->pausemtx),std::ref(*this)));
  _gamemode = gamemode;
  if(gamemode == 2)
  {
    controllerthreads.emplace_back(std::thread(&Controller::HandleInput,player2,std::ref(_running),std::ref(*snake1.get()),
       std::ref(this->pausemtx), std::ref(*this)));
  }
  
  while (_running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //std::cout<<"Hello Main Task"<<std::endl;
    frame_start = SDL_GetTicks();    
    // Input, Update, Render - the main game loop.
    Update(renderer);
    renderer.Render(*snake.get(),*snake1.get(),food, poison, shrinkfood,_gamemode);
    frame_end = SDL_GetTicks();
    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(Player1score,Player2score, frame_count, _gamemode);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

}


void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlacePoison() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      poison.x = x;
      poison.y = y;
      return;
    }
  }
}
void Game::PlaceShrinkFood(){
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake->SnakeCell(x, y)) {
      shrinkfood.x = x;
      shrinkfood.y = y;
      return;
    }
  }
}

void Game::Update(Renderer& renderer) {
  static int posionupdate = SDL_GetTicks();
  static int shrinkupdate = SDL_GetTicks();
  static int snakepoisonedUpdate = 0;
  static int snake1poisonUpdate = 0;
  std::unique_lock<std::mutex> lock(this->pausemtx);
  if(_pause == true)
  {
    renderer.UpdatePauseMessage();
    return;
  }
  
  if (!snake->alive || !snake1->alive)
  {
    _running = false;
    return;
  } 


  if(snake->poisoned == false)
    snake->Update();
  //snake.Update();
  if(snake1->poisoned == false && (_gamemode == 2))
  {
     snake1->Update();
  }
   
  
  if((SDL_GetTicks() - posionupdate) > 5000)
  {
    PlacePoison();
    posionupdate = SDL_GetTicks();
  }
  if((SDL_GetTicks() - shrinkupdate) > 2000)
  {
    PlaceShrinkFood();
    shrinkupdate = SDL_GetTicks();
  }
  
  int new_x = static_cast<int>(snake->head_x);
  int new_y = static_cast<int>(snake->head_y);
  int new1_x = static_cast<int>(snake1->head_x);
  int new1_y = static_cast<int>(snake1->head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    Player1score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake->GrowBody();
    snake->speed += 0.02;
  }
  else if((poison.x == new_x && poison.y == new_y) && snake->poisoned == false)
  {
    snake->poisoned = true;
    snakepoisonedUpdate = SDL_GetTicks();
    PlacePoison();
  }
  else if(shrinkfood.x == new_x && shrinkfood.y == new_y)
  {
    //should be shrinking.
    Player1score--;
    snake->ShrinkBody();
    PlaceShrinkFood();
  }
  else if(food.x == new1_x && food.y == new1_y && (_gamemode == 2))
  {
    Player2score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake1->GrowBody();
    snake1->speed += 0.02;
  }
  else if(poison.x == new1_x && poison.y == new1_y && (_gamemode == 2))
  {
   // std::thread poisonthread(&Game::PoisonEffect, this);
     snake1->poisoned = true;
     snake1poisonUpdate = SDL_GetTicks();
   // poisonthread.join();
    PlacePoison();
  }
  else if(shrinkfood.x == new1_x && shrinkfood.y == new1_y && (_gamemode == 2))
  {
//should be shrinking.
    Player2score--;
    snake1->ShrinkBody();
    PlaceShrinkFood();
  }
  else if(new1_x == new_x && new1_y == new_y && (_gamemode == 2))
  {
    snake->alive = false;
    snake1->alive = false;
  }

  if((SDL_GetTicks() - snakepoisonedUpdate )> 3000)
  { 
    snake->poisoned = false;
  }
  if((SDL_GetTicks() - snake1poisonUpdate )> 3000)
  { 
    snake1->poisoned = false;
  }
}

int Game::GetPlayer1Score() const { return Player1score; }
int Game::GetPlayer2Score() const { return Player2score;}
int Game::GetSize() const { return snake->size; }
void Game::GamePauseRequest(void){_pause ^= true;}
int Game::GameStatus(void){
  if(!snake->alive && !snake1->alive)
  {
    std::cout<< "Both players lose"<< std::endl;
    return 0;
  }
  else if (!snake->alive)
  {
    std::cout << "Player 2 wins" << std::endl;
    return GetPlayer2Score();
  }
  else if(!snake1->alive)
  {
    std::cout << "Player 1 wins" << std::endl;
    return GetPlayer1Score();
  } 
  std::cout << "Game Ended with exit button" << std::endl;
    return 0;
}
void Game::PoisonEffect(void)
{
  std::cout << "Thread started due to eating a poison"<<std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}