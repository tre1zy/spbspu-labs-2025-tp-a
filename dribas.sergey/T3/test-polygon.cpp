#include <boost/test/unit_test.hpp>
#include "poligon.hpp"
#include <sstream>

BOOST_AUTO_TEST_SUITE(PointTest)

BOOST_AUTO_TEST_CASE(CorrectPointInput)
{
    dribas::Point p;
    std::istringstream iss("(123;456)");
    iss >> p;
    BOOST_CHECK(iss.good());
    BOOST_CHECK_EQUAL(p.x, 123);
    BOOST_CHECK_EQUAL(p.y, 456);
}

BOOST_AUTO_TEST_CASE(IncorrectPointInput)
{
    dribas::Point p;
    std::istringstream iss("123;456)");
    iss >> p;
    BOOST_CHECK(iss.fail());

    iss.clear();
    iss.str("(123:456)");
    iss >> p;
    BOOST_CHECK(iss.fail());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PoligonTest)

BOOST_AUTO_TEST_CASE(CorrectPoligonInput)
{
    dribas::Poligon p;
    std::istringstream iss("3 (0;0) (1;1) (2;2)");
    iss >> p;
    BOOST_CHECK(iss.good());
    BOOST_CHECK_EQUAL(p.points.size(), 3);
}

BOOST_AUTO_TEST_CASE(InvalidPointCount)
{
    dribas::Poligon p;
    std::istringstream iss("2 (0;0) (1;1)");
    iss >> p;
    BOOST_CHECK(iss.fail());
}

BOOST_AUTO_TEST_CASE(IncorrectPointsInPoligon)
{
    dribas::Poligon p;
    std::istringstream iss("3 (0;0) (1;1) invalid");
    iss >> p;
    BOOST_CHECK(iss.fail());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AreaCalculationTest)

BOOST_AUTO_TEST_CASE(RectangleArea)
{
    dribas::Poligon rect;
    rect.points = {{0,0}, {2,0}, {2,2}, {0,2}};
    const double area = dribas::getPoligonArea(rect);
    BOOST_CHECK_CLOSE(area, 4.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(TriangleArea)
{
    dribas::Poligon triangle;
    triangle.points = {{0,0}, {2,0}, {0,2}};
    const double area = dribas::getPoligonArea(triangle);
    BOOST_CHECK_CLOSE(area, 2.0, 1e-6);
}

BOOST_AUTO_TEST_SUITE_END()