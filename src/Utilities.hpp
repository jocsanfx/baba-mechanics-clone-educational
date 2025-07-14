#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Config.hpp"
#include <string>
#include <set>
#include <map>
#include <vector>

char getSymbolForEntity(const std::string& type);
std::string entityString(char c);
inline const std::map<char, char> relation {
  {'B', '*'},
  {'W', '#'},
  {'G', 'h'},
  {'F', '&'},
  {'R', '$'},
  {'A', '~'},
  {'L', '-'},
  {'C', '+'},
};

inline const std::map<char, std::string> action {
  {'N', "isWin"},
  {'D', "isLose"},
  {'H', "isLose"},
  {'K', "isLose"},
  {'Y', "isYou"},
  {'S', "isStop"},
  {'P', "isPush"},
  {'M', "isMelt"},
  {'|', "isBreak"},
};

inline const std::set<char> backgrounds{'~', 'Z', 'h', 'b', '#'};
inline const std::set<char> valid_objects{'*', '$', '+', '-', 'f', 'x'
  , 'y', 'z', '0'};

#endif
