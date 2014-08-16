#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <queue>

using namespace std;
using namespace boost;

/**
   Kind of a gross implementation, but I wanted to see if I could figure this 
   out. The question goes, "Given a small amount of memory and a large amount of
   data on disk (i.e. more data than can fit in memory at any one time) find the
   duplicates within the data.

   In this case, I've made the assumption that the data on disk are ints written
   to a file.

   The solution below loads the data into memory in small chunks, sorts 
   those small chunks and then writes them back to disk to temporary files.

   Once all the data has been sorted into temporary files, we merge pairs
   of the smaller file into single, larger files. Once we've done this merge 
   across all small files, we're left with one single large sorted file, in 
   which duplicate data entries are next to one another.
*/


void getline(ifstream && input, string & output)
{
  getline(input,output);
  boost::trim(output);
} 

/**
   Assumes files are named 1, 2, 3... `flushed_file_count`, merges said files,
   and then writes the final file back to `filename`.
 */
void merge_files(const int flushed_file_count, const string & filename)
{
  // Figure out initial set of files to merge and put their names in a queue.
  queue<string> files_to_merge;
  for(int i = 1; i <= flushed_file_count; ++i)
    files_to_merge.push(lexical_cast<string>(i));

  int count_to_merge = 0; // Count to determine the name of the new files.

  // While the queue still has items to merge in it...
  while(files_to_merge.size() > 1) 
  {
    // Get the two files to merge off the queue.
    const string file_1_filename = files_to_merge.front();
    ifstream file_1(file_1_filename.c_str());
    files_to_merge.pop();
    const string file_2_filename = files_to_merge.front();
    ifstream file_2(file_2_filename.c_str());
    files_to_merge.pop();

    // Create a new file to merge the two into
    const string merge_filename = 
        "merge_" + lexical_cast<string>(++count_to_merge);
    ofstream merge(merge_filename.c_str());

    // If there's data in both files, figure out which comes first and advance
    // in the respective file.
    string line_1, line_2;
    getline(file_1, line_1);
    getline(file_2, line_2);

    while(!line_1.empty() && !line_2.empty())
    {
      const int
          line_1_int = lexical_cast<int>(line_1), 
          line_2_int = lexical_cast<int>(line_2);

      if(line_1_int < line_2_int) {
        merge << line_1_int << endl;
        getline(file_1, line_1);
      }
      else {
        merge << line_2_int << endl;
        getline(file_2, line_2);
      }
    }

    // If there's only data in one file, print the remainder of it.
    while(!line_1.empty())
    {
      merge << line_1 << endl;
      getline(file_1, line_1);
    }

    while(!line_2.empty())
    {
      merge << line_2 << endl;
      getline(file_2, line_2);
    }

    // Cleanup
    merge.close();
    file_1.close();
    file_2.close();
    remove(file_1_filename.c_str());
    remove(file_2_filename.c_str());

    // add the merged file to the queue
    files_to_merge.push(merge_filename);
  }

  // Rename the final merge to the name of the original file.
  rename(files_to_merge.front().c_str(), filename.c_str());
}


/**
   Writes the data in `data` to a file on disk named `file_number`.
*/
void flush_data(const vector<int> &data, const int file_number)
{
  ofstream file;
  file.open(lexical_cast<string>(file_number).c_str());
 
  for(vector<int>::const_iterator itr = data.begin(); itr != data.end(); ++itr)
  {
    file << *itr << endl;
  }
}


/**
   Reads each line from the file at `filename` until we get to `items_per_chunk`
   items in memory. When that happens, we sort, flush the data to disk, and then
   start over (overwriting the old data with the next chunk's.

   After all data has been sorted and written to temporary files, we merge those
   temporary files into one larger one and then write that file back to the 
   original location.
**/
void external_sort(const string & filename, const int items_per_chunk)
{
  ifstream file;
  file.open(filename.c_str());
  if(!file.is_open())
    throw runtime_error("Can't open: " + filename + "; ");

  int flushed_file_count = 0;

  vector<int> data(items_per_chunk);
  vector<int>::iterator data_input_point = data.begin();

  int count = 0;
  while(!file.eof())
  {
    string line;
    getline(file, line);

    if(line.empty())
      continue;

    *data_input_point++ = boost::lexical_cast<int>(line.c_str());
    ++count;

    if(count >= items_per_chunk || file.eof()){
      data.resize(count);
      sort(data.begin(), data.end());
      flush_data(data, ++flushed_file_count);
      data_input_point = data.begin();
      count = 0;
    }

    if(file.eof()) {
      file.close();
      break;
    }
  }

  merge_files(flushed_file_count, filename);
}


int main(int argc, char* argv[])
{
  // Realize I'm not doing a lot of validation here...
  if(argc != 2)
    cerr << "You need to specify a file to sort." << endl;

  external_sort(argv[1], 10);
}
