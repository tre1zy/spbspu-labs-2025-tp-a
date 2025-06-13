#include "command-handlers.hpp"

double belyaev::areaOdd(double value, const Polygon& src)
{
  if (isPolygonOdd(src))
  {
    return value + calcArea(src);
  }
  return value;
}

double belyaev::areaEven(double value, const Polygon& src)
{
  if (isPolygonEven(src))
  {
    return value + calcArea(src);
  }
  return value;
}

double belyaev::areaMean(double value, const Polygon& src, size_t size)
{
  if (size == 0)
  {
    throw std::logic_error("Size is 0.");
  }
  return value + calcArea(src) / size;
}

double belyaev::areaVertices(double value, const Polygon& src, size_t vertices)
{
  if (src.points.size() == vertices)
  {
    return value + calcArea(src);
  }
  return value;
}

void belyaev::area(const std::vector<Polygon>& data, std::istream& in, std::ostream& out)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  if (in.fail())
  {
    throw std::logic_error("Failed.");
  }

  std::map<std::string, std::function<double(double, const Polygon&)>> subCmds;
  subCmds["ODD"] = std::bind(areaOdd, _1, _2);
  subCmds["EVEN"] = std::bind(areaEven, _1, _2);
  subCmds["MEAN"] = std::bind(areaMean, _1, _2, data.size());
  double res = 0.0;
  try
  {
    subCmds.at(subcommand);
    res = std::accumulate(data.begin(), data.end(), 0.0, subCmds[subcommand]);
  }
  catch (const std::out_of_range& e)
  {
    std::cout << subcommand << '\n';
    if (isStringNumeric(subcommand))
    {
      auto areaVerticesBind = std::bind(areaVertices, _1, _2, std::stoull(subcommand));
      res = std::accumulate(data.begin(), data.end(), 0.0, areaVerticesBind);
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
  using namespace std::placeholders;
  auto compareAreasBind = std::bind(compareAreas, _1, _2);
  Polygon resultingPolygon;
  if (command == "min")
  {
    resultingPolygon = *std::min_element(data.begin(), data.end(), compareAreasBind);
  }
  else if (command == "max")
  {
    resultingPolygon = *std::max_element(data.begin(), data.end(), compareAreasBind);
  }
  else
  {
    throw std::logic_error("minMaxArea failed");
  }
  StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << calcArea(resultingPolygon) << '\n';
}

void belyaev::minMaxVertices(const std::vector<Polygon>& data, std::ostream& out, const std::string& command)
{
  using namespace std::placeholders;
  auto compareVertsBind = std::bind(compareVertices, _1, _2);
  Polygon resultingPolygon;
  if (command == "min")
  {
    resultingPolygon = *std::min_element(data.begin(), data.end(), compareVertsBind);
  }
  else if (command == "max")
  {
    resultingPolygon = *std::max_element(data.begin(), data.end(), compareVertsBind);
  }
  else
  {
    throw std::logic_error("minMaxVertices failed");
  }
  StreamGuard guard(out);
  out << getVertices(resultingPolygon) << '\n';
}

void belyaev::minMax(const std::vector<Polygon>& data, std::istream& in, std::ostream& out, const std::string& command)
{
  using namespace std::placeholders;

  std::string subcommand;
  in >> subcommand;
  if (in.fail())
  {
    throw std::logic_error("Failed.");
  }

  std::map<std::string, std::function<void()>> subCmds;
  subCmds["AREA"] = std::bind(minMaxArea, std::ref(data), std::ref(out), std::cref(command));
  subCmds["VERTICES"] = std::bind(minMaxVertices, std::ref(data), std::ref(out), std::cref(command));
  try
  {
    subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    throw std::logic_error("Unknown command.");
  }
}

void belyaev::countEven(const std::vector<Polygon>& data, size_t& result)
{
  using namespace std::placeholders;
  auto isPolygonEvenBind = std::bind(isPolygonEven, _1);
  result = std::count_if(data.begin(), data.end(), isPolygonEvenBind);
}

void belyaev::countOdd(const std::vector<Polygon>& data, size_t& result)
{
  using namespace std::placeholders;
  auto isPolygonOddBind = std::bind(isPolygonOdd, _1);
  result = std::count_if(data.begin(), data.end(), isPolygonOddBind);
}

void belyaev::countVertices(const std::vector<Polygon>& data, size_t& result, size_t givenSize)
{
  using namespace std::placeholders;
  auto isPolygonOfSizeBind = std::bind(isPolygonOfSize, _1, givenSize);
  result = std::count_if(data.begin(), data.end(), isPolygonOfSizeBind);
}

void belyaev::count(const std::vector<Polygon>& data, std::istream& in, std::ostream& out)
{
  std::map<std::string, std::function<void()>> subCmds;
  size_t result;
  subCmds["EVEN"] = std::bind(countEven, std::cref(data), std::ref(result));
  subCmds["ODD"] = std::bind(countOdd, std::cref(data), std::ref(result));

  std::string subcommand;
  in >> subcommand;
  if (in.fail())
  {
    throw std::logic_error("Failed.");
  }

  try
  {
    subCmds.at(subcommand)();
  }
  catch (const std::out_of_range& e)
  {
    if (isStringNumeric(subcommand))
    {
      countVertices(data, result, std::stoull(subcommand));
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
    throw std::logic_error("Failed.");
  }
  data.size();
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
    throw std::logic_error("Invalid input.");
  }

  int minX, minY, maxX, maxY;
  minX = std::numeric_limits<int>::max();
  minY = std::numeric_limits<int>::max();
  maxX = std::numeric_limits<int>::min();
  maxY = std::numeric_limits<int>::min();
  auto getMinCoordsBind = std::bind(getMinMaxCoordsInPoly, _1, std::ref(minX), std::ref(minY), "min");
  auto getMaxCoordsBind = std::bind(getMinMaxCoordsInPoly, _1, std::ref(maxX), std::ref(maxY), "max");
  std::for_each(data.begin(), data.end(), getMinCoordsBind);
  std::for_each(data.begin(), data.end(), getMaxCoordsBind);

  int currentMinX = minX;
  int currentMinY = minY;
  int currentMaxX = maxX;
  int currentMaxY = maxY;
  getMinCoordsBind(inframePoly);
  getMaxCoordsBind(inframePoly);
  StreamGuard guard(out);
  if (currentMinX != minX || currentMinY != minY || currentMaxX != maxX || currentMaxY != maxY)
  {
    out << "<FALSE>" << '\n';
  }
  else
  {
    out << "<TRUE>" << '\n';
  }
}
