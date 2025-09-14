#include "sequence-commands.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>
#include <map>
#include <stream-guard.hpp>
#include "music-io-utils.hpp"

namespace
{
  using namespace aleksandrov;

  template< class Key, class Value >
  struct KeyReturner
  {
    Key operator()(const std::pair< Key, Value >& pair) const noexcept
    {
      return pair.first;
    }
  };

  struct NotesCounter
  {
    size_t operator()(size_t count, const MusicalElement& element) const
    {
      return count + (element.isNote() ? 1 : (element.isInterval() ? 2 : 0));
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
  commands["save"] = std::bind(saveSeq, std::ref(in), std::ref(seqs));
  commands["clear"] = std::bind(clearSeq, std::ref(in), std::ref(seqs));
  commands["delete"] = std::bind(deleteSeq, std::ref(in), std::ref(seqs));
  commands["clone"] = std::bind(cloneSeq, std::ref(in), std::ref(seqs));
  commands["add"] = std::bind(addToSeq, std::ref(in), std::ref(seqs));
  commands["remove"] = std::bind(removeFromSeq, std::ref(in), std::ref(seqs));
  commands["merge"] = std::bind(mergeSeqs, std::ref(in), std::ref(seqs));
  commands["print"] = std::bind(printSeq, std::ref(in), std::ref(out), std::cref(seqs));
  commands["elements"] = std::bind(elementsSeq, std::ref(in), std::ref(out), std::cref(seqs));
  commands["notes"] = std::bind(notesSeq, std::ref(in), std::ref(out), std::cref(seqs));
  commands["type"] = std::bind(typeSeq, std::ref(in), std::ref(out), std::cref(seqs));

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
  in >> seqName;
  if (seqs.find(seqName) != seqs.end())
  {
    throw std::logic_error("Sequence '" + seqName + "' already exists!");
  }
  seqs[seqName];
}

void aleksandrov::loadSeq(std::istream& in, Sequences& seqs)
{
  std::string fileName;
  std::string seqName;
  in >> fileName >> seqName;

  std::ifstream file(fileName);
  if (!file)
  {
    throw std::logic_error("Incorrect file!");
  }
  Sequence seq;
  std::istream_iterator< MusicalElement > inItBegin(file);
  std::istream_iterator< MusicalElement > inItEnd;
  std::copy(inItBegin, inItEnd, std::back_inserter(seq));
  if (file || file.eof())
  {
    seqs[seqName] = std::move(seq);
  }
  else
  {
    throw std::logic_error("Incorrect sequence description!");
  }
}

void aleksandrov::saveSeq(std::istream& in, Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  std::string fileName;
  in >> fileName;
  std::ofstream file(fileName);
  if (!file)
  {
    throw std::logic_error("Incorrect file!");
  }
  const Sequence& sequence = seqIt->second;
  std::ostream_iterator< MusicalElement > outIt(file, " ");
  StreamGuard guard(file);
  std::copy(sequence.begin(), sequence.end(), outIt);
  if (!file)
  {
    throw std::logic_error("Failed to write to file!");
  }
}

void aleksandrov::clearSeq(std::istream& in, Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("There is no such sequence '" + seqName + "'");
  }
  seqIt->second.clear();
}

void aleksandrov::deleteSeq(std::istream& in, Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  seqs.erase(seqIt);
}

void aleksandrov::cloneSeq(std::istream& in, Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  std::string seqCloneName;
  in >> seqCloneName;
  if (seqs.find(seqCloneName) != seqs.end())
  {
    throw std::logic_error("Sequence '" + seqCloneName + "' already exists!");
  }
  seqs.insert({ seqCloneName, seqIt->second });
}

void aleksandrov::addToSeq(std::istream& in, Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  MusicalElement element;
  if (!(in >> element))
  {
    throw std::logic_error("Incorrect element name!");
  }
  Sequence& sequence = seqIt->second;
  if (in.peek() == '\n' || in.peek() == EOF)
  {
    sequence.push_back(element);
  }
  else
  {
    size_t position = 0;
    if (!(in >> position) || !position || position > sequence.size())
    {
      throw std::logic_error("Incorrect position!");
    }
    sequence.insert(sequence.begin() + position, element);
  }
}

void aleksandrov::removeFromSeq(std::istream& in, Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  Sequence& sequence = seqIt->second;
  if (in.peek() == '\n' || in.peek() == EOF)
  {
    sequence.pop_back();
  }
  else
  {
    size_t position = 0;
    if (!(in >> position) || !position || position > sequence.size())
    {
      throw std::logic_error("Incorrect position!");
    }
    sequence.erase(sequence.begin() + position - 1);
  }
}

void aleksandrov::mergeSeqs(std::istream& in, Sequences& seqs)
{
  std::string seqName1;
  std::string seqName2;
  std::string newSeqName;
  in >> seqName1 >> seqName2 >> newSeqName;
  auto seqIt1 = seqs.find(seqName1);
  if (seqIt1 == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName1 + "'");
  }
  auto seqIt2 = seqs.find(seqName2);
  if (seqIt2 == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName2 + "'");
  }
  if (seqs.find(newSeqName) != seqs.end())
  {
    throw std::logic_error("Sequence '" + newSeqName + "' already exists!");
  }
  Sequence merged;
  const Sequence& seq1 = seqIt1->second;
  const Sequence& seq2 = seqIt2->second;

  std::copy(seq1.begin(), seq1.end(), std::back_inserter(merged));
  std::copy(seq2.begin(), seq2.end(), std::back_inserter(merged));

  seqs[newSeqName] = std::move(merged);
}

void aleksandrov::printSeq(std::istream& in, std::ostream& out, const Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
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

void aleksandrov::elementsSeq(std::istream& in, std::ostream& out, const Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  size_t seqSize = seqIt->second.size();
  out << "Total number of elements in '" << seqName << "': " << seqSize << '\n';
}

void aleksandrov::notesSeq(std::istream& in, std::ostream& out, const Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  const Sequence& sequence = seqIt->second;
  size_t notesCount = std::accumulate(sequence.begin(), sequence.end(), 0, NotesCounter{});
  out << "Total number of notes in '" << seqName << "': " << notesCount << '\n';
}

void aleksandrov::typeSeq(std::istream& in, std::ostream& out, const Sequences& seqs)
{
  std::string seqName;
  in >> seqName;
  auto seqIt = seqs.find(seqName);
  if (seqIt == seqs.end())
  {
    throw std::logic_error("No such sequence '" + seqName + "'");
  }
  const Sequence& sequence = seqIt->second;
  size_t position = 0;
  if (!(in >> position) || !position || position > sequence.size())
  {
    throw std::logic_error("Incorrect position!");
  }
  out << sequence[position - 1] << ": ";
  switch (sequence[position - 1].getType())
  {
  case MusicalElementType::Note:
    out << "note";
    break;
  default:
    out << "interval";
  }
  out << '\n';
}
