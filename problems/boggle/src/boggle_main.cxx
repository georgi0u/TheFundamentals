// STL
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <vector>

// boost
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

using namespace std;


namespace {

//////////////////////
// Helper Functions //
//////////////////////

/**
   \param filename a path to a file
   \return string containing the contents of the file found at the `filename` 
   param
*/
string slurp(const string & filename) 
{
  ifstream in;
  in.open(filename.c_str());
  stringstream sstr;
  sstr << in.rdbuf();
  in.close();
  return sstr.str();
}

} // </namespace>


namespace boggle {

/**
   Simple struct for housing an x,y coordinate pair.
*/
struct Point {
  Point(const int the_x, const int the_y) 
      : x(the_x), y(the_y) {}

  Point(const Point & point) 
      : x(point.x), y(point.y) {}

  Point operator=(const Point &rhs) {
      return Point(rhs.x, rhs.y);
  }

  bool operator==(const Point & rhs)
  { 
    return x == rhs.x && y == rhs.y; 
  }

  bool operator!=(const Point & rhs)
  { 
    return !(*this == rhs);
  }

  const int x, y;

  private:
  Point() : x(0), y(0) {}
};

/**
   Stream operator overload for a boggle::Point
*/
std::ostream&
operator<< (std::ostream& stream, const boggle::Point & point )
{
  stream << "(" << point.x << ", " << point.y << ")";
  return stream;
}


vector<Point>
get_surrounding_points(const Point & point, const int min, const int max)
{
  vector<Point> surrounding_points = 
      boost::assign::list_of
      (Point(point.x-1, point.y+1))   // left and up
      (Point(point.x-1, point.y  ))   // left 
      (Point(point.x-1, point.y-1))   // left and down
      (Point(point.x,   point.y-1))   // down
      (Point(point.x+1, point.y-1))   // right and down
      (Point(point.x+1, point.y  ))   // right
      (Point(point.x+1, point.y+1))   // right and up
      (Point(point.x+1, point.y+1));  // up

  vector<Point> valid_surrounding_points;
  BOOST_FOREACH(const Point & point, surrounding_points){
    if(point.x < min || point.x >= max)
       continue;
    if(point.y < min || point.y >= max)
      continue;
    valid_surrounding_points.push_back(point);
  }

  return valid_surrounding_points;
}


/**
   A class for representing a boggle board and encapsulating some of the logic
   involved with playing the game.
*/
class Board {
  public:

  /**
     Constructor which initializes the boggle::board from a string of newline
     separated rows of letters. Expects the board to be N x N, meaning its width
     must be the same length as its height.
  */
  Board(const string & board_string) 
      : _board(board_string)
  {}

  /**
     \return int describing the length (and thus width) of the NxN 
     boggle board.
   */
  size_t length() const { return _board.size(); }

  /**
     \param x the x position of the letter you want to return
     \param y the y position of the letter you want to return
     \return char the letter found at position (x,y).

     The board is considered to be in the first quadrant with its bottom left 
     corner rooted at the origin.
  */
  const char & 
  letter(const Point & point) const
  {
    const int the_board_length = length();
    if(point.x > the_board_length-1) {
      ostringstream error;
      error << "`x` position is out of bounds. board is "
            << the_board_length << " wide and you requested the data at index `"
            << point.x << "`.";
      throw std::out_of_range(error.str());
    }
    if(point.y > the_board_length-1) {
      ostringstream error;
      error << "`y` position is out of bounds. board is "
            << the_board_length << " long and you requested the data at index `"
            << point.y << "`.";
      throw std::out_of_range(error.str());
    }

    return _board[(the_board_length-1)-point.y][point.x];
  }

  /** 
      \return string representation of the board. 
  */
  string
  to_string() const 
  {
    return _board;
  }

  bool
  exists(const string & word)
  {

  }

  private:
  const string _board;
};


/**
   Stream operator overload for a boggle::Board
*/
std::ostream&
operator<< (std::ostream& stream, const boggle::Board & board )
{
  stream << board.to_string();
  return stream;
}

} // </boggle>



int main(int argc, char* argv[])
{

  /////////////////////////
  // Get Program Options //
  /////////////////////////

  boost::program_options::variables_map option_map;
  try {
    boost::program_options::options_description description(
        "Boggle game options");

    description.add_options()
        ("help", "produce help message")

        ("board_file",
         boost::program_options::value<string>()->required(),
         "the path of the file containing the board.")

        ("dictionary_file",
         boost::program_options::value<string>()->required(),
         "the path of the file containing the words defined as valid.");

    boost::program_options::positional_options_description postional_arguments;
    postional_arguments.add("board_file", 1);
    postional_arguments.add("dictionary_file", 2);

    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
        .options(description)
        .positional(postional_arguments)
        .run(), 
        option_map);

    if(option_map.count("help")) {
      cout << description << endl;
      return 1;
    }

    boost::program_options::notify(option_map);
  }
  catch(const boost::program_options::error & error) {
    cerr << error.what() << endl;
    return 1;
  }


  ////////////////////
  // Solve The Game //
  ////////////////////
  {
    using boggle::Board;
    using boggle::Point;

    // Construct the board
    Board board(slurp(option_map["board_file"].as<string>()));

    // Construct the dictionary
    vector<string> dictionary;
    {
      const string dictionary_string = 
          slurp(option_map["dictionary_file"].as<string>());
      boost::split(dictionary, dictionary_string, boost::is_any_of("\n"));
    }

    cout << board << endl;
    cout << endl;
    BOOST_FOREACH(const string & word, board.words())
        cout << word << endl;

  }



}
