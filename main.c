#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "kmp.h"

int
main(int argc, char **argv)
{
	const char *str = "L";

	struct kmp_table table = alloc_table(str);
	fill_table(table);

	int i;
	for (i = 0; i < table.str_size; i++) {
		printf("%c %d\n", table.str[i], table.err[i]);
	}

	const char *str2 = "LOLOLALOLALOLA";
	int pos = 0;
	for (i = 0; i < strlen(str2); i++) {
		struct kmp_result r = advance(str2[i], pos, table);
		pos = r.pos;

		printf("Pos after %c: %d\n", str2[i], pos);
		if (r.match) {
			printf("Matched.\n");
		}
	}

	free_table(table);

	return 0;
}

