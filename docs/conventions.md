# Coding Conventions: 

This document includes important coding and style convetions that contributors to this project should follow. Please go through this document before submitting a Pull Request.


## Style Conventions

**Note:** You need `indent` to be installed on your machine 

We are following something similar to the Linux style convention with some modifications. For fixing any style conventions error, it is enough that you run `make beautiful` which will fix the style for all the files in the project. To learn more visit the [indent docs page](https://www.gnu.org/software/indent/manual/indent.html#SEC2). 


## Nameing conventions 

Please follow these simple naming conventions and guidelines when picking function and variable names. 


#### Function Naming Conventions: 

1. All the functions must have cbt as a prefix. (This may not be applied to helper functions that does not operate on crabtrees or any of its elements).
2. All the words in a function name are lower case.
3. All the words should be seperated by an underscore.
4. Names should be as descriptive as possible. 


#### variables nameing conventions: 

1. All the variables should be lower case
2. Variable could include numbers in their names. 
3. The variable name must describe what it stores and why is it there.


#### Test Naming Conventions:

1. The the prefix of the test name will be the name the of the function being tested
2. The suffix will be test(n) where n is the index of the test written for this function.

For example, the name of the tests for cbt_insert should be: 

1. cbt_insert_test1
2. cbt_insert_test2
3. cbt_insert_test3