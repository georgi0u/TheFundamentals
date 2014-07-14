// STL
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <vector>
#include <math.h> 

// boost
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
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
  {
    boost::algorithm::trim(_board);

    vector<string> rows;
    boost::split(rows, _board, boost::is_any_of("\n"));

    _board.clear();

    // Make sure each row is the same length.
    BOOST_FOREACH(const string & row, rows) {
      if(row.size() != rows.back().size()) {
        throw runtime_error("You have inconsistent row lengths.");
      }
      _board += row;
    }

    // Make sure the number of columns matches the length of each row.
    if(rows.size() != rows.back().size()) {
      throw runtime_error("You have inconsistent row lengths.");
    }

    _length = sqrt(_board.size());

    _letters.insert(_board.begin(), _board.end());
  }

  /**
     \return int describing the length (and thus width) of the NxN 
     boggle board.
   */
  size_t length() const { return _length; }

  /**
     \param x the x position of the letter you want to return
     \param y the y position of the letter you want to return
     \return char the letter found at position (x,y).

     The board's top left corner is at (0,0) and its bottom right corner is at
     (N-1,N-1), where N is the length/width of the board.
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

    return _board[board_index(point, the_board_length)];
  }

  static
  unsigned int
  board_index(const Point & point, const size_t board_length)
  {
    return point.x + (point.y * the_board_length);
  }

  /** 
      \return string representation of the board. 
  */
  string
  to_string() const 
  {
    ostringstream str;

    for(unsigned int y = 0; y < length(); ++y) {
      for(unsigned int x = 0; x < length(); ++x) {
        str << letter(Point(x, y)) << " ";
      }
      str << endl;
    }
    str << endl;

    return str.str();
  }

  bool
  exists(const string & word)
  {
    // If this isn't a word, nope
    if(word.empty())
      return false;

    // If the word contains letters that aren't on the board, nope.
    vector<char> extra_letters;
    set_difference(
        word.begin(), word.end(), _letters.begin(), _letters.end()
        extra_letters.begin());
    if(!extra_letters.empty())
      return false;

    string word_to_find = word;
    boost::scoped_ptr<GameState> state;

    for(unsigned int i = 0; i < word_to_find.legnth(); ++i)
    {
      VisitedCacheMap_t::const_iterator itr = 
          _visited_cache.find(word.substr(0,word.legnth()-i));

      if(itr != _visited_cache.end()) {
        state = make_shared<GameState>(itr->second);
        break;
      }
    }

    if(state == NULL)
      state = make_shared<GameState>();
  }

  private:
  string _board;
  size_t _length;
  std::set<char> _letters;

  class GameState {
    public:
    GameState(const Point & last_char, const size_t board_length) : 
        _board_length(board_length),
        _last_char(point), 
        _previously_visited(board_length*board_length, false) 
    {
      visit(last_char);
    }

    void visit(const Point & point) {
      _last_char = point;
      const size_t index = board_index(point);
      if(index < 0 || index >= _previously_visited.size())
        throw out_of_range("Can't visit a point off the board");
      _previously_visited[index] = true;
    }

    private:
    const size_t board_length;
    Point _last_char;
    vector<bool> _previously_visited;
  };

  typedef boost::unordered_map<string, GameState > GameStateCacheMap_t;

  VisitedCacheMap_t _visited_cache;


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

  }



}
