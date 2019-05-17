#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) ((a) <= (b) ? (a) : (b))

enum
{
	NEVER,
	COPY,
	ALWAYS,
};

static char delim = '\n';
static char sep = '/';
static char init_sep = NEVER;
//static char trailing_sep = COPY;

struct line
{
	char *buf;
	size_t cap;
	size_t len;
};

static void swaplptr(struct line **a, struct line **b)
{
	struct line *c = *a;
	*a = *b;
	*b = c;
}

static void pdiff(struct line *cur, struct line *prev)
{
	size_t same = 0;
	size_t same_idx = 0;
	size_t i;
	size_t comm_len = MIN(cur->len, prev->len);
	for (i = 0; i < comm_len; i++) {
		if (cur->buf[i] != prev->buf[i]) 
			break;
		if (cur->buf[i] == sep) {
			same++;
			same_idx = i;
		}
	}
	if (i == comm_len && prev->buf && (cur->buf[i] == sep || prev->buf[i] == sep)) {
		same++;
		same_idx = i;
	}
	printf("%lu ", same);
	char *diff = cur->buf + same_idx;
	if (*diff == sep && init_sep == NEVER)
		diff++;
	if (*diff != sep && init_sep == ALWAYS)
		printf("%c", sep);
	printf("%s%c", diff, delim);
}

int main(void)
{
	struct line lines[2];
	struct line *cur = &lines[0];
	struct line *prev = &lines[1];
	ssize_t nread;

	memset(lines, 0, sizeof(lines));
	while ((nread = getdelim(&cur->buf, &cur->cap, delim, stdin)) != -1) {
		assert(nread >= 0);
		cur->len = (size_t)nread;
		if (cur->len && cur->buf[cur->len-1] == delim)
			cur->buf[--cur->len] = '\0';
		pdiff(cur, prev);
		swaplptr(&cur, &prev);
	}
	
	return EXIT_SUCCESS;
}
