#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <mutex>
#include <condition_variable>
class Game;

class Controller {
 public:
  Controller(uint32_t up, uint32_t down, uint32_t right, uint32_t left, uint8_t id):
        _upbutton(up),_downbutton(down),_rightbutton(right),_leftbutton(left),_id(id){}
  void HandleInput(bool &running, Snake &snake, std::mutex &paustmtx, Game& game) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  uint32_t _upbutton;
  uint32_t _downbutton;
  uint32_t _rightbutton;
  uint32_t _leftbutton;
  uint8_t _id;
};

#endif