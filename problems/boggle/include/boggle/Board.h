// Project
#include <boggle/Point.h>

// STL
#include <fstream>
#include <list>
#include <set>
#include <string>
#include <vector>

// boost
#include <boost/unordered_map.hpp>

namespace boggle {

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
  Board(const std::string & board_string);

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
  const char & letter(const Point & point) const;

  /** 
      \return string representation of the board.
  */
  std::string to_string() const;

  /**
     \param word a string representing the word you want to know about.
     \return true if `word` exists (i.e., is playable) in the boggle board,
     false otherwise.

     Note, this method populates a cache, which at the moment is unbound in how 
     many resources it can consume.

     Beware.
  */
  bool exists(const std::string & word) const;


  private:

  /**
     \param point the point you wish to convert to an index.
     \param board_length the length or width of one of your board's sides.
     \returns int the index location of `point` on the internal representation 
     of the board.

     Converts a Point into a board_index, where we assume the board is a single
     dimension vector of length `board_length * board_length`
  */
  static int board_index(const Point & point, const size_t board_length)
  {
    return point.x() + (point.y() * board_length);
  }

  /**
     \param board_index the internal board representation index you wish to
     convert to an point.
     \param board_length the length or width of one of your board's sides.
     \returns int the `point` equivalent of the internal board representation's
     index.
  */
  static Point point(const unsigned int board_index, const size_t board_length)
  {
    return Point((board_index%(board_length)), (board_index/(board_length)));
  }

  /**
     A class to encapsulate the state of the game for any given word.
  */
  class GameState {
    public:
    GameState(const size_t board_length);
    GameState(const GameState & state, const Point & point);

    void visit(const Point & point);

    bool visited(const unsigned int index) const { 
      return _previously_visited[index];
    }

    const std::vector<bool> & visited() const { return _previously_visited; }

    const Point & last_letter() const { return _last_char; }

    private:
    const size_t _board_length;
    Point _last_char;
    std::vector<bool> _previously_visited;
  };

  typedef boost::unordered_map<
    std::string, std::list<GameState> > GameStateCacheMap_t;

  /**
     \param word the word you wish to look for in the cache.
     \return an iterator pointing to a position in the cache representing the 
     largest substring of the `word` parameter.

     If no substrings are found, the cache is primed with the first letter of 
     the `word` parameter, and an iterator to primed key-value pair is returned.
  */
  GameStateCacheMap_t::iterator 
  find_sub_word_gamestate(const std::string & word) const;

  /**
     A cache of all the words we've seen and all the end GameStates for those 
     words; an association of words with a list of valid GameStates for said
     word. 

     If the associated list is empty, then that word can not be created on 
     `this` board.
  */
  mutable GameStateCacheMap_t _visited_cache;

  std::string _board;
  size_t _length;
  std::set<char> _letters;
};


/**
   Stream operator overload for a boggle::Board
*/
inline 
std::ostream&
operator<< (std::ostream& stream, const boggle::Board & board )
{
  stream << board.to_string();
  return stream;
}

}
