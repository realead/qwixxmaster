#pragma once

#include <utility>
#include <string>

const size_t COLOR_CNT=4;
enum Color {cRED=0, cYELLOW=1, cGREEN=2, cBLUE=3};
extern int CLOSING_NUMBERS[4];
std::string color2str(Color color);
bool str2color(const std::string &str, Color &color);
    
    
struct ColorState{
   unsigned int last;
   unsigned int cnt;
   ColorState(unsigned char last, unsigned char cnt); 
};   
    
class Evaluator;
class State{
  friend class Evaluator;
  private:
    unsigned char last[COLOR_CNT];
    unsigned char cnt[COLOR_CNT];
    unsigned char missed;
  public:
    State();
    ColorState get_color_state(Color color) const;
    int get_missed() const;
    
    //returns false if move impossible
    bool take(Color color, int number); 
    bool take_possible(Color color, int number) const;
    void add_miss();
    bool set(Color color, int last, int taken);
    bool set_missed(int missed);
    
    int score() const; 
    bool ended() const;
};


