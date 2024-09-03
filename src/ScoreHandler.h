#ifndef SCOREHANDLER_H
#define SCOREHANDLER_H
#include <fstream>
#include <string>
#include <sstream>


class ScoreHandler{
    public:
    ScoreHandler(std::string filename);
    ~ScoreHandler();
    void ScoreUpdate(int _newScore);
    void ScorefileUpdate(int _newhighestscore);
    void scorefileRead(void);

    private:
        int highest_score{0};
        std::string _scorefilename;
};







#endif /*SCOREHANDLER_H*/