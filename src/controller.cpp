#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"
#include "game.h"
#include <mutex>


void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  //std::cout<<"Snake moved to " << int(snake.direction) <<std::endl;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake, std::mutex &paustmtx, Game& game) const {
  
  SDL_Event e;
  
  while(running)
  {
  if (SDL_WaitEvent(&e)) {
    //std::cout <<"Hello from handle input task # "<<_id <<std::endl;
   // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      if(e.key.keysym.sym == _upbutton)
        ChangeDirection(snake, Snake::Direction::kUp,Snake::Direction::kDown);
      else if (e.key.keysym.sym == _downbutton)
        ChangeDirection(snake, Snake::Direction::kDown,Snake::Direction::kUp);
      else if(e.key.keysym.sym == _leftbutton)
        ChangeDirection(snake, Snake::Direction::kLeft,Snake::Direction::kRight);
      else if(e.key.keysym.sym == _rightbutton)
        ChangeDirection(snake, Snake::Direction::kRight,Snake::Direction::kLeft);
      else if(e.key.keysym.sym == SDLK_ESCAPE)
      {
        std::unique_lock<std::mutex> lock(paustmtx);
       std::cout<<"Task# "<<_id<<"Owns the mutex"<<std::endl;
        game.GamePauseRequest();
      }
       
    }
  }
  }
}