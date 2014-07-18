// Corresponding
#include <boggle/Board.h>

// STL
#include <math.h> 
#include <sstream>
#include <stdexcept>
#include <vector>

// boost
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>

using namespace std;

boggle::Board::Board(const string & board_string) 
    : _board(board_string)
{
  boost::algorithm::trim(_board);

  vector<string> rows;
  boost::split(rows, _board, boost::is_any_of("\n"));

  _board.clear();

  // Make sure each row is the same length.
  BOOST_FOREACH(const string & row, rows)
  {
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

const char & 
boggle::Board::letter(const Point & point) const
{
  if(point.x() > (int)length()-1) {
    ostringstream error;
    error << "`x` position is out of bounds. board is "
          << length() << " wide and you requested the data at index `"
          << point.x() << "`.";
    throw std::out_of_range(error.str());
  }
  if(point.y() > (int)length()-1) {
    ostringstream error;
    error << "`y` position is out of bounds. board is "
          << length() << " long and you requested the data at index `"
          << point.y() << "`.";
    throw std::out_of_range(error.str());
  }

  return _board[Board::board_index(point, length())];
}

string
boggle::Board::to_string() const 
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
boggle::Board::exists(const string & word)
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
  // If this word has no valid GameStates, it can't be formed, so... nope.
  if(cache_itr->second.empty())
    return false;
  // If the subword we've found is the same size as the word, it is the word,
  // and we know it has at least one valid state because we passed the above 
  // check, so YUUUUUUUUP.
  if(sub_word_end_index == word.size())
    return true;


  // For each additional subword that we don't have in the `_visited_cache`, 
  // see if it exists on the board, and if it does, add it to the cache.
  for(sub_word_end_index+=1; 
      sub_word_end_index <= word.size(); 
      ++sub_word_end_index)
  {
    const string cached_subword = word.substr(0, sub_word_end_index-1);
    const string uncached_subword = word.substr(0, sub_word_end_index);

    for(unsigned int j = 0; j < _board.size(); ++j){

      // If this board letter isn't the one we need, then try again.
      if(_board[j] != uncached_subword[uncached_subword.size()-1]) 
        continue;

      BOOST_FOREACH(const GameState & state, _visited_cache[cached_subword])
      {
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

boggle::Board::GameStateCacheMap_t::iterator
boggle::Board::find_sub_word_gamestate(const string & word)
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

boggle::Board::GameState::GameState(const size_t board_length) : 
    _board_length(board_length),
    _last_char(-1, -1),
    _previously_visited(board_length * board_length, false) 
{}

void 
boggle::Board::GameState::visit(const Point & point) 
{
  _last_char = point;
  const int index = Board::board_index(point, _board_length);
  if(index < 0 || index >= (int)_previously_visited.size())
    throw out_of_range("Can't visit a point off the board");
  _previously_visited[index] = true;
}
