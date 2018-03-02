/**
 *  Transaction LIST ADT : automatated unit test driver
 *
 *  COMP220: Assignment 3
 *  Author:  Joseph Fall
 *  Date:    Feb. 1, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "trnslist.h"

/*
* Run a single test case
* Report on failed test
* Return 0 if test successful, 1 otherwise
*/

int run_test(int result, int expected, char* msg) 
{
   if (result != expected)
   {
      printf("\nTest failed: %s \n\tFunction returned %d but expected %d\n", msg, result, expected);
      return(1);
   }
   return(0);
}

/*
 * Run a series of unit tests cases.
 * Report on failed tests.
 */
void main()
{
   int i;
   int failed_test_counter = 0;
   printf("Unit Test Suite for TransactionList ADT operations...\n");
   
   // Test 1: Constructor
   TransactionList list = tlistCreate();
   printf("1");
   failed_test_counter += run_test(tlistLen(list), 0, "Constructor fails to create empty list.");
   printf(".");
   
   
   // Test 2: Append Transactions
   for (i=0; i<5; i++) {
      tlistAppend(&list, "Dave", 12.34, "Cici");
   }
   printf("2");
   failed_test_counter += run_test(tlistLen(list), 5, "List length incorrect after Appending 5 items.");
   printf(".  List should have 5 records:\n");
   tlistPrint(list);

   // Test 3: Serialize the list
   printf("3");
   int slen = tlistSerialLen(list);
   printf("\nAllocating bytes to serialize list: %d\n", slen);
   char buf[slen];
   buf[0] = '\0';
   tlistSerialize(99, list, buf);
   failed_test_counter += run_test(strlen(buf)<=slen, true, "Serialized List exceeds expected length.");
   printf("\nSerialized List: %s\n\n", buf);
   
   // Test 4: Destructor (white-box tests)
   printf("4");
   tlistDelete(&list);
   failed_test_counter += run_test(tlistLen(list), 0, "Destructor fails to empty the list.");
   printf(".");
   failed_test_counter += run_test(list.capacity, 0, "Destructor fails to set list capacity to zero.");
   printf(".");
   failed_test_counter += run_test((long int)list.data, (long int)NULL, "Destructor fails to set list data pointer to NULL.");
   printf(".  List should be empty: \n");
   tlistPrint(list);


   // Test 5: Append beyond capacity (white-box tests)
   tlistAppend(&list, "Dave", 12.34, "Cici");
   int new_size = list.capacity*3+2;
   for (i=1; i<new_size; i++) {
      tlistAppend(&list, "Donny", 98.76, "Connie");
   }
   printf("5");
   failed_test_counter += run_test(tlistLen(list), new_size, "List length incorrect after Appending beyond its capacity.");
   printf(".  List should have %d records\n", new_size);
   tlistPrint(list);

   tlistDelete(&list);
   printf(".  List should be empty\n");
   tlistPrint(list);
   printf("\n");
   
   printf("\n\nUnit Test Suite Complete: ");
   if (failed_test_counter == 0) printf("ALL TESTS PASSED\n");
   else printf("FAILED %d TESTS\n", failed_test_counter);
   
}