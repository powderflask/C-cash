/**
 *  Block ADT : private implementation / algorithms
 *
 *  COMP220: Assignment 3
 *  Author:  
 *  Date:    Feb. 1, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "trnslist.h"
#include "puzzle.h"
#include "block.h"

/***********************
 * BLOCK private implementation details
 ***********************/

// Dynamic transactions in t are owned by this Block and will be deleted when this Block is deleted!
Block_t* blkCreate(TransactionList t, int difficulty, Nonce_t proof_of_work ) {
   static id = 0;
   id++;
   Block_t* block = malloc(sizeof(Block_t));
   block->id = id;
   block->transactions = t;
   block->proof_of_work = proof_of_work;
   block->difficulty = difficulty;
   strcpy(block->hash, NULL_HASH);
   block->prev = block->next = NULL;
   return block;
}

void blkDelete(Block_t* block) {
   tlistDelete(&block->transactions);
   free(block);
}

void blkSerializeTransactions(const Block_t block, char* buf) {
   tlistSerialize(block.id, block.transactions, buf);
}

// It's up to the caller to delete the Puzzle that is returned.
Puzzle_t blkCreatePuzzle(const Block_t block, const Hash_t prev_hash) {
   char buf[tlistSerialLen(block.transactions)+1];
   blkSerializeTransactions(block, buf);
   return puzzleCreate(buf, prev_hash, block.difficulty);
}

bool blkValidates(const Block_t block, const Hash_t prev_hash, Nonce_t proof_of_work) {
   // Use the Puzzle logic to check the proof_of_work - DRY
   Puzzle_t puzzle = blkCreatePuzzle(block, prev_hash);
   bool sovled = puzzleIsSovedBy(puzzle, proof_of_work);
   puzzleDelete(&puzzle);
   return sovled;
}

bool blkIsValid(const Block_t block) {
   return blkValidates(block, blkPrevHash(block), block.proof_of_work);
}

// Chain new_block to the tail_block IFF it validates on that chain, and compute new_block's Hash
// Fail and return false if the new_block does not validate on this chain, return true if it does.
bool blkChainTo(Block_t* tail_block, Block_t* new_block) {
   assert(tail_block->next == NULL);  // don't allow chaining to an occupied link
   if (blkValidates(*new_block, tail_block->hash, new_block->proof_of_work)) {
      tail_block->next = new_block;
      new_block->prev = tail_block;
      blkComputeHash(new_block);
      return true;
   }
   return false;
}

void blkComputeHash(Block_t* block) {
   // Use the puzzle to build the appropriate Hash for the block - DRY
   Puzzle_t puzzle = blkCreatePuzzle(*block, blkPrevHash(*block));
   puzzleHash(puzzle, block->proof_of_work, block->hash);
   puzzleDelete(&puzzle);
}

const char* blkPrevHash(const Block_t block) {
   if (block.prev != NULL) {
      return block.prev->hash;
   }
   else {
      return NULL_HASH;
   }
}
