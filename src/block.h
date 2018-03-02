/**
 *  Block ADT : public interface
 *    Block:  One Node in a BlockChain
 *
 *  COMP220: Assignment 3
 *  Author:  Joseph Fall
 *  Date:    Feb. 1, 2018
 */
 
/*******************
 * PRIVATE TYPE DECLARATIONS
 ********************/
typedef struct Block_s Block_t;
struct  Block_s {
    int id;                         // a unique identifier for this Block
    TransactionList transactions;   // the list of transactions recorded by this Block
    Nonce_t proof_of_work;          // a Nonce that produces a valid hash for this Block
    Hash_t hash;                    // a hash of this Block, stored as a C-string
    int difficulty;                 // the level of difficulty to find the Block's proof_of_work
    Block_t* prev;                  // previous Block in the BlockChain
    Block_t* next;                  // next Block in the BlockChain
};

/*********************
 *  PUBLIC INTERFACE
 *********************/
 
/*
 * Constructor - return pointer to new block to lock given data 
 * POST:  tlistLen(list) == 0
 */
Block_t* blkCreate(TransactionList t, int difficulty, Nonce_t proof_of_work );

/*
 * Destructor - free all dynamic memory associated with the given block
 */
void blkDelete(Block_t* block);

/*
 * Create a serial representation of the block data, stored to buf as a C-string
 * PRE: buf has at least tlistSerialLen(block.transactions)+1 bytes allocated
 */
void blkSerializeTransactions(const Block_t block, char* buf);

/*
 *  Return the puzzle whose solution "locks" the given block using the hash of the previous block
 */
Puzzle_t blkCreatePuzzle(const Block_t block, const Hash_t prev_hash);

/*
 *  Return true iff the given proof_of_work correctly "locks" blkCreatePuzzle(block, prev_hash)
 */
bool blkValidates(const Block_t block, const Hash_t prev_hash, Nonce_t proof_of_work);

/*
 *  Return true iff blkValidates(block, blkPrevHash(block), block.proof_of_work)
 */
bool blkIsValid(const Block_t block);

/*
 * Chain the new_block to the given tail_block iff blkValidates(new_block, tail_block->hash, new_block->proof_of_work)
 */
bool blkChainTo(Block_t* tail_block, Block_t* new_block);

/*
 * block->hash <- SHA1 hash of serialized block data
 */
void blkComputeHash(Block_t* block);

/*
 *  Return block.prev->hash or NULL_HASH if block is first in chain
 */
const char* blkPrevHash(const Block_t block);
