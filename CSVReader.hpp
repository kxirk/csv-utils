#ifndef CSVREADER
#define CSVREADER

#include <fstream>
#include <sstream>
#include <string>
#include <vector>


class CSVReader {
  private:
    std::fstream file;
    std::string header;
    long bufferSize;
    char* buffer;
    std::stringstream csv;

    bool ReadChunk ();
    bool ReadFile (const std::string& filename);

  public:
    CSVReader (const std::string& filename, bool containsHeader = false, long bufferSize = 1048576);
    ~CSVReader ();

    std::string GetHeader ();
    int GetHeaderIndex (const std::string& name);

    bool GetLine (std::string& line);
    static void EscapeLine (std::vector<std::string>& escaped, const std::string& line);
};

#endif /* CSVREADER */
