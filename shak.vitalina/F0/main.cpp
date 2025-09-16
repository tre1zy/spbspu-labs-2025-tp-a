int main()
{
  using namespace shak;
  shak::Dictionaries dicts;
  std::map< std::string, std::function< void() > > cmds;
  cmds["create"] = std::bind(createDict, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["clear"] = std::bind(clearDict, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["delete"] = std::bind(deleteDict, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["insert"] = std::bind(insertW, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["remove"] = std::bind(removeW, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["print"] = std::bind(printDict, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["frequency"] = std::bind(getFreq, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["uniqe"] = std::bind(getUniqe, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["equal"] = std::bind(equal, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  cmds["compare"] = std::bind(compare, std::ref(dicts), std::ref(std::cin), std::ref(std::cout));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      cmds.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
