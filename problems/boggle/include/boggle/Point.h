// STL
#include <fstream>

namespace boggle {

/**
   Simple class for housing an x,y coordinate pair.
*/
class Point {
  public:
  Point(const int the_x, const int the_y) 
      : _x(the_x), _y(the_y) {}

  Point(const Point & point) 
      : _x(point.x()), _y(point.y()) {}

  Point & operator=(const Point &rhs)
  {
    _x = rhs._x;
    _y = rhs._y;
    return *this;
  }

  bool operator==(const Point & rhs)
  { 
    return _x == rhs._x && _y == rhs._y; 
  }

  bool operator!=(const Point & rhs)
  { 
    return !(*this == rhs);
  }

  bool adjacent(const Point & other) const
  {
    const unsigned int delta_x = abs(x() - other.x());
    const unsigned int delta_y = abs(y() - other.y());
    return ((delta_x != 0 || delta_y != 0) && delta_x <= 1 && delta_y <= 1);
  }

  int x() const { return _x; }
  int y() const { return _y; }

  private:
  int _x, _y;
  Point() : _x(0), _y(0) {}
};


/**
   Stream operator overload for a boggle::Point
*/
inline
std::ostream&
operator<< (std::ostream& stream, const boggle::Point & point )
{
  stream << "(" << point.x() << ", " << point.y() << ")";
  return stream;
}

}
