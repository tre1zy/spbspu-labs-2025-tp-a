#include "sequence-commands.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <functional>
#include <algorithm>
#include <map>
#include <stream-guard.hpp>
#include "music-io-utils.hpp"

namespace
{
  template< class Key, class Value >
  struct KeyReturner
  {
    Key operator()(const std::pair< Key, Value >& pair) const noexcept
    {
      return pair.first;
    }
  };
}

void aleksandrov::printHelp(std::ostream& out)
{
  StreamGuard guard(out);
  out << "No help\n";
}

void aleksandrov::processCommands(std::istream& in, std::ostream& out, Sequences& seqs)
{
  std::map< std::string, std::function< void() > > commands;
  commands["list"] = std::bind(listSeqs, std::ref(out), std::cref(seqs));
  commands["new"] = std::bind(newSeq, std::ref(in), std::ref(seqs));
  commands["load"] = std::bind(loadSeq, std::ref(in), std::ref(seqs));
  commands["print"] = std::bind(printSeq, std::ref(in), std::ref(out), std::cref(seqs));

  std::string command;
  while (!(in >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::out_of_range&)
    {
      out << "ERROR: Invalid command!\n";
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::logic_error& e)
    {
      out << "ERROR: " << e.what() << '\n';
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void aleksandrov::listSeqs(std::ostream& out, const Sequences& seqs)
{
  if (seqs.empty())
  {
    out << "No sequences.\n";
    return;
  }
  std::ostream_iterator< std::string > outIt(out, "\n");
  KeyReturner< std::string, Sequence > returner;
  StreamGuard guard(out);
  std::transform(seqs.begin(), seqs.end(), outIt, returner);
}

void aleksandrov::newSeq(std::istream& in, Sequences& seqs)
{
  std::string seqName;
  if (!(in >> seqName))
  {
    throw std::logic_error("Incorrect sequence name!");
  }
  if (seqs.find(seqName) != seqs.end())
  {
    throw std::logic_error("Sequence '" + seqName + "' already exists!");
  }
  seqs[seqName];
}

void aleksandrov::loadSeq(std::istream& in, Sequences& seqs)
{
  std::string fileName;
  if (!(in >> fileName))
  {
    throw std::logic_error("Incorrect file name!");
  }
  std::string seqName;
  if (!(in >> seqName))
  {
    throw std::logic_error("Incorrect sequence name!");
  }
  std::ifstream file(fileName);
  if (!file)
  {
    throw std::logic_error("Incorrect file!");
  }
  Sequence seq;
  readSequence(file, seq);
  if (file || file.eof())
  {
    seqs[seqName] = std::move(seq);
  }
  else
  {
    throw std::logic_error("Incorrect sequence description!");
  }
}

void aleksandrov::printSeq(std::istream& in, std::ostream& out, const Sequences& seqs)
{
  std::string seqName;
  if (!(in >> seqName))
  {
    throw std::logic_error("Incorrect sequence name!");
  }
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  const Sequence& sequence = seqIt->second;
  std::ostream_iterator< MusicalElement > outIt(out, "\n");
  StreamGuard guard(out);
  std::copy(sequence.begin(), sequence.end(), outIt);
}

