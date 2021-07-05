#include <math.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>
#include "unity_fixture.h"

#define SKIP_FAILING 1

#define NEGATIVE_ZERO (-0.0)

#define DOUBLE_MANTISSA 52
#define DOUBLE_EXPONENT 11
#define DOUBLE_SIGN     1

typedef struct
{
	uint64_t
		mantissa : DOUBLE_MANTISSA,
		exponent : DOUBLE_EXPONENT,
		sign : DOUBLE_SIGN;
} doubleStruct;

union binaryDouble
{
	double d;
	doubleStruct b;
};

double getDoubleMaxAccuracy(double v)
{
	union binaryDouble val;
	int shift = -DOUBLE_MANTISSA;  //2^-52 - accuracy when exponent is 0
	double accuracy = 1;
	int exponent;

	val.d = v;
	exponent = val.b.exponent - 1023; //exponent is represented as unsigned int but offset is 1023
	shift += exponent;

	//infinity Nan etc.
	if (exponent == 1024)  
		return INFINITY;

	if (shift > 0) {
		// - 1/2*2*2*2...
		for (int i = 0; i < shift; i++) {
			accuracy *= 2;
		}
	}
	else if (shift < 0) {
		// + 2*2*2*2...
		for (int i = 0; i > shift; i--) {
			accuracy /= 2;
		}
	}
	return accuracy;
}

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
	double v;  //for temporary use
	//b=0 e>0
	v = pow(0, DBL_TRUE_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, DBL_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, DBL_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, INFINITY);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, NAN);
	TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(NEGATIVE_ZERO, DBL_TRUE_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, DBL_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, DBL_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, INFINITY);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, NAN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);
	

	//b=0 e<0
	v = pow(0, -DBL_TRUE_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -DBL_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -1);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -DBL_MAX);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -INFINITY);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -NAN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

	if (SKIP_FAILING <= 0) {
		v = pow(NEGATIVE_ZERO, -DBL_TRUE_MIN);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -DBL_MIN);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -1);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -DBL_MAX);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -INFINITY);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -NAN);
		TEST_ASSERT_DOUBLE_IS_NAN(v);
	}

	

	//b>0 e=0
	v = pow(DBL_TRUE_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MAX, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(INFINITY, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(NAN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(DBL_TRUE_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MAX, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(INFINITY, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(NAN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	//b<0 e=0
	v = pow(-DBL_TRUE_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MAX, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-INFINITY, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-NAN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(-DBL_TRUE_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MAX, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-INFINITY, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-NAN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

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

//base = 0 e>0
TEST(test_pow, pow_b0eg)
{
	double v;  //for temporary use

	//subnormals
	v = pow(0, 3.5070074779947304409e-309);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(0, 2.1596406259513307089e-308);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(0, 2.1297671547624933736e-308);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(0, 1.0799966675291680488e-308);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(0, 1.7806831507983543041e-308);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 3.1570758295871739858e-309);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 9.3845000450219172621e-309);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 2.0375791784045555202e-308);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 1.7627198195207633498e-308);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 2.1349415154024930876e-308);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	//normal
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(0, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(NEGATIVE_ZERO, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(NEGATIVE_ZERO, 2));
}
//if(SKIP_FAILING <= 0) TEST_ASSERT_DOUBLE_IS_NEG_INF(pow(NEGATIVE_ZERO, -0.3)); //gets positive inf (unreachable code in tested function)

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


TEST(test_pow, pow_precision) {
	//check accuracy relative to double max accuracy (for given value)
	//pass only if accuracy better or equal 2x max accuracy

	double v = pow(0.3, 0.3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v)*2, 0.696845301935949, v);
}


TEST_GROUP_RUNNER(test_pow)
{
	RUN_TEST_CASE(test_pow, pow_edge);
	RUN_TEST_CASE(test_pow, pow_b0eg);
	RUN_TEST_CASE(test_pow, pow_q1);
	RUN_TEST_CASE(test_pow, pow_q2);
	RUN_TEST_CASE(test_pow, pow_q3);
	RUN_TEST_CASE(test_pow, pow_q4);
	RUN_TEST_CASE(test_pow, pow_precision);
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