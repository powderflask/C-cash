/**
 *  BlockChain ADT : public interface
 *      A doubly-linked chain of Blocks, using cryptographic hash to guarantee immutibility
 *
 *  COMP220: Assignment 3
 *  Author:  Joseph Fall
 *  Date:    Feb. 1, 2018
 */
 
/*******************
 * PRIVATE TYPE DECLARATIONS
 ********************/
typedef struct {
    Block_t* head;
    Block_t* tail;
} BlockChain;

/*********************
 *  PUBLIC INTERFACE
 *********************/
 
/*
 * Constructor - return a new, empty BlockChain 
 * POST:  bcLen(chain) == 0
 */
BlockChain bcNew( );

/*
 * Destructor - remove all data and free all memory associated with the chain 
 * POST: bcLen(chain) == 0
 */
void bcDelete( BlockChain *chain ); 

/*
 * Print a text representation of this chain on STDOUT
 */
void bcPrint( const BlockChain chain ); 

/*
 * Return the number of Blocks in the chain
 */
int bcLen( const BlockChain chain );

/*
 *  Return true iff blkIsValid(block) for every block in the chain
 */
bool blkIsValid(const BlockChain chain);


/*
 * Return a pointer to the chain's tail Block
 */
Block_t* bcTail(const BlockChain chain);

/*
 * Append the given block to the chain 
 *    iff blkValidates(new_block, bcTail->hash, new_block->proof_of_work)
 * POST: bcTail(*chain) == new_block && blkIsValid(*new_block)
 */
void bcAppend( BlockChain *chain, Block_t* new_block );
