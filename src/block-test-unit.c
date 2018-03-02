/**
 *  BLOCK ADT : automatated unit test driver
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
#include "puzzle.h"
#include "block.h"


#define MINING_DIFFICULTY 2  // Increase to make mining harder (test driver will run slooooower!)

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
   printf("Unit Test Suite for block chain BLOCK ADT operations...\n");
   
   TransactionList t1 = tlistCreate();
   for (i=0; i<5; i++) {
      tlistAppend(&t1, "Dave", (i+3)*2+99.0/(i+1), "Circi");
   }

   // Test 1: Constructor
   printf("1");
   Block_t* block1 = blkCreate(t1, MINING_DIFFICULTY, NULL_NONCE);
   printf(".");

   // Test 2: CreatePuzzle
   printf("2");
   Puzzle_t puzzle = blkCreatePuzzle(*block1, NULL_HASH);
   printf(".");
   
   // Test 3: Validate
   printf("3");
   block1->proof_of_work = puzzleMine(puzzle);
   printf(".");
   bool valid = blkIsValid(*block1);
   failed_test_counter += run_test(valid, true, "Block does not validate with mined PoW");
   printf(".");
   
   // Test 4: Set Block Hash
   printf("4");
   blkComputeHash(block1);
   failed_test_counter += run_test(strcmp(block1->hash, NULL_HASH)==0, false, "Block hash is not updated.");
   printf(".");
   
   // Test 5: Link Blocks
   printf("5");
   TransactionList t2 = tlistCreate();
   for (i=0; i<5; i++) {
      tlistAppend(&t2, "Jan", (i+2)*3+69.0/(i+2), "Bob");
   }
   Block_t* block2 = blkCreate(t2, MINING_DIFFICULTY, NULL_NONCE);
   printf(".");
   puzzle = blkCreatePuzzle(*block2, block1->hash);
   printf(".");
   block2->proof_of_work = puzzleMine(puzzle);
   printf(".");
   bool linked = blkChainTo(block1, block2);
   printf(".");
   failed_test_counter += run_test(linked, true, "Block2 did not successfully link to Block1");
   failed_test_counter += run_test(strcmp(block1->hash, block2->hash)==0, false, "Block hashes are identical.");
   failed_test_counter += run_test(block1->next==block2, true, "Block next link not set correctly.");
   failed_test_counter += run_test(block2->prev==block1, true, "Block prev link not set correctly.");
   printf(".");
   
   // Test 6: Destructor (white-box tests)
   printf("6");
   blkDelete(block1);
   block1 = NULL;
   blkDelete(block2);
   block2 = NULL;
   printf(".");


   printf("\n\nUnit Test Suite Complete: ");
   if (failed_test_counter == 0) printf("ALL TESTS PASSED\n");
   else printf("FAILED %d TESTS\n", failed_test_counter);
   
}