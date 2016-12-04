#pragma once

#include <utility>
#include <string>

const size_t COLOR_CNT=4;
enum Color {red=0, yellow=1, green=2, blue=3};
std::string color2str(Color color);
    
class State{
  private:
    unsigned char last[COLOR_CNT];
    unsigned char cnt[COLOR_CNT];
    unsigned char missed;
  public:
    State();
    typedef std::pair<unsigned int, unsigned int> ColorState;
    ColorState get_color_state(Color color) const;
    int get_missed() const;
    
};


