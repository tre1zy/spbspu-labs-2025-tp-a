#include "main_processor.hpp"

#include <iostream>
#include <fstream>
#include <boost/json.hpp>

bool rychkov::MainProcessor::load(std::ostream& out, std::ostream& err, std::string filename)
{
  std::ifstream in(filename);
  if (!in)
  {
    err << "failed to open save file on read - \"" << filename << "\"\n";
    return false;
  }
  boost::json::value doc = boost::json::parse(in);
  std::map< std::string, ParseCell > new_parsed;
  for (const boost::json::object::value_type& file: doc.as_object())
  {
    std::pair< decltype(new_parsed)::iterator, bool > cell_p = new_parsed.emplace(file.key(),
          ParseCell{{out, err, file.key()}, last_stage_, include_dirs_});
    if (cell_p.second)
    {
      out << "<--LOAD: \"" << file.key() << "\"-->\n";
    }
  }
  return true;
}
