/**
 *  Transaction LIST ADT : public interface
 *      A List of Transaction Records (an exchange of C-cash coins between 2 parties)
 * 
 *  COMP220: Assignment 3
 *  Author:  Joseph Fall
 *  Date:    Feb. 1, 2018
 */
 
/*******************
 * PRIVATE TYPE DECLARATIONS
 ********************/
typedef struct  {
    char* debtor;    // username of the debtor (peron paying) in this transaction
    double amount;   // the transaction amout payed from the debtor to the creditor
    char* creditor;  // username of the creditor (peron bieng payed) in this transaction
    time_t timestamp; // timestamp for this transaction
} TransactionRecord;

typedef struct  {
    int capacity;    // current list capacity -- current size of the data array -- current max. len of list
    TransactionRecord* data;       // the actual list data, a dyanmic, resizable array of TransactionRecords
    int len;         // the current length of the list -- the number of elements in the list
} TransactionList;


/*********************
 *  PUBLIC INTERFACE
 *********************/
/*
 * Constructor - return a new, empty list 
 * POST:  tlistLen(list) == 0
 */
TransactionList tlistCreate( );

/*
 * Destructor - remove all data and free all memory associated with the list 
 * POST: tlistLen(list) == 0
 */
 void tlistDelete( TransactionList *list ); 

/*
 * Print a text representation of this list on STDOUT
 */
void tlistPrint( const TransactionList list  ); 

/*
 * Compute and return buffer size required to hold serialized list
 */
int tlistSerialLen(const TransactionList list);

/*
 * Generate a serialzied text representation of this list, store in buffer as a C-string
 * Assumes that s is at least tlistSerialLen(list)+1 in length
 */
void tlistSerialize(int id, const TransactionList list, char* buffer);

/*
 * Return the number of transaction records in the list
 */
int tlistLen( const TransactionList list );

/*
 * Append the given item to the list 
 * A deep copy of char* parameters is made - caller's memory is not used or managed by the list!
 * POST: a new TransactionRecord containing copies of the given data is appended to this list
 */
void tlistAppend( TransactionList *list, char* debtor, double amount, char* creditor );
