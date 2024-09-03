#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <thread>
#include <fstream>
#include <string>
#include <sstream>
#include "ScoreHandler.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{500 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  ScoreHandler score("Scores.txt");
  score.scorefileRead();
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Game game(kGridWidth, kGridHeight);
  game.Run(std::move(renderer), kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Player1 Score: " << game.GetPlayer1Score() << " Player2 Score: "<<game.GetPlayer2Score() << "\n";
  score.ScoreUpdate(game.GameStatus());
  return 0;
}