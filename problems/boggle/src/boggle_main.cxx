// Project
#include <boggle/Board.h>

// STL
#include <fstream>
#include <iostream>
#include <string>

// boost
#include <boost/program_options.hpp>

using namespace std;


namespace {

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

}

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

    // Open the dictionary file
    const string filename = option_map["dictionary_file"].as<string>();
    ifstream in(filename.c_str());
    if(!in.is_open()) {
      cerr << "Couldn't open dictionary file: " << filename << endl;
      return -1;
    }

    // Check each word in the dictionary.
    while(in.good()) {
      string word;
      getline(in, word);

      // The reason we even bother checking words under 3 characters, even 
      // though they're not allowed by the rules of the game, is to prime the
      // cache. This assumes our dictionary is such that we have a lot of common
      // prefixes, which may or may not be true.
      if(board.exists(word) && word.size() >= 3)
        cout << word << endl;
    }
    cout << endl;

    in.close();
  }
}
