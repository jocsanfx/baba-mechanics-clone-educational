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
  {'S', '+'},
};

inline const std::vector<char> action {
  'W', 'D', 'Y', 'S', 'P', 'K', 'M'
};

#endif