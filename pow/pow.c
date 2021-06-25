#include <math.h>
#include <limits.h>
#include <float.h>
#include "unity_fixture.h"

#define NEGATIVE_ZERO (-0.0)

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
	//b=0 e>0
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(0, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(NEGATIVE_ZERO, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(NEGATIVE_ZERO, 2));

	//b=0 e<0
	TEST_ASSERT_DOUBLE_IS_INF(pow(0, -0.3));
	TEST_ASSERT_DOUBLE_IS_INF(pow(0, -2));
	TEST_ASSERT_DOUBLE_IS_NEG_INF(pow(NEGATIVE_ZERO, -0.3)); //gets positive inf (unreachable code in tested function)

	//b!=0 e=0
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0.3, 0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0.3, NEGATIVE_ZERO));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(-0.3, 0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(-0.3, 2));

	/*
	//Value of 0^0 is 1 or 0 (no consensus)
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(0, 0));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(NEGATIVE_ZERO, 0));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(0, NEGATIVE_ZERO));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(NEGATIVE_ZERO, NEGATIVE_ZERO));
	*/
	//or
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0, 0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(NEGATIVE_ZERO, 0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0, NEGATIVE_ZERO));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(NEGATIVE_ZERO, NEGATIVE_ZERO));
}

/*
* q1 - all positive quater in 2D Cartesian coordinate system (counting anticlockwise)
* b>0 e>0
*/
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

/*
* b<0 e>0
*/
TEST(test_pow, pow_q2)
{
	//imaginary numbers
	TEST_ASSERT_FLOAT_IS_NAN(pow(-2, 0.3));
	TEST_ASSERT_FLOAT_IS_NAN(pow(-2, 1.4));

	//positive or negative check
	TEST_ASSERT_EQUAL_DOUBLE(16, pow(-2, 4));
	TEST_ASSERT_EQUAL_DOUBLE(-8, pow(-2, 3));
}

/*
* b<0 e<0
*/
TEST(test_pow, pow_q3)
{
	//imaginary numbers
	TEST_ASSERT_FLOAT_IS_NAN(pow(-2, -0.3));
	TEST_ASSERT_FLOAT_IS_NAN(pow(-2, -1.4));

	//positive or negative check
	TEST_ASSERT_EQUAL_DOUBLE(0.0625, pow(-2, -4));
	TEST_ASSERT_EQUAL_DOUBLE(-0.125, pow(-2, -3));
}

/*
* b>0 e<0
*/
TEST(test_pow, pow_q4)
{
	//positive or negative check
	TEST_ASSERT_EQUAL_DOUBLE(0.0625, pow(2, -4));
	TEST_ASSERT_EQUAL_DOUBLE(0.7192230933248643, pow(3, -0.3));
}

int check_precision(char expected[], char actual[]) {
	//todo: safety of iterator
	int i;
	for (i = 0; i < 20; i++) {
		if (expected[i] != actual[i])
			return i;
	}
	return i;
}

TEST(test_pow, pow_precision) {
	//concept only
	char expected[25] = "0.6968453019359489301421";
	char actual[25];
	fprintf(actual, "%.20f", pow(0.3, 0.3));
	printf("Precision %i",check_precision(expected, actual));
}

TEST_GROUP_RUNNER(test_pow)
{
	RUN_TEST_CASE(test_pow, pow_edge);
	RUN_TEST_CASE(test_pow, pow_q1);
	RUN_TEST_CASE(test_pow, pow_q2);
	RUN_TEST_CASE(test_pow, pow_q3);
	RUN_TEST_CASE(test_pow, pow_q4);
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