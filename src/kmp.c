#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "checked.h"
#include "kmp.h"

struct kmp_table
alloc_table(const char *str)
{
	struct kmp_table result;

	result.str = str;
	result.str_size = strlen(str);

	assert(result.str_size > 0);
	result.err = checked_malloc(sizeof (int) * result.str_size);

	return (result);
}

void
free_table(struct kmp_table table)
{
	free(table.err);
}

void
fill_table(struct kmp_table table)
{
	int pos = 2;
	int ext = 0;

	table.err[0] = -1;

	if (table.str_size == 1) {
		return;
	}

	table.err[1] = 0;
	while (pos < table.str_size) {
		if (table.str[pos - 1] == table.str[ext]) {
			table.err[pos++] = ++ext;
		} else if (ext > 0) {
			ext = table.err[ext];
		} else {
			table.err[pos++] = 0;
		}
	}
}

struct kmp_result
advance(char c, int pos, struct kmp_table table)
{
	struct kmp_result result;

	while (pos >= 0 && table.str[pos] != c) {
		pos = table.err[pos];
	}

	result.pos = ++pos;
	result.match = pos == table.str_size;

	if (result.match) {
		result.pos = table.err[pos - 1] + 1;
	}

	return (result);
}
