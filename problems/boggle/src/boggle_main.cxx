// STL
#include <fstream>
#include <iostream>
#include <math.h> 
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// boost
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <boost/unordered_map.hpp>

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
class Point {
  public:
  Point(const int the_x, const int the_y) 
      : _x(the_x), _y(the_y) {}

  Point(const Point & point) 
      : _x(point.x()), _y(point.y()) {}

  Point & operator=(const Point &rhs) {
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
std::ostream&
operator<< (std::ostream& stream, const boggle::Point & point )
{
  stream << "(" << point.x() << ", " << point.y() << ")";
  return stream;
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

    // _board is guaranteed to be a factor of 2
    _length = (size_t)(sqrt(_board.size())); 
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
    if(point.x() > the_board_length-1) {
      ostringstream error;
      error << "`x` position is out of bounds. board is "
            << the_board_length << " wide and you requested the data at index `"
            << point.x() << "`.";
      throw std::out_of_range(error.str());
    }
    if(point.y() > the_board_length-1) {
      ostringstream error;
      error << "`y` position is out of bounds. board is "
            << the_board_length << " long and you requested the data at index `"
            << point.y() << "`.";
      throw std::out_of_range(error.str());
    }

    return _board[Board::board_index(point, the_board_length)];
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

  /**
     \param word a string representing the word you want to know about.
     \return true if `word` exists (i.e., is playable) in the boggle board,
     false otherwise.
  */
  bool
  exists(const string & word)
  {
    // If this isn't a word, nope
    if(word.empty())
      return false;

    // If the word contains letters that aren't on the board, nope.
    BOOST_FOREACH(const char letter, word) {
      if(!_letters.count(letter))
        return false;
    }

    GameStateCacheMap_t::iterator cache_itr(
        find_sub_word_gamestate(word));

    unsigned int sub_word_end_index = cache_itr->first.size();
    if(sub_word_end_index == word.size())
      return true;
    if(cache_itr->second.empty())
      return false;

    // For each additional subword that we don't have in the `_visited_cache`, 
    // see if it exists on the board, and if it does, add it to the cache.
    for(sub_word_end_index+=1; sub_word_end_index <= word.size(); ++sub_word_end_index) 
    {
      const string cached_subword = word.substr(0, sub_word_end_index-1);
      const string uncached_subword = word.substr(0, sub_word_end_index);

      for(unsigned int j = 0; j < _board.size(); ++j){

        // If this board letter isn't the one we need, then try again.
        if(_board[j] != uncached_subword[uncached_subword.size()-1]) 
          continue;

        BOOST_FOREACH(const GameState & state, _visited_cache[cached_subword]) {

          // If this board letter is already visited, then try again. 
          if(state.visited(j))
            continue;

          const Point current_letter_point = Board::point(j, length());

          // If the found letter isn't adjacent to the last one, then try again.
          if(!state.last_letter().adjacent(current_letter_point))
            continue;

          // We found a valid position
          GameState new_state(state);
          new_state.visit(current_letter_point);
          _visited_cache[uncached_subword].push_back(new_state);
        }
      }

      if(_visited_cache[uncached_subword].empty())
        return false;
    }

    return _visited_cache[word].size() > 0;
  }

  private:
  string _board;
  size_t _length;
  set<char> _letters;

  static
  int
  board_index(const Point & point, const size_t board_length)
  {
    return point.x() + (point.y() * board_length);
  }

  static
  Point 
  point(const unsigned int board_index, const size_t board_length)
  {
    return Point(
        (board_index%(board_length)),
        (board_index/(board_length)));
  }

  class GameState {
    public:
    GameState(const size_t board_length) : 
        _board_length(board_length),
        _last_char(-1, -1),
        _previously_visited(board_length * board_length, false) 
    {}

    void visit(const Point & point) 
    {
      _last_char = point;
      const int index = Board::board_index(point, _board_length);
      if(index < 0 || index >= (int)_previously_visited.size())
        throw out_of_range("Can't visit a point off the board");
      _previously_visited[index] = true;
    }

    bool visited(const unsigned int index) const { 
      return _previously_visited[index];
    }

    const vector<bool> & visited() const { return _previously_visited; }

    const Point & last_letter() const { return _last_char; }

    private:
    const size_t _board_length;
    Point _last_char;
    vector<bool> _previously_visited;
  };

  typedef boost::unordered_map<string, list<GameState> > GameStateCacheMap_t;

  GameStateCacheMap_t::iterator
  find_sub_word_gamestate(const string & word)
  {
    // Look in the cache for sub words we've encountered already
    for(unsigned int i = 0; i < word.size(); ++i)
    {
      GameStateCacheMap_t::iterator itr =
          _visited_cache.find(word.substr(0, word.size()-i));
      if(itr != _visited_cache.end()) 
        return itr;
    }

    // If we haven't encountered any portion of this word, prime the cache.
    list<GameState> states;
    for(unsigned int i = 0; i < _board.size(); ++i)
    {
      if(_board[i] == word[0]) {
        states.push_back(GameState(length()));
        states.back().visit(Board::point(i, length()));
      }
    }

    return _visited_cache.insert(make_pair(word.substr(0,1), states)).first;
  }

  GameStateCacheMap_t _visited_cache;
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
    const string filename = option_map["dictionary_file"].as<string>();
    ifstream in(filename.c_str());

    if(!in.is_open()) {
      cerr << "Couldn't open dictionary file: " << filename << endl;
      return -1;
    }

    while(in.good()) {
      string word;
      getline(in, word);
      if(board.exists(word) && word.size() >= 3)
        cout << word << endl;
    }
    cout << endl;
    in.close();
  }
}
