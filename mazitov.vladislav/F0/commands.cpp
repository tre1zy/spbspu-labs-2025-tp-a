#include "commands.hpp"
#include "dataset.hpp"
#include "huffman.hpp"
#include <algorithm>
#include <iomanip>
#include <stdexcept>

namespace
{
  struct CharFilter
  {
    const std::string& text;
    explicit CharFilter(const std::string& t):
      text(t)
    {}
    bool operator()(char c) const
    {
      return text.find(c) == std::string::npos;
    }
  };
}

void mazitov::getCmds(std::map< std::string, std::function< void(DataSetManager&, std::istream&, std::ostream&) > >& cmds)
{
  cmds["compress"] = compressCommand;
  cmds["decompress"] = decompressCommand;
  cmds["degree"] = degreeCommand;
  cmds["show_original"] = showOriginalCommand;
  cmds["show_compressed"] = showCompressedCommand;
  cmds["delete"] = deleteCommand;
  cmds["merge"] = mergeCommand;
  cmds["subtract"] = subrtactCommand;
  cmds["compare_degree"] = compareDegreesCommand;
  cmds["show_code"] = showCodeCommand;
  cmds["load_text"] = loadTextCommand;
  cmds["compressedToBin"] = compressedToBinCommand;
}

void mazitov::compressCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName, text;
  in >> setName;
  std::getline(in, text);
  if (!text.empty() && text[0] == ' ')
  {
    text = text.substr(1);
  }
  if (!text.empty() && text[0] == '"' && text.back() == '"')
  {
    text = text.substr(1, text.size() - 2);
  }
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  if (text.empty())
  {
    throw std::runtime_error("<TEXT_IS_EMPTY>");
  }
  
  auto* dataSet = mgmt.getDataSet(setName);
  dataSet->originalText = text;
  if (mgmt.compressDataSet(setName))
  {
    out << "Succesful: text was compressed and add in " << setName << ".\n";
  }
}

void mazitov::decompressCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("SET_NOT_FOUND");
  }
  
  auto* dataSet = mgmt.getDataSet(setName);
  if (dataSet->originalText.empty())
  {
    throw std::runtime_error("<NOT_COMPRESSED_DATA>");
  }
  out << "Original text: \"" << dataSet->originalText << "\"\n";
}

void mazitov::degreeCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  
  auto* dataSet = mgmt.getDataSet(setName);
  if (dataSet->originalText.empty() || dataSet->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA>");
  }

  std::string compressed = mgmt.getCompressedData(setName);
  double originalSize = dataSet->originalText.size() * 8.0;
  double compressedSize = dataSet->huffCodes.size();
  double ratio = compressedSize / originalSize;
  out << "Degree ratio: " << std::fixed << std::setprecision(2) << ratio << "\n";
}

void mazitov::showOriginalCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }

  auto* dataSet = mgmt.getDataSet(setName);
  if (dataSet->originalText.empty())
  {
    throw std::runtime_error("<NO_ORIGINAL_TEXT_LOAD>");
  }
  out << dataSet->originalText << "\n";
}

void mazitov::deleteCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }
  mgmt.deleteDataSet(setName);
  out << "Set: " << setName << "was deleted.\n";
}

void mazitov::mergeCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string set1, set2, resultSet;
  in >> set1 >> set2 >> resultSet;
  if (!mgmt.dataSetExists(set1)) 
  {
    throw std::runtime_error("<SET1_NOT_FOUND>");
  }
  if (!mgmt.dataSetExists(set2)) 
  {
    throw std::runtime_error("<SET2_NOT_FOUND>");
  }
  if (mgmt.dataSetExists(resultSet)) 
  {
    throw std::runtime_error("<RESULT_SET_ALREADY_EXISTS>");
  }
  auto* ds1 = mgmt.getDataSet(set1);
  auto* ds2 = mgmt.getDataSet(set2);
  mgmt.createDataSet(resultSet);
  auto* result = mgmt.getDataSet(resultSet);
  result->originalText = ds1->originalText + ds2->originalText;
  mgmt.compressDataSet(resultSet);
  out << "Sets: " << set1 << " and " << set2 << " were merge in " << resultSet << "\n";
}

void mazitov::subrtactCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string set1, set2, resultSet;
  in >> set1 >> set2 >> resultSet;
  if (!mgmt.dataSetExists(set1))
  {
    throw std::runtime_error("<SET1_NOT_FOUND>");
  }
  if (!mgmt.dataSetExists(set2))
  {
    throw std::runtime_error("<SET1_NOT_FOUND>");
  }
  if (mgmt.dataSetExists(resultSet))
  {
    throw std::runtime_error("<RESULT_SET_ALREADY_EXISTS>");
  }
  mgmt.createDataSet(resultSet);
  auto* ds1 = mgmt.getDataSet(set1);
  auto* ds2 = mgmt.getDataSet(set2);
  auto* result = mgmt.getDataSet(resultSet);
  std::string resultText;
  resultText.resize(ds2->originalText.size());
  auto it = std::copy_if(ds2->originalText.begin(), ds2->originalText.end(), resultText.begin(), CharFilter(ds1->originalText));
  resultText.erase(it, resultText.end());
  result->originalText = resultText;
  mgmt.compressDataSet(resultSet);
  out << "From set2" << " were deleted all codes including in set1\n";
}

void mazitov::compareDegreesCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string set1, set2;
  in >> set1 >> set2;
  if (!mgmt.dataSetExists(set1))
  {
    throw std::runtime_error("<SET1_NOT_FOUND>");
  }
  if (!mgmt.dataSetExists(set2))
  {
    throw std::runtime_error("<SET2_NOT_FOUND>");
  }

  auto* ds1 = mgmt.getDataSet(set1);
  auto* ds2 = mgmt.getDataSet(set2);
  if (ds1->originalText.empty() || ds1->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA_IN_SET1>");
  }
  if (ds2->originalText.empty() || ds2->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA_IN_SET2>");
  }
  std::string comp1 = mgmt.getCompressedData(set1);
  std::string comp2 = mgmt.getCompressedData(set2);
  double ratio1 = static_cast< double >(comp1.size()) / (ds1->originalText.size() * 8);
  double ratio2 = static_cast< double >(comp2.size()) / (ds2->originalText.size() * 8);
  if (ratio1 < ratio2)
  {
    out << set1 << " better compress (" << ratio1 << " < " << ratio2 << ")\n";
  }
  else if (ratio2 > ratio1)
  {
    out << set2 << " better compress (" << ratio2 << " < " << ratio1 << ")\n";
  }
  else
  {
    out << "Two sets has equal degree of ratio: " << ratio1 << '\n';
  }
}

void mazitov::showCodeCommand(DataSetManager& mgmt, std::istream& in, std::ostream& out)
{
  std::string setName;
  in >> setName;
  if (!mgmt.dataSetExists(setName))
  {
    throw std::runtime_error("<SET_NOT_FOUND>");
  }

  auto* ds = mgmt.getDataSet(setName);
  if (ds->huffCodes.empty())
  {
    throw std::runtime_error("<NO_COMPRESSED_DATA>");
  }
  out << "Huffman codes from set " << setName << ":\n";
  for (const auto& pair : ds->huffCodes)
  {
    out << "  '" << pair.first << "': " << pair.second << "\n";
  }
}
