#ifndef KMP_H_
#define	KMP_H_

/*
 * Data structure KMP algorithm.
 *
 * Stores needle as well as error function table.
 */
struct kmp_table
{
	const char *str;
	int str_size;
	int *err;
};

/*
 * Allocates memory for error function table
 * for a given string.
 *
 * The string cannot be empty.
 */
void
alloc_table(struct kmp_table *table, const char *str);

/*
 * Frees any memory allocated by alloc_table.
 *
 * Note that free_table does not free memory
 * used by the string. If the string was dynamically
 * allocated, it should be freed separately.
 */
void
free_table(struct kmp_table *table);

/*
 * Prepare the error table function for a given string.
 *
 * This function expects the table to be already allocated
 * by alloc_table.
 */
void
fill_table(struct kmp_table *table);

/*
 * Data structure for result of KMP algorithm.
 *
 * pos specifies new position in the string.
 * match is non-zero when the needle matches.
 */
struct kmp_result
{
	int pos;
	int match;
};

/*
 * Consumes one character and changes the position
 * inside needle accordingly.
 *
 * table is helper structure described above.
 * res is pointer to the result of this operation.
 * c is character to be consumed.
 * pos is current position inside the needle.
 *
 * The result specifies new position and whether the
 * new character caused the needle to match.
 *
 * Note that this function is thread safe. Multiple
 * threads can perform this operation with a single
 * kmp_table provided that no-one is changing the
 * table from outside.
 */
void
advance(struct kmp_table *table, struct kmp_result *res, char c, int pos);

#endif /* KMP_H_ */
