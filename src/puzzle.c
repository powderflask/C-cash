/**
 *  Cryptographic Puzzle ADT : private implementation / algorithms
 *
 *  COMP220: Assignment 3
 *  Author:  
 *  Date:    Feb. 1, 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "sha1/sha1.h"
#include "puzzle.h"

const Hash_t NULL_HASH = {};           // A special null-hash for the first Block in the chain
const Nonce_t NULL_NONCE = {};         // A special null-nonce for a default or initializing a Nonce

/***********************
 * PUZZLE private implementation details
 ***********************/
/*
 * Constructor - return a new Puzzle whose solution "locks" the given 2 data items
 * @param buf - the data buffer to be locked  (Puzzle makes a copy of the caller's data)
 * @param prev_hash - additional hash to be locked with the data_buffer (Puzzle makes a copy of the caller's data)
 * @param difficulty - level of difficulty set for this Puzzle - increaing difficulty makes Puzzle harder to "mine"
 * PRE: 0 < difficulty < HASH_DIGEST_SIZE/2
 */
const Puzzle_t puzzleCreate(const char* buf,  const Hash_t prev_hash, int difficulty) {
   Puzzle_t puzzle;
   puzzle.data_buffer = malloc((strlen(buf) + 1) * sizeof(char));
   strcpy(puzzle.data_buffer, buf);
   strcpy(puzzle.prev_hash, prev_hash);
   puzzle.difficulty = difficulty;
   return puzzle;
}

/*
 * Destructor - remove all data and free all memory associated with the puzzle 
 */
void puzzleDelete( Puzzle_t *puzzle ) {
   free(puzzle->data_buffer);
   puzzle->data_buffer = NULL;
   strcpy(puzzle->prev_hash, NULL_HASH);
}

/*
 * Print a text representation of the puzzle on STDOUT
 */
void puzzlePrint( const Puzzle_t puzzle  ) {
   printf("Puzzle (%d): <%s> + <%s>\n", puzzle.difficulty, puzzle.data_buffer, puzzle.prev_hash);
}

/*
 * Print a hash as hex.
 */
void printHash( Hash_t hash ) {
   /* Print the digest as one long hex value */ 
   int n;
   printf("\n HASH: 0x"); 
   for (n = 0; n < HASH_DIGEST_SIZE; n++)
      printf("%02x", (uint8_t)hash[n]);   
   printf("\n\n");
} 

/*
 * Hash the puzzle data along with the given proof-of-work
 * @param puzzle : the puzzle data to be hashed
 * @param proof-of-work:  a Nonce to be hashed with the puzzle data
 * @param hash: OUTPUT - the SHA1 hash result
 */
void puzzleHash(const Puzzle_t puzzle, const Nonce_t proof_of_work, Hash_t hash) {
   // cat puzzle.data, puzzle.prev_hash, proof_of_work.c_nonce (careful - not null term.)
   // strcpy(hash, "00004567890123456789");  // STUB
   SHA1_CTX sha; 
   uint8_t results[HASH_DIGEST_SIZE]; 

   SHA1Init(&sha); 
   //SHA1Update(&sha, (uint8_t *)"abc", 3); 
   SHA1Update(&sha, (uint8_t *)puzzle.data_buffer, strlen(puzzle.data_buffer)); 
   SHA1Update(&sha, (uint8_t *)puzzle.prev_hash, strlen(puzzle.prev_hash)); 
   SHA1Update(&sha, (uint8_t *)proof_of_work.c_nonce, NONCE_BYTES); 
   SHA1Final(results, &sha);
   strncpy(hash, results, HASH_DIGEST_SIZE);
   hash[HASH_DIGEST_SIZE] = '\0';
}

/*
 * Return true iff the puzzle is solved by the given proof_of_work
 */
bool puzzleIsSovedBy(const Puzzle_t puzzle, const Nonce_t proof_of_work) {
   Hash_t hash;
   puzzleHash(puzzle, proof_of_work, hash);
   // Puzzle is solved by the given proof-of-work if its hash starts with enough leading zeros.
   int i;
   for (i=0; i<puzzle.difficulty; i++) {
      if (hash[i] != '0') {
         return false;
      }
   }
   return true;
}


/*********************
 *  C-Cash MINER
 *   - a default implementation for algorithm that "mines" C-cash blocks
 *********************/
/*
 * "Mine" (solve) the puzzle and return the proof_of_work that "locks" the puzzle's data
 */
Nonce_t puzzleMine(const Puzzle_t puzzle) {
    Nonce_t proof_of_work = {0};
    
   while (!puzzleIsSovedBy(puzzle, proof_of_work)) {
      if (proof_of_work.i_nonce == NONCE_MAX)   // avoid overflow / infinite loop if puzzle is too hard
         return NULL_NONCE;                     // got to return something, although this won't be a valid proof-of-work
      proof_of_work.i_nonce += 1;
   }
   return proof_of_work;
}