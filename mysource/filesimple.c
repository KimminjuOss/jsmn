#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

char *readJSONFile(){
	FILE *f;
	f=fopen("data.json","r");
	//char strTemp[255];
	char *str;
	char oneline[255];
	int n=0;

	str = (char *)malloc(sizeof(oneline));
	while(1){ // str로 우선 한 줄을 읽어 드린다. 다음줄을 읽어드릴때...
		               // 어떻게?
		fgets(oneline,sizeof(oneline),f);
		if(feof(f)) break;

		n+=strlen(oneline);
		//realloc(str) : 크기 = count+1
		str = (char *)realloc(str,n+1);
		//strcat
    strcat(str,oneline);

	}
	fclose(f);
	return str;
}

static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	static const char *str;
	str=readJSONFile();
	//printf("%s",str);
	//return 0;

	jsmn_init(&p);
	r = jsmn_parse(&p, str, strlen(str), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(str, &t[i], "name") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- name: %.*s\n", t[i+1].end-t[i+1].start,
					str + t[i+1].start);
			i++;
		} else if (jsoneq(str, &t[i], "keywords") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- keywords: %.*s\n", t[i+1].end-t[i+1].start,
					str + t[i+1].start);
			i++;
		} else if (jsoneq(str, &t[i], "description") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					str + t[i+1].start);
			i++;
		} else if (jsoneq(str, &t[i], "examples") == 0) {
			int j;
			printf("- examples:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, str + g->start);
			}
			i += t[i+1].size + 1;
		}
		
	}
	return EXIT_SUCCESS;
}
