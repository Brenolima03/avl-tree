// point.hpp
#ifndef POINT_HPP
#define POINT_HPP

#include <ostream>

// A custom type usable with AvlTree<T>. Just needs operator< (for
// ordering) and operator<< (for printing).
struct Point {
  int x = 0;
  int y = 0;

  Point() = default;
  Point(int x, int y) : x(x), y(y) {}
};

inline bool operator<(const Point& a, const Point& b) {
  if (a.x != b.x) return a.x < b.x;
  return a.y < b.y;
}

inline bool operator>(const Point& a, const Point& b) {
  if (a.x != b.x) return a.x > b.x;
  return a.y > b.y;
}

inline bool operator==(const Point& a, const Point& b) {
  return a.x == b.x && a.y == b.y;
}

inline std::ostream& operator<<(std::ostream& os, const Point& p) {
  return os << '(' << p.x << ',' << p.y << ')';
}

#endif
