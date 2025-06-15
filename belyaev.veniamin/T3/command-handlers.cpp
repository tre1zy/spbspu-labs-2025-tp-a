#include "command-handlers.hpp"

double belyaev::areaOdd(const std::vector<Polygon>& data)
{
  using namespace std::placeholders;
  auto areaOddAccumBind = std::bind(areaOddAccumulate, _1, _2);
  return std::accumulate(data.begin(), data.end(), 0.0, areaOddAccumBind);
}

double belyaev::areaEven(const std::vector<Polygon>& data)
{
  using namespace std::placeholders;
  auto areaEvenAccumBind = std::bind(areaEvenAccumulate, _1, _2);
  return std::accumulate(data.begin(), data.end(), 0.0, areaEvenAccumBind);
}

double belyaev::areaMean(const std::vector<Polygon>& data)
{
  if (data.size() == 0)
  {
    throw std::logic_error("Invalid query in AREA MEAN.");
  }
  using namespace std::placeholders;
  auto areaMeanAccumBind = std::bind(areaMeanAccumulate, _1, _2, data.size());
  return std::accumulate(data.begin(), data.end(), 0.0, areaMeanAccumBind);
}

double belyaev::areaVertices(const std::vector<Polygon>& data, size_t vertices)
{
  using namespace std::placeholders;
  auto areaVerticesAccumBind = std::bind(areaVerticesAccumulate, _1, _2, vertices);
  return std::accumulate(data.begin(), data.end(), 0.0, areaVerticesAccumBind);
}

void belyaev::area(const std::vector<Polygon>& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  if (in.fail())
  {
    throw std::logic_error("Input failed in AREA.");
  }

  std::map<std::string, std::function<double()>> subCmds;
  subCmds["ODD"] = std::bind(areaOdd, std::ref(data));
  subCmds["EVEN"] = std::bind(areaEven, std::ref(data));
  subCmds["MEAN"] = std::bind(areaMean, std::ref(data));
  double res = 0.0;
  try
  {
    res = subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    if (isStringNumeric(subcommand))
    {
      size_t vertices = std::stoull(subcommand);
      if (vertices < 3)
      {
        throw std::logic_error("Invalid query in AREA.");
      }
      res = areaVertices(data, vertices);
    }
    else
    {
      throw std::logic_error("Unknown command.");
    }
  }

  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << res << '\n';
}

void belyaev::minMaxArea(const std::vector<Polygon>& data, std::ostream& out, const std::string& command)
{
  Polygon resultingPolygon;
  std::map<std::string, std::function<Polygon()>> minOrMax;
  minOrMax["min"] = std::bind(minElement, std::cref(data), compareAreas);
  minOrMax["max"] = std::bind(maxElement, std::cref(data), compareAreas);
  try
  {
    resultingPolygon = minOrMax.at(command)();
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("minMaxArea failed.");
  }
  
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << calcArea(resultingPolygon) << '\n';
}

void belyaev::minMaxVertices(const std::vector<Polygon>& data, std::ostream& out, const std::string& command)
{
  Polygon resultingPolygon;
  std::map<std::string, std::function<Polygon()>> minOrMax;
  minOrMax["min"] = std::bind(minElement, std::cref(data), compareVertices);
  minOrMax["max"] = std::bind(maxElement, std::cref(data), compareVertices);
  try
  {
    resultingPolygon = minOrMax.at(command)();
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("minMaxVertices failed.");
  }

  StreamGuard guard(out);
  out << getVertices(resultingPolygon) << '\n';
}

void belyaev::minMax(const std::vector<Polygon>& data, std::istream& in, std::ostream& out, const std::string& command)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  if (in.fail() || data.size() == 0)
  {
    throw std::logic_error("Input failed in MIN/MAX.");
  }

  std::map<std::string, std::function<void()>> subCmds;
  subCmds["AREA"] = std::bind(minMaxArea, std::ref(data), std::ref(out), std::cref(command));
  subCmds["VERTEXES"] = std::bind(minMaxVertices, std::ref(data), std::ref(out), std::cref(command));
  try
  {
    subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("Unknown command.");
  }
}

size_t belyaev::countEven(const std::vector<Polygon>& data)
{
  using namespace std::placeholders;
  auto isPolygonEvenBind = std::bind(isPolygonEven, _1);
  return std::count_if(data.begin(), data.end(), isPolygonEvenBind);
}

size_t belyaev::countOdd(const std::vector<Polygon>& data)
{
  using namespace std::placeholders;
  auto isPolygonOddBind = std::bind(isPolygonOdd, _1);
  return std::count_if(data.begin(), data.end(), isPolygonOddBind);
}

size_t belyaev::countVertices(const std::vector<Polygon>& data, size_t givenSize)
{
  using namespace std::placeholders;
  auto isPolygonOfSizeBind = std::bind(isPolygonOfSize, _1, givenSize);
  return std::count_if(data.begin(), data.end(), isPolygonOfSizeBind);
}

void belyaev::count(const std::vector<Polygon>& data, std::istream& in, std::ostream& out)
{
  std::map<std::string, std::function<size_t()>> subCmds;
  size_t result;
  subCmds["EVEN"] = std::bind(countEven, std::cref(data));
  subCmds["ODD"] = std::bind(countOdd, std::cref(data));

  std::string subcommand;
  in >> subcommand;
  if (in.fail())
  {
    throw std::logic_error("Input failed in COUNT.");
  }

  try
  {
    result = subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    if (isStringNumeric(subcommand))
    {
      size_t vertices = std::stoull(subcommand);
      if (vertices < 3)
      {
        throw std::logic_error("Invalid query in COUNT.");
      }
      result = countVertices(data, std::stoull(subcommand));
    }
    else
    {
      throw std::logic_error("Unknown command.");
    }
  }

  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << result << '\n';
}

void belyaev::rmecho(std::vector<Polygon>& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  Polygon rmPolygon;
  in >> rmPolygon;
  if (in.fail())
  {
    throw std::logic_error("Input failed in RMECHO.");
  }
  size_t oldSize = data.size();
  auto helperBind = std::bind(rmEchoHelper, rmPolygon, _1, _2);
  auto new_end = std::unique(data.begin(), data.end(), helperBind);
  data.erase(new_end, data.end());
  size_t removed = oldSize - data.size();

  StreamGuard guard(out);
  out << removed << '\n';
}

void belyaev::inframe(const std::vector<Polygon>& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  Polygon inframePoly;
  in >> inframePoly;
  if (in.fail())
  {
    throw std::logic_error("Input failed in INFRAME.");
  }

  Borders polygonBorders = std::accumulate(data.begin(), data.end(), Borders{}, getPolygonBorders);
  auto isPointInBordersBind = std::bind(isPointInBorders, _1, std::cref(polygonBorders));
  bool inside = std::all_of(inframePoly.points.begin(), inframePoly.points.end(), isPointInBordersBind);

  StreamGuard guard(out);
  if (inside)
  {
    out << "<TRUE>\n";
  }
  else
  {
    out << "<FALSE>\n";
  }
}

belyaev::commandMap belyaev::mapCommandHandlers(std::vector<Polygon>& data)
{
  using namespace std::placeholders;
  commandMap cmds;
  const std::string minCommand = "min";
  const std::string maxCommand = "max";
  cmds["AREA"] = std::bind(area, std::cref(data), _1, _2);
  cmds["MAX"] = std::bind(minMax, std::cref(data), _1, _2, maxCommand);
  cmds["MIN"] = std::bind(minMax, std::cref(data), _1, _2, minCommand);
  cmds["COUNT"] = std::bind(count, std::cref(data), _1, _2);
  cmds["RMECHO"] = std::bind(rmecho, std::ref(data), _1, _2);
  cmds["INFRAME"] = std::bind(inframe, std::cref(data), _1, _2);
  return cmds;
}