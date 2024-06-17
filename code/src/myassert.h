/*
 * myassert.h
 *
 * Simple assert macro to use in place of standard assert which permits tests
 * to continue after failed assert and returns ASEERTION_FAILURE upon failure.
 *
 *      Author: jonl
 *
 */

#ifndef SRC_MYASSERT_H_
#define SRC_MYASSERT_H_

#define ASSERTION_FAILURE 0
#define TEST_SUCCESS 1

#define assert(expr) do {\
        if (!(expr)) {\
            printf("Assertion failed in test %s (%s line %d)\n", __FUNCTION__, __FILE__, __LINE__);\
            return ASSERTION_FAILURE;\
        }\
} while(0)


#endif /* SRC_MYASSERT_H_ */
