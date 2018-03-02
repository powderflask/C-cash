/**
 *  Cryptographic Puzzle ADT : public interface
 *      A Cryptographic Puzzle who's solution (proof-of-work) serves as a "lock" for block of data
 * 
 *  COMP220: Assignment 3
 *  Author:  Joseph Fall
 *  Date:    Feb. 1, 2018
 */
 
 
/*********************
 *  PUBLIC TYPES AND DECLARATIONS
 *********************/
// Data type for the Puzzle's "proof-of-work" - 8 bytes of data.
#ifndef NONCE_MAX
   #define NONCE_MAX ULONG_MAX      // the largest possible proof_of_work value.  Smallest Nonce is zero.
   #define NONCE_BYTES sizeof(long int)
#endif

union Nonce_u {
    unsigned long int i_nonce;
    char c_nonce[NONCE_BYTES];
};
typedef union Nonce_u Nonce_t;

extern const Nonce_t NULL_NONCE;             // a Null Nonce value


// Data type for data hash values
#ifndef HASH_DIGEST_SIZE                     // Size of Hash depends on the hashing algorithm used
   #define HASH_DIGEST_SIZE 20               // SHA1 produces a 20 byte digest
#endif

typedef char Hash_t[HASH_DIGEST_SIZE + 1];   // a Hash is array of byte data - store as a C-string for convenience.

extern const Hash_t NULL_HASH;               // a Null Hash value

 
/*******************
 * PRIVATE TYPE DECLARATIONS
 ********************/
typedef struct  {
    char* data_buffer;        // the data_buffer to be "locked" by this Puzzle
    Hash_t prev_hash;         // additional data to be "locked" with the data_buffer
    unsigned int difficulty;  // the level of difficulty required to solve this puzzle (>= 1)
} Puzzle_t;


/*********************
 *  PUBLIC INTERFACE
 *********************/
// Helper - for testing
void printHash( Hash_t hash );

/*
 * Constructor - return a new Puzzle whose solution "locks" the given 2 data items
 * @param buf - the data buffer to be locked  (Puzzle makes a copy of the caller's data)
 * @param prev_hash - additional hash to be locked with the data_buffer (Puzzle makes a copy of the caller's data)
 * @param difficulty - level of difficulty set for this Puzzle - increaing difficulty makes Puzzle harder to "mine"
 * PRE: 0 < difficulty < HASH_DIGEST_SIZE/2
 */
const Puzzle_t puzzleCreate(const char* buf,  const Hash_t prev_hash, int difficulty);

/*
 * Destructor - remove all data and free all memory associated with the puzzle 
 */
void puzzleDelete( Puzzle_t *puzzle ); 

/*
 * Print a text representation of the puzzle on STDOUT
 */
void puzzlePrint( const Puzzle_t puzzle  ); 

/*
 * Hash the puzzle data along with the given proof-of-work
 * @param puzzle : the puzzle data to be hashed
 * @param proof-of-work:  a Nonce to be hashed with the puzzle data
 * @param hash: OUTPUT - the SHA1 hash result
 */
void puzzleHash(const Puzzle_t puzzle, const Nonce_t proof_of_work, Hash_t hash); 

/*
 * Return true iff the puzzle is solved by the given proof_of_work
 */
bool puzzleIsSovedBy(const Puzzle_t puzzle, const Nonce_t proof_of_work);


/*********************
 *  C-Cash MINER
 *   - a default implementation for algorithm that "mines" C-cash blocks
 *********************/
/*
 * "Mine" (solve) the puzzle and return the proof_of_work that "locks" the puzzle's data
 */
Nonce_t puzzleMine(const Puzzle_t puzzle);