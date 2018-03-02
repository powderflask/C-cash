/**
 *  Transaction LIST ADT : private implementation / algorithms
 *
 *  COMP220: Assignment 3
 *  Author:  
 *  Date:    Feb. 1, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <assert.h>

#include "trnslist.h"

// max characters in an int converted to string rep. (not really appropriate, but can't find in std. lib.)
#define INT_LEN 10

/***********************
 * TRANSACTION RECORD private implementation details
 *   --> storage for a single C-cash transaction
 ***********************/
// Definitions used for serializing TransactionRecords
const char* TRNS_SERIAL_FORMAT = "<%s:%lf:%s>";  // format for serialized records
const int TRNS_SERIAL_DELIMS = 4;              // number of delimiter characters used to serialize a record

TransactionRecord trnsCreate(char* debtor, double amount, char* creditor) 
{
   assert( amount > 0 );
   char* d = calloc(strlen(debtor)+1, sizeof(char));
   strcpy(d, debtor);
   char* c = calloc(strlen(creditor)+1, sizeof(char));
   strcpy(c, creditor);
   time_t t = time(NULL);
   TransactionRecord r = {d, amount, c, t};
   return r;
}

void trnsDelete(TransactionRecord* r) {
   free(r->debtor);
   r->debtor = NULL;
   free(r->creditor);
   r->creditor = NULL;
}

void trnsPrint(const TransactionRecord r) {
   const char* fmt = "%x (%X)";
   char formatted_datetime[80];
   strftime(formatted_datetime, 80, fmt, localtime( &r.timestamp ));
   printf("   <%s --> %.4f --> %s> %s \n", r.debtor, r.amount, r.creditor, formatted_datetime);
   
   // CODE FOR READING A FORMATTED DATE/TIME back into a timestamp
   struct tm timestruct;
   strptime (formatted_datetime, fmt, &timestruct);
   assert(mktime(&timestruct) == r.timestamp);
}

int trnsSerialLen(const TransactionRecord r) {
   return strlen(r.debtor) + strlen(r.creditor) + DBL_DIG + TRNS_SERIAL_DELIMS;  // max. char length of serialized record
}

// Generate a serialzied text representation of this record, store in buffer as a C-string
// Assumes that s is at least trnsSerialLen(r)+1 in length
void trnsSerialize(const TransactionRecord r, char* buffer) {
   sprintf(buffer, TRNS_SERIAL_FORMAT, r.debtor, r.amount, r.creditor);
}


/***********************
 * TRANSACTION LIST private implementation details
 *   --> storage for a series of C-cash transactions
 ***********************/
// Size of block that list capacity will grow by as list expands.
const int TLIST_DEFAULT_BLOCK_SIZE = 10;

const char* TLIST_SERIAL_REC_FORMAT = "%s,";  // format for each serialized record in the serialized list
const int TLIST_SERIAL_REC_DELIMS = 1;        // number of delimiter characters used for each serialize a record
const int TLIST_SERIAL_DELIMS = INT_LEN+3;    // number of additional delimiters added to serialize list

/*
 * Public Constructor - return a new, empty list 
 * POST:  tlistLen(list) == 0
 */
TransactionList tlistCreate( ) {
   TransactionList l = {0, NULL, 0};
   return l;   
}

// Helper:  Free all memory from the transaction data array
void tlistFreeData(TransactionList *list ) {
   int i;
   for (i=0; i<list->len; i++) {
      trnsDelete(&(list->data[i]));
   }
   free(list->data);
   list->data = NULL;
}
/*
 * Destructor - remove all data and free all memory associated with the list 
 * POST:  list.len == 0 && list.capacity == 0
 */
void tlistDelete( TransactionList *list ) {
   tlistFreeData( list );
   list->capacity = 0;
   list->len = 0;
}

/*
 * PRIVATE: increase list capacity by the given size
 * POST:  list.len == list.len + size
 */
void tlistGrowBy( TransactionList *list, int size ) {
   int new_capacity = list->capacity + size;
   // This is approximately what realloc does, altough realloc is more efficient and should be prefered.
   //   list->data = realloc(list->data, new_capacity*sizeof(TransactionRecord));
   TransactionRecord *new_data = calloc(new_capacity, sizeof(TransactionRecord));
   int i;
   for (i=0; i<list->len; i++) {
      new_data[i] = list->data[i];
   }
   free(list->data);  // Don't delete transaction records here - we just copied them to the new data array!
   list->data = new_data;
   
   list->capacity = new_capacity;
}

/*
 * PRIVATE: increase list capacity by the TLIST_DEFAULT_BLOCK_SIZE
 */
void tlistGrow( TransactionList *list ) {
   tlistGrowBy(list, TLIST_DEFAULT_BLOCK_SIZE);
}

/*
 * Print a text representation of this list on STDOUT
 */
void tlistPrint( const TransactionList list  ) {
   int i;
   printf("--- Transactions ---\n");
   for (i=0; i<list.len; i++) {
      trnsPrint(list.data[i]);
   }
   printf("--------------------\n");
}

/*
 * Compute and return buffer size required to hold serialized list
 */
int tlistSerialLen(const TransactionList list) {
   int len = 0;
   int i;
   for (i=0; i<list.len; i++) {
      len += trnsSerialLen(list.data[i]) + TLIST_SERIAL_REC_DELIMS;
   }
   return len + TLIST_SERIAL_DELIMS;
}

/*
 * Generate a serialzied text representation of this list, store in buffer as a C-string
 * Assumes that s is at least tlistSerialLen(list)+1 in length
 */
void tlistSerialize(int id, const TransactionList list, char* buffer) {
   sprintf(buffer, "%d:[", id);  // opening list delim.
   int i;
   for (i=0; i<list.len; i++) {
      int end = strlen(buffer);     // index of end of buffer, where next serialized record should be stored.
      trnsSerialize(list.data[i], &buffer[end]);
      strcat(buffer, ",");   // record delimiter
   fflush(stdout);
   }
   // Replace last record delim. with closing list delim.
   buffer[strlen(buffer)-1] = ']';
}

/*
 * Return the number of transaction records in the list
 */
int tlistLen( const TransactionList list ) {
   return list.len;
}

/*
 * Append the given transaction to the list 
 * POST: list->data[list->len-1] == item
 */
void tlistAppend( TransactionList *list, char* debtor, double amount, char* creditor ) {
   if (list->len == list->capacity) {
      tlistGrow(list);
   }
   list->data[list->len] = trnsCreate(debtor, amount, creditor);
   list->len++;
}
