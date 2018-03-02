/**
 *  Cyrptographic PUZZLE ADT : automatated unit test driver
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

#include "puzzle.h"

/*
* Run a single test case
* Report on failed test
* Return 0 if test successful, 1 otherwise
*/

int run_test(long int result, long int expected, char* msg) 
{
   if (result != expected)
   {
      printf("\nTest failed: %s \n\tFunction returned %ld but expected %ld\n", msg, result, expected);
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
   printf("Unit Test Suite for Puzzle ADT operations...\n");
   
   const char* dummy_hash = "12345678901234567890";
   
   // Test 1: Constructor
   Hash_t hash;
   strcpy(hash, dummy_hash);
   Puzzle_t puzzle = puzzleCreate("Some data for Puzzle", hash, 3);
   printf("1");
   printf(".  After create:\n");
   puzzlePrint(puzzle);
   

   // Test 2: Compute a hash of the puzzle with a proof-of-work
   printf("2");
   Nonce_t pow;
   pow.i_nonce = 987654321;
   puzzleHash(puzzle, pow, hash);
   failed_test_counter += run_test(strlen(hash), HASH_DIGEST_SIZE, "puzzleHash has incorrect length");
   failed_test_counter += run_test(strcmp(hash, dummy_hash)==0, false, "Hash was not set by puzzleHash");
   printf(".  Hash of puzzle with incorrect PoW:\n");
   printHash(hash);
  
   // Test 3: Mine the puzzle
   printf("3");
   pow = puzzleMine(puzzle);
   failed_test_counter += run_test((long int)pow.i_nonce, (long int)399977, "Incorrect proof of work returned.");
   printf("\nProof of Work for puzzle: %ld\n\n", pow.i_nonce);
   printf(".  Hash of puzzle with PoW:\n");
   puzzleHash(puzzle, pow, hash);
   printHash(hash);
   printf(".");
   
   // Test 4: isSolvedBy
   printf("4");
   failed_test_counter += run_test(puzzleIsSovedBy(puzzle, pow), true, "Mined proof-of-work does not solve puzzle.");
   printf(".");

   // Test 5: Destructor (white-box tests)
   printf("5");
   puzzleDelete(&puzzle);
   failed_test_counter += run_test((long int)puzzle.data_buffer, (long int)NULL, "Destructor fails to free data buffer.");
   printf(".");


   printf("\n\nUnit Test Suite Complete: ");
   if (failed_test_counter == 0) printf("ALL TESTS PASSED\n");
   else printf("FAILED %d TESTS\n", failed_test_counter);

}