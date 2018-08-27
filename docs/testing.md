#Testing Using CUNIT

This document explains how to write tests for this project using the [CUnit framework](http://cunit.sourceforge.net) for unit testing. Please note, this document does not cover the interface for the CUnit and you are highly encouraged to go through the CUnit documentations before writing any tests.

##Prerequisites

You will need to install the [CUnit library](https://sourceforge.net/projects/cunit/) before compiling your tests.
 

#Test Structure 

The structure is fairly simple. There are two main components for testing: 

1. The main file that is responsible for running all the tests you have written (can be found at [test/test.c](../test/test.c))
2. The test suites files that contains the actual tests.

Currently we have three main test suites: 

1. Unit tests for the interface. [test/testsuite.c](../test/testsuite.c)
2. Unit tests for helpers. [test/helpers_testsuite.c](../test/helpers_testsuite.c)
3. Comprehensive tests.

Accoridng to what type of test you are creating, choose where to place your test. 

A **test** is simply a function created to test some feature in the project. All the tests must have the following format: 

```
static void name_of_function_to_be_tested_test_n(void)
{
	
}
```
**Look at the [conventions](conventions.md) for naming a test**.

## After creating the function, you will have to include it with the other tests. Fortunately, this is also easy:

1. Open [test/test.c](test/test.c)
2. You will find two arrays at the top of the page: utests and ctests (comprehensive tests).
3. Choose one according to what type of test are you adding.
4. Add the following to the array {"test_name", test_name}, 

For example, adding name_of_function_to_be_tested_test_n to the tests: 

```
static struct test utests[] = {
	{"test1", test1}
	{"test2", test2},
	{"test3", test3},
	....
	....
	{NULL, NULL}
} 

```

will be 

```
static struct test utests[] = {
	{"test1", test1}
	{"test2", test2},
	{"test3", test3},
	....
	{"name_of_function_to_be_tested_test_n", name_of_function_to_be_tested_test_n},
	{NULL, NULL}
} 

```
