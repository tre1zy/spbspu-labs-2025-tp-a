#include "commands.h"
#include <functional>
#include <algorithm>
#include <iomanip>
#include <string>
#include <map>
#include <numeric>
#include "geom.h"
#include "io-guard.h"

namespace
{
	using namespace fedorova;

	double subArea(const Point& a, const Point& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	double areaPolygon(const Polygon& polygon)
	{
		const auto& p = polygon.points;
		double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, subArea);
		sum += subArea(p.back(), p.front());
		return std::abs(sum) / 2.0;
	}

	bool isEven(const Polygon& polygon)
	{
		return polygon.points.size() % 2 == 0;
	}

	bool isOdd(const Polygon& polygon)
	{
		return !isEven(polygon);
	}

	bool isNum(const Polygon& polygon, size_t numOfVer)
	{
		return polygon.points.size() == numOfVer;
	}

	bool noCondition(const Polygon&)
	{
		return true;
	}

	template < typename T >
	double calcArea(const std::vector< Polygon >& polygons, T function)
	{
		std::vector< Polygon > newPolygons;
		std::vector< double > areas;
		std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(newPolygons), function);
		std::transform(newPolygons.begin(), newPolygons.end(), std::back_inserter(areas), areaPolygon);
		return std::accumulate(areas.begin(), areas.end(), 0.0);
	}

	bool areaComparator(const Polygon& a, const Polygon& b)
	{
		return areaPolygon(a) < areaPolygon(b);
	}

	bool verComparator(const Polygon& a, const Polygon& b)
	{
		return a.points.size() < b.points.size();
	}

	bool isPermutation(const Polygon& polygon1, const Polygon& polygon2)
	{
		if (polygon1.points.size() != polygon2.points.size())
		{
			return false;
		}
		return std::is_permutation(polygon1.points.begin(), polygon1.points.end(), polygon2.points.begin());
	}

	struct RightAngles
	{
		const Polygon& polygon;
		bool operator()(size_t number) const
		{
			size_t countVertexes = polygon.points.size();
			const Point& previous = polygon.points[(number + countVertexes - 1) % countVertexes];
			const Point& current = polygon.points[number];
			const Point& next = polygon.points[(number + 1) % countVertexes];
			double vector1_x = current.x - previous.x;
			double vector1_y = current.y - previous.y;
			double vector2_x = next.x - current.x;
			double vector2_y = next.y - current.y;
			double scalarProduct = vector1_x * vector2_x + vector1_y * vector2_y;
			return scalarProduct == 0;
		}
	};

	bool isRightAngle(const Polygon& polygon)
	{
		if (polygon.points.size() <= 2)
		{
			return false;
		}
		std::vector< size_t > nums(polygon.points.size());
		std::iota(nums.begin(), nums.end(), 0);
		return std::any_of(nums.begin(), nums.end(), RightAngles{ polygon });
	}
}

double fedorova::areaMean(const std::vector< Polygon >& polygons)
{
	if (polygons.size() == 0)
	{
		throw std::logic_error("<THERE ARE NO POLYGONS>");
	}
	return calcArea(polygons, noCondition) / polygons.size();
}

double fedorova::areaNum(const std::vector< Polygon >& polygons, size_t numOfVer)
{
	using namespace std::placeholders;
	auto function = std::bind(isNum, _1, numOfVer);
	return calcArea(polygons, function);
}

void fedorova::area(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
	std::string subCommand;
	is >> subCommand;
	IoGuard guard(os);
	os << std::fixed << std::setprecision(1);
	using namespace std::placeholders;
	using Predicate = std::function< bool(const Polygon&) >;

	std::map< std::string, std::function< double(const std::vector< Polygon >&) > > subcmds;
	subcmds["EVEN"] = std::bind(calcArea< Predicate >, _1, isEven);
	subcmds["ODD"] = std::bind(calcArea< Predicate >, _1, isOdd);
	subcmds["MEAN"] = areaMean;
	try
	{
		os << subcmds.at(subCommand)(polygons);
	}
	catch (...)
	{
		int numOfVer = std::stoull(subCommand);
		if (numOfVer < 3)
		{
			throw std::logic_error("<WRONG SUBCOMMAND>");
		}
		os << areaNum(polygons, numOfVer);
	}
}

void fedorova::maxArea(const std::vector< Polygon >& polygons, std::ostream& os)
{
	os << areaPolygon(*std::max_element(polygons.begin(), polygons.end(), areaComparator));
}

void fedorova::maxVertexes(const std::vector< Polygon >& polygons, std::ostream& os)
{
	os << (*std::max_element(polygons.begin(), polygons.end(), verComparator)).points.size();
}

void fedorova::max(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
	if (polygons.size() == 0)
	{
		throw std::logic_error("<THERE ARE NO POLYGONS>");
	}
	std::string subcommand;
	is >> subcommand;
	IoGuard guard(os);
	os << std::fixed << std::setprecision(1);

	std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
	subcmds["AREA"] = maxArea;
	subcmds["VERTEXES"] = maxVertexes;
	subcmds.at(subcommand)(polygons, os);
}

void fedorova::minArea(const std::vector< Polygon >& polygons, std::ostream& os)
{
	os << areaPolygon(*std::min_element(polygons.begin(), polygons.end(), areaComparator));
}

void fedorova::minVertexes(const std::vector< Polygon >& polygons, std::ostream& os)
{
	os << (*std::min_element(polygons.begin(), polygons.end(), verComparator)).points.size();
}

void fedorova::min(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
	if (polygons.size() == 0)
	{
		throw std::logic_error("<THERE ARE NO POLYGONS>");
	}
	std::string subcommand;
	is >> subcommand;
	IoGuard guard(os);
	os << std::fixed << std::setprecision(1);

	std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > subcmds;
	subcmds["AREA"] = minArea;
	subcmds["VERTEXES"] = minVertexes;
	subcmds.at(subcommand)(polygons, os);
}

size_t fedorova::countEven(const std::vector< Polygon >& polygons)
{
	return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t fedorova::countOdd(const std::vector< Polygon >& polygons)
{
	return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t fedorova::countNum(const std::vector< Polygon >& polygons, size_t numOfVer)
{
	using namespace std::placeholders;
	return std::count_if(polygons.begin(), polygons.end(), std::bind(isNum, _1, numOfVer));
}

void fedorova::count(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
	std::string subCommand;
	is >> subCommand;
	IoGuard guard(os);
	os << std::fixed << std::setprecision(1);

	std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > subcmds;
	subcmds["EVEN"] = countEven;
	subcmds["ODD"] = countOdd;
	try
	{
		os << subcmds.at(subCommand)(polygons);
	}
	catch (...)
	{
		size_t numOfVer = std::stoull(subCommand);
		if (numOfVer < 3)
		{
			throw std::logic_error("<WRONG SUBCOMMAND>");
		}
		os << countNum(polygons, numOfVer);
	}
}

void fedorova::perms(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
	Polygon polygon;
	is >> polygon;
	if (polygon.points.size() < 3)
	{
		throw std::logic_error("WRONG POLYGON SIZE");
	}
	using namespace std::placeholders;
	os << std::count_if(polygons.begin(), polygons.end(), std::bind(isPermutation, _1, polygon));
}

void fedorova::rightShapes(const std::vector< Polygon >& polygons, std::istream& is, std::ostream& os)
{
	os << std::count_if(polygons.cbegin(), polygons.cend(), isRightAngle);
	return;
}
