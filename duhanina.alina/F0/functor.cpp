#include "functor.hpp"
#include "shannonFano.hpp"

bool NullChecker::operator()(const duhanina::Node* node) const
{
  return node == nullptr;
}

duhanina::Node* duhanina::NodeMerger::operator()(duhanina::Node* subtree, duhanina::Node* node) const
{
  if (!subtree || !node)
  {
    throw std::invalid_argument("Null node");
  }
  duhanina::Node* new_node = nullptr;
  duhanina::Node* new_subtree = nullptr;
  try
  {
    new_node = new duhanina::Node(*node);
    new_subtree = new duhanina::Node(subtree->freq + new_node->freq, subtree, new_node);
  }
  catch (...)
  {
    delete new_subtree;
    delete new_node;
    throw;
  }
  return new_subtree;
}

size_t duhanina::FreqAccumulator::operator()(size_t sum, duhanina::Node* node) const
{
  return sum + node->freq;
}

duhanina::SplitPositionFinder::SplitPositionFinder(size_t half_total):
  sum_(0),
  half_total_(half_total)
{}

bool duhanina::SplitPositionFinder::operator()(duhanina::Node* node)
{
  sum_ += node->freq;
  return sum_ >= half_total_;
}

duhanina::Node* duhanina::NodeCreator::operator()(const std::pair< char, size_t >& pair) const
{
  return new duhanina::Node(pair.first, pair.second);
}

duhanina::FreqCounter::FreqCounter(std::map<char, size_t>& freq_map):
  freq_map_(freq_map)
{}

void duhanina::FreqCounter::operator()(char c) const
{
  freq_map_[c]++;
}

duhanina::CodeTableFiller::CodeTableFiller(duhanina::CodeTable& table):
  table_(table)
{}

void duhanina::CodeTableFiller::operator()(const std::pair< char, std::string >& entry) const
{
  table_.code_to_char[entry.second] = entry.first;
}

duhanina::TreeBuilder::TreeBuilder(std::vector<duhanina::Node*>& nodes):
  nodes_(nodes)
{}

void duhanina::TreeBuilder::operator()() const
{
  if (nodes_.size() <= 1)
  {
    return;
  }
  auto node_pair = duhanina::split_nodes(nodes_);
  nodes_ = duhanina::merge_nodes(node_pair.first, node_pair.second);
  (*this)();
}

void duhanina::TreeDeleter::operator()(duhanina::Node* node) const
{
  duhanina::delete_tree(node);
}

duhanina::CharEncoder::CharEncoder(const duhanina::CodeTable& table, std::string& result):
  table_(table),
  result_(result)
{}

void duhanina::CharEncoder::operator()(char c) const
{
  auto it = table_.char_to_code.find(c);
  if (it == table_.char_to_code.end())
  {
    throw std::runtime_error("INVALID_CODES");
  }
  result_ += it->second;
}

duhanina::CodeAccumulator::CodeAccumulator(std::string& decoded, std::string& current_code, const duhanina::CodeTable& table):
  decoded_(decoded),
  current_code_(current_code),
  table_(table)
{}

void duhanina::CodeAccumulator::operator()(char bit)
{
  current_code_ += bit;
  auto it = table_.code_to_char.find(current_code_);
  if (it != table_.code_to_char.end())
  {
    decoded_ += it->second;
    current_code_.clear();
  }
}

duhanina::SizeTByteWriter::SizeTByteWriter(std::ofstream& out, size_t value):
  out_(out),
  value_(value)
{}

void duhanina::SizeTByteWriter::operator()(size_t byte_index) const
{
  char byte = static_cast< char >((value_ >> (8 * byte_index)) & 0xFF);
  out_.put(byte);
}

duhanina::BitWriter::BitWriter(std::ofstream& out):
  out_(out)
{}

void duhanina::BitWriter::operator()(char bit)
{
  if (bit == '1')
  {
    buffer_ |= (1 << (7 - bit_pos_));
  }
  bit_pos_++;
  if (bit_pos_ == 8)
  {
    out_.put(buffer_);
    buffer_ = 0;
    bit_pos_ = 0;
  }
}

void duhanina::BitWriter::flush()
{
  if (bit_pos_ > 0)
  {
    out_.put(buffer_);
  }
}

duhanina::HeaderReader::HeaderReader(std::ifstream& in, size_t& bit_count):
  in_(in),
  bit_count_(bit_count),
  shift_(0)
{}

void duhanina::HeaderReader::operator()(int)
{
  char byte;
  if (!in_.get(byte))
  {
    throw std::runtime_error("INVALID_HEADER");
  }
  bit_count_ |= static_cast< size_t >(static_cast< unsigned char >(byte)) << (8 * shift_++);
}

duhanina::BitHandler::BitHandler(std::string& result, size_t& processed, size_t total_bits):
  result_(result),
  processed_(processed),
  total_bits_(total_bits)
{}

void duhanina::BitHandler::operator()(size_t bit_pos)
{
  if (processed_ < total_bits_)
  {
    result_ += bits_.test(7 - bit_pos) ? '1' : '0';
    ++processed_;
  }
}

duhanina::DataProcessor::DataProcessor(size_t total_bits):
  total_bits_(total_bits),
  processed_(0)
{
  result_.reserve(total_bits);
}

void duhanina::DataProcessor::operator()(char byte)
{
  if (processed_ >= total_bits_)
  {
    return;
  }
  std::bitset< 8 > bits(byte);
  BitHandler handler(result_, processed_, total_bits_);
  handler.bits_ = bits;
  std::vector< size_t > bit_positions(8);
  std::iota(bit_positions.begin(), bit_positions.end(), 0);
  std::for_each(bit_positions.begin(), bit_positions.end(), handler);
}

std::string duhanina::DataProcessor::get_result() const
{
  return result_;
}

bool duhanina::DataProcessor::is_complete() const
{
  return processed_ == total_bits_;
}

duhanina::TableEntryWriter::TableEntryWriter(std::ofstream& output_stream):
  out_(output_stream)
{}

void duhanina::TableEntryWriter::operator()(const std::pair< char, std::string >& entry)
{
  out_ << static_cast< int >(entry.first) << " " << entry.second << "\n";
}

void duhanina::LineProcessor::operator()(const std::string& line, duhanina::CodeTable& table) const
{
  if (line.empty())
  {
    return;
  }
  const size_t space_pos = line.find(' ');
  if (space_pos == std::string::npos || space_pos == line.length() - 1)
  {
    return;
  }
  try
  {
    const int char_code = std::stoi(line.substr(0, space_pos));
    const std::string code = line.substr(space_pos + 1);
    const char character = static_cast< char >(char_code);
    if (char_code < std::numeric_limits< char >::min() || char_code > std::numeric_limits< char >::max())
    {
      throw std::out_of_range("CHAR_CODE_OUT_OF_RANGE");
    }
    table.char_to_code[character] = code;
    table.code_to_char[code] = character;
  }
  catch (...)
  {
    return;
  }
}

void duhanina::StreamProcessor::operator()(std::istream_iterator< std::string > it, duhanina::CodeTable& table) const
{
  if (it != std::istream_iterator< std::string >())
  {
    line_processor_(*it, table);
  }
}

duhanina::CharChecker::CharChecker(const std::map< char, std::string >& char_map):
  char_to_code_ref_(char_map)
{}

bool duhanina::CharChecker::operator()(char c) const
{
  return char_to_code_ref_.find(c) == char_to_code_ref_.end();
}

duhanina::CharInserter::CharInserter(std::set< char >& missing_set):
  missing_ref_(missing_set)
{}

void duhanina::CharInserter::operator()(char c) const
{
  missing_ref_.insert(c);
}

duhanina::CharPrinter::CharPrinter(std::ostream& output_stream):
  out_(output_stream)
{}

void duhanina::CharPrinter::operator()(char c) const
{
  if (std::isprint(c))
  {
    out_ << "'" << c << "' ";
  }
  else
  {
    out_ << "[0x" << std::hex << static_cast< int >(c) << "] ";
  }
}

duhanina::CodePairPrinter::CodePairPrinter(std::ostream& output_stream):
  out_(output_stream)
{}

void duhanina::CodePairPrinter::operator()(const std::pair< char, std::string >& char_code_pair) const
{
  const char c = char_code_pair.first;
  str_t code = char_code_pair.second;
  if (std::isprint(c) && !std::isspace(c))
  {
    out_ << "'" << c << "'\t" << code << "\n";
  }
  else
  {
    out_ << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast< int >(c) << std::dec << "\t" << code << "\n";
  }
}

duhanina::EncodingInfoPrinter::EncodingInfoPrinter(std::ostream& output_stream):
  out_(output_stream)
{}

void duhanina::EncodingInfoPrinter::operator()(const std::pair< str_t, CodeTable >& encoding_entry) const
{
  str_t encoding_id = encoding_entry.first;
  const CodeTable& table = encoding_entry.second;
  out_ << "  " << encoding_id << " (" << table.char_to_code.size() << " symbols, total " << table.total_chars << ")\n";
}

duhanina::EncodingChecker::EncodingChecker(str_t text, std::ostream& output_stream):
  text_ref_(text),
  out_(output_stream)
{}

void duhanina::EncodingChecker::operator()(const std::pair< str_t, CodeTable >& encoding_pair) const
{
  str_t id = encoding_pair.first;
  const CodeTable& table = encoding_pair.second;
  std::set< char > missing = find_missing_chars(text_ref_, table.char_to_code);
  out_ << " - " << id << ": ";
  if (missing.empty())
  {
    out_ << "FULL";
  }
  else
  {
    out_ << "partial";
  }
  out_ << " support\n";
}
