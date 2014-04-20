#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "consumer.h"
#include "kmp.h"
#include "queue.h"

int
main(int argc, char **argv)
{
	char str[] = "void *arg_";

	struct kmp_table table = alloc_table(str);
	fill_table(table);

	/*
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
	*/

	struct queue q = alloc_queue(20);

	char f1[] = "main.c";
	char f2[] = "consumer.c";
	char f3[] = "kmp.h";

	enqueue(f1, &q);
	enqueue(f2, &q);
	enqueue(f3, &q);
	enqueue(NULL, &q);

	struct consume_arg arg = {&q, &table};
	consume(&arg);

	free_queue(&q);
	free_table(table);

	return 0;
}

