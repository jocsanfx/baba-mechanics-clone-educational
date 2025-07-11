#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>
#include <map>
#include <vector>

char getSymbolForEntity(const std::string &type);
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

#endif