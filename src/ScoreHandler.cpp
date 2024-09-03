#include "ScoreHandler.h"
#include <iostream>


/*Score handler class constructor*/
ScoreHandler::ScoreHandler(std::string filename):_scorefilename(filename){}


/*Score handler class destructor*/
ScoreHandler::~ScoreHandler(){}

/*new Score update*/
void ScoreHandler::ScoreUpdate(int _newScore)
{
    if(_newScore > highest_score)
    {
        highest_score = _newScore;
    }
    ScorefileUpdate(highest_score);
}

void ScoreHandler::ScorefileUpdate(int _newhighestscore)
{
    std::string result;
    result = "Highest Score = " + std::to_string(_newhighestscore);
    std::cout<<result<<std::endl;
    std::ofstream outfile(_scorefilename);
    if(outfile.is_open())
        outfile<<result;
    
    outfile.close();
}

void ScoreHandler::scorefileRead(void){
  std::ifstream scoresfile(_scorefilename);
  if(!scoresfile)
  {
    std::cout<<"Unable to open scores file"<<std::endl;
  }
  else
  {
    std::string line;
    int number;
    std::string temp;
    std::getline(scoresfile,line);
    std::stringstream iss;
    iss << line;
    
    while(!iss.eof())
    {
      iss >> temp;
      if(std::stringstream(temp) >> number)
      {
        highest_score = number;
      }
     temp = "";
    } 
  }

}

