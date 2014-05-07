#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "checked.h"
#include "kmp.h"

void
alloc_table(struct kmp_table *table, const char *str)
{
	table->str = str;
	table->str_size = strlen(str);

	assert(table->str_size > 0);
	table->err = checked_malloc(sizeof (int) * table->str_size);
}

void
free_table(struct kmp_table *table)
{
	free(table->err);
}

void
fill_table(struct kmp_table *table)
{
	int pos = 2;
	int ext = 0;

	table->err[0] = -1;

	if (table->str_size == 1) {
		return;
	}

	table->err[1] = 0;
	while (pos < table->str_size) {
		if (table->str[pos - 1] == table->str[ext]) {
			table->err[pos++] = ++ext;
		} else if (ext > 0) {
			ext = table->err[ext];
		} else {
			table->err[pos++] = 0;
		}
	}
}

void
advance(struct kmp_table *table, struct kmp_result *res, char c, int pos)
{
	while (pos >= 0 && table->str[pos] != c) {
		pos = table->err[pos];
	}

	res->pos = ++pos;
	res->match = pos == table->str_size;

	if (res->match) {
		res->pos = table->err[pos - 1] + 1;
	}
}
