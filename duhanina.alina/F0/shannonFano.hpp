#ifndef SHANNONFANO_HPP
#define SHANNONFANO_HPP

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <iostream>

namespace duhanina
{
  struct CodeTable
  {
    std::map< char, std::string > char_to_code;
    std::map< std::string, char > code_to_char;
    size_t total_chars = 0;
  };

  struct Node
  {
    char symbol;
    int freq;
    Node* left;
    Node* right;

    Node(char s, int f):
      symbol(s),
      freq(f),
      left(nullptr),
      right(nullptr)
    {}

    Node(int f, Node* l, Node* r):
      symbol(0),
      freq(f),
      left(l),
      right(r)
    {}
  };

  void print_help(std::ostream& out);
  void build_codes(const std::string& input_file, const std::string& encoding_name, std::ostream& out);
  void show_codes(const std::string& encoding_name, std::ostream& out);
  void save_codes(const std::string& encoding_name, const std::string& output_file, std::ostream& out);
  void load_codes(const std::string& input_file, const std::string& encoding_name, std::ostream& out);
  void clear_codes(const std::string& encoding_name, std::ostream& out);
  void encode_file_with_codes(const std::string& input_file, const std::string& encoding_name, const std::string& output_file, std::ostream& out);
  void decode_file_with_codes(const std::string& input_file, const std::string& encoding_name, const std::string& output_file, std::ostream& out);
  void encode_file(const std::string& input_file, const std::string& output_file, const std::string& encoding_name, std::ostream& out);
  void decode_file(const std::string& input_file, const std::string& output_file, const std::string& encoding_name, std::ostream& out);
  void compare(const std::string& file1, const std::string& file2, const std::string& encod_name1, const std::string& encod_name2, std::ostream& out);
  void list_encodings(std::ostream& out);
}

#endif
