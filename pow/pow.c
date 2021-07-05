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
	if (SKIP_FAILING <= 0)
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
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

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
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

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
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

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
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

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

//base=0 e>0
TEST(test_pow, pow_b0egt0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(0, 8.5070074779947304409e-324);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 5.4700889228634501649e-314);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 2.9632080560777316336e-310);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 2.2250738585072009e-308); //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 8.5070074779947304409e-324);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 5.4700889228634501649e-314);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 2.9632080560777316336e-310);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 2.2250738585072009e-308);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	//normal
	v = pow(0, 7.4469280707415617115e-300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 1.8895501503254452658e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 6.8677543336531501339);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 1.8351115573726972663e3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 3.6848459649033649121e300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 7.4469280707415617115e-300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 1.8895501503254452658e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 6.8677543336531501339);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 1.8351115573726972663e3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 3.6848459649033649121e300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
}

//base=0 e<0
TEST(test_pow, pow_b0elt0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(0, -8.5070074779947304409e-324);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -5.4700889228634501649e-314);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -2.9632080560777316336e-310);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -2.2250738585072009e-308);  //max subnormal
	TEST_ASSERT_DOUBLE_IS_INF(v);

	if (SKIP_FAILING <= 0) {
		v = pow(NEGATIVE_ZERO, -8.5070074779947304409e-324);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -5.4700889228634501649e-314);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -2.9632080560777316336e-310);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -2.2250738585072009e-308);  //max subnormal
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
	}

	//normal
	v = pow(0, -7.4469280707415617115e-300);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -1.8895501503254452658e-10);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -6.8677543336531501339);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -1.8351115573726972663e3);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -3.6848459649033649121e300);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	if (SKIP_FAILING <= 0) {
		v = pow(NEGATIVE_ZERO, -7.4469280707415617115e-300);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -1.8895501503254452658e-10);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -6.8677543336531501339);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -1.8351115573726972663e3);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -3.6848459649033649121e300);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
	}
}

//base>0 e=0
TEST(test_pow, pow_bgt0e0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(8.5070074779947304409e-324, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(5.4700889228634501649e-314, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.9632080560777316336e-310, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.2250738585072009e-308, 0);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(8.5070074779947304409e-324, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(5.4700889228634501649e-314, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.9632080560777316336e-310, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.2250738585072009e-308, NEGATIVE_ZERO);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	//normal
	v = pow(7.4469280707415617115e-300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8895501503254452658e-10, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.8677543336531501339, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8351115573726972663e3, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(3.6848459649033649121e300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(7.4469280707415617115e-300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8895501503254452658e-10, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.8677543336531501339, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8351115573726972663e3, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(3.6848459649033649121e300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
}

//base<0 e=0
TEST(test_pow, pow_blt0e0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(-8.5070074779947304409e-324, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-5.4700889228634501649e-314, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.9632080560777316336e-310, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.2250738585072009e-308, 0);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(-8.5070074779947304409e-324, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-5.4700889228634501649e-314, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.9632080560777316336e-310, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.2250738585072009e-308, NEGATIVE_ZERO);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	//normal
	v = pow(-7.4469280707415617115e-300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8895501503254452658e-10, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-6.8677543336531501339, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8351115573726972663e3, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-3.6848459649033649121e300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(-7.4469280707415617115e-300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8895501503254452658e-10, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-6.8677543336531501339, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8351115573726972663e3, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-3.6848459649033649121e300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
}

/*
* q1 - all positive quater in 2D Cartesian coordinate system (counting anticlockwise)
* b>0 e>0
*/
TEST(test_pow, pow_q1)
{
	double v;  //for temporary use

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

	v = pow(1.2293932197128487616, 4.919211108097790941);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.7619006135271186864, v);
	v = pow(2.3434171654580540078, 8.8294451971292708947);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1843.1761167746826686, v);
	v = pow(6.7015557734313127014, 5.0037729932387442489);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 13614.300641159401493, v);
	v = pow(0.061919319416988492788, 7.4026446028192118831);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1.1384620257956381893e-09, v);
	v = pow(0.046303816698153560383, 4.4591320302676935583);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1.1215253348327877321e-06, v);
	v = pow(0.076025103632163651413, 1.6677585188713122122);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.013605250376588693545, v);
	v = pow(0.088725097950332315611, 2.3373472793172478937e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999999943384487722, v);
	v = pow(0.018497307594293105337, 3.979277920444295843e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999999841221631769, v);
	v = pow(0.025849472203437973139, 4.9175011041281912226e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999999820242468918, v);
	v = pow(5.3130117203428532903e-200, 6.5848441614424670233e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999969785653908261, v);
	v = pow(6.3322233072206948246e-200, 2.4250596352027023941e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999988876946432903, v);
	v = pow(2.0241151977613807665e-200, 8.9926431580908856471e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.9999995865076645174, v);
	v = pow(4.426023943835088241e-300, 4.856176488254850071e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999966526962535962, v);
	v = pow(1.9648929063772985563e-300, 5.7160598775604715866e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999960553473410396, v);
	v = pow(2.8095014380214060567e-300, 4.0769529949552865165e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999971879525595408, v);
	v = pow(3.0858239924437172473e-300, 5.7548500053146474646e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.180100741921854123e-300, 4.4020745617130794726e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(3.5017510945649241719e-300, 2.2918779544940000592e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(8.6615515187326650152e-300, 2.925656283841282048e+20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(6.4089784309100145385e-300, 3.3125165733958136627e+20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(6.3744653233137211113e-300, 6.5611239964138969498e+20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(4.3947923185230016435e-300, 4.6873309282317120247e+200);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(7.1612778288688736481e-300, 3.5797744574942288385e+200);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(7.277914358085932091e-300, 4.7708572296848924635e+200);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(3.299985384529049751e-300, 1.7289077094922795551e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(5.6096750453040556797e-300, 6.4669059463572243246e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(5.3727449167225824969e-300, 4.4058307349695032198e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(3.801744107528840151e+20, 6.2960767868730766401e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(4.3292687197583717171e+20, 7.7354332215304716487e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(7.6633345526017255014e+20, 3.0515279378331787359e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(6.768372964638490501e+250, 5.1799624462168174661e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(8.9496369929668159799e+250, 2.749413059197071004e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(1.8463861860018252425e+250, 1.877579716185553432e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);

	v = pow(5.2427537471429067573e+250, 7.8891184911466593608e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(4.8788266684168152085e+250, 4.1476508897221315154e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.3714491173922081684e+250, 6.9300635476336526611e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.6323794426741273502e+250, 4.5384433070669668239e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.1566209884265496683e+250, 3.6468630417125682143e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
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


TEST(test_pow, pow_quick) {
	// MAX_INT <= e (INT) <= MIN_INT
	// b!=0 e!=0

	double v;  //for temporary use
	//edge
	v = pow(-NAN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, INT_MIN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-1, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(-DBL_TRUE_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	v = pow(NAN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, INT_MIN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(1, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(DBL_TRUE_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	v = pow(-NAN, INT_MAX);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, INT_MAX);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, INT_MAX);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v); //got INF
	v = pow(-1, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -1, v);
	v = pow(-DBL_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-DBL_TRUE_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NAN, INT_MAX);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, INT_MAX);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, INT_MAX);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(1, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(DBL_TRUE_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	//e = +-1
	v = pow(-NAN, 1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, 1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -DBL_MAX, v);
	v = pow(-1, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -1, v);
	v = pow(-DBL_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -DBL_MIN, v);
	v = pow(-DBL_TRUE_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -DBL_TRUE_MIN, v);

	v = pow(NAN, 1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, 1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, DBL_MAX, v);
	v = pow(1, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, DBL_MIN, v);
	v = pow(DBL_TRUE_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, DBL_TRUE_MIN, v);

	v = pow(-NAN, -1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, -1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -5.562684646268003e-309, v);
	v = pow(-1, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -1, v);
	v = pow(-DBL_MIN, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -4.494232837155790e+307, v);
	v = pow(-DBL_TRUE_MIN, -1);
	TEST_ASSERT_DOUBLE_IS_NEG_INF(v);

	v = pow(NAN, -1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, -1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 5.562684646268003e-309, v);
	v = pow(1, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 4.494232837155790e+307, v);
	v = pow(DBL_TRUE_MIN, -1);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	//subnormals
	v = pow(-8.5070074779947304409e-324, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(5.4700889228634501649e-314, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(2.9632080560777316336e-310, 10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-DBL_TRUE_MIN, 8);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(2.2250738585072009e-308, -2);  //max subnormal
	TEST_ASSERT_DOUBLE_IS_INF(v);

	//normal
	v = pow(-7.4469280707415617115e-300, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(-1.8895501503254452658e-10, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(6.8677543336531501339, 10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.334250799601485e+08, v);
	v = pow(-6.8677543336531501339, 9);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -3.398855996003328e+07, v);
	v = pow(1.8351115573726972663e3, -2);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.969443451714097e-07, v);
	v = pow(DBL_MAX, 4);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(DBL_MIN, 4);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
}


TEST_GROUP_RUNNER(test_pow)
{
	RUN_TEST_CASE(test_pow, pow_edge);
	RUN_TEST_CASE(test_pow, pow_b0egt0);
	RUN_TEST_CASE(test_pow, pow_b0elt0);
	RUN_TEST_CASE(test_pow, pow_bgt0e0);
	RUN_TEST_CASE(test_pow, pow_blt0e0);
	RUN_TEST_CASE(test_pow, pow_q1);
	RUN_TEST_CASE(test_pow, pow_q2);
	RUN_TEST_CASE(test_pow, pow_q3);
	RUN_TEST_CASE(test_pow, pow_q4);
	RUN_TEST_CASE(test_pow, pow_quick);
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