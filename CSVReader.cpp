#include "CSVReader.hpp"

#include <algorithm>
#include <cstring>

#include <boost/tokenizer.hpp>


bool CSVReader::ReadChunk () {
  memset(buffer, 0, this->bufferSize);
  this->csv.clear();

  this->file.read(buffer, this->bufferSize);
  this->csv.write(buffer, this->bufferSize);

  return ((this->file).gcount() > 0);
}

bool CSVReader::ReadFile (const std::string& filename) {
  this->file.open(filename);

  return this->ReadChunk();
}


CSVReader::CSVReader (const std::string& filename, bool containsHeader, long bufferSize) {
  this->bufferSize = bufferSize;
  this->buffer = new char[bufferSize];
  this->ReadFile(filename);

  if (containsHeader) {
    this->GetLine(this->header);
  }
}

CSVReader::~CSVReader () {
  this->file.close();
  delete[] this->buffer;
}


std::string CSVReader::GetHeader () {
  return this->header;
}

int CSVReader::GetHeaderIndex (const std::string& name) {
  std::vector<std::string> escaped; 
  EscapeLine(escaped, this->header);

  int pos = distance(escaped.begin(), find(escaped.begin(), escaped.end(), name));
  return (pos == escaped.size() ? -1 : pos);
}


bool CSVReader::GetLine (std::string& line) {
  while ( getline(this->csv, line) ) {
    if ( this->csv.eof() ) {
      if ( !this->ReadChunk() ) return false;

      std::string remaining;
      getline(this->csv, remaining);

      line.append(remaining);
    }

    return true;
  }

  return false;
}

void CSVReader::EscapeLine (std::vector<std::string>& escaped, const std::string& line) {
  escaped.clear();

  boost::escaped_list_separator<char> separator('\\', ',', '\"');
  boost::tokenizer<boost::escaped_list_separator<char>> tok(line, separator);

  escaped.assign(tok.begin(), tok.end());
}
