#include <math.h>
#include <limits.h>
#include <float.h>
#include "unity_fixture.h"

TEST_GROUP(test_pow);

TEST_SETUP(test_pow)
{
}

TEST_TEAR_DOWN(test_pow)
{
}

/*TESTS*/

//edge cases
TEST(test_pow, pow_edge)
{
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(0, 0.3));

	TEST_ASSERT_DOUBLE_IS_INF(pow(0, -0.3));
	TEST_ASSERT_DOUBLE_IS_NEG_INF(pow(-0, -0.3));

	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0.3, 0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0.3, -0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(-0.3, 0));

	/*
	//Value of 0^0 is 1 or 0 (no consensus)
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(0, 0));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(-0, 0));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(0, -0));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(-0, -0));
	*/
	//or
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0, 0));
}

//q1 - all positive quater in 2D Cartesian coordinate system (counting anticlockwise)
TEST(test_pow, pow_q1)
{
	//base < 1
	TEST_ASSERT_EQUAL_DOUBLE(0.696845301935949, pow(0.3, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(0.3, pow(0.3, 1));
	TEST_ASSERT_EQUAL_DOUBLE(0.23580092567898683, pow(0.3, 1.2));
	TEST_ASSERT_EQUAL_DOUBLE(0.0081, pow(0.3, 4));
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(0.3, DBL_MAX));

	//base 1
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, 1));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, 1.2));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, DBL_MAX));

	//base > 1
	TEST_ASSERT_EQUAL_DOUBLE(1.0562199684392581, pow(1.2, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(1.2, pow(1.2, 1));
	TEST_ASSERT_EQUAL_DOUBLE(1.244564747204, pow(1.2, 1.2));
	TEST_ASSERT_DOUBLE_IS_INF(pow(1.2, DBL_MAX));
	TEST_ASSERT_EQUAL_DOUBLE(DBL_MAX, pow(DBL_MAX, 1));
	TEST_ASSERT_DOUBLE_IS_INF(pow(DBL_MAX, 1.2));
}


TEST_GROUP_RUNNER(test_pow)
{
	RUN_TEST_CASE(test_pow, pow_edge);
	RUN_TEST_CASE(test_pow, pow_q1);
}

void runner(void)
{
	RUN_TEST_GROUP(test_pow);
}

int main(int argc, char *argv[])
{
	UnityMain(argc, (const char **)argv, runner);
	return 0;
}