#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

 static int jsoneq(const char *json, jsmntok_t tok, jsmntok_t s) {
 	if (tok.type == JSMN_STRING && s.end - s.start == tok.end - tok.start &&
 			strncmp(json + tok.start, json + s.start , tok.end - tok.start) == 0) {
 		return 0;
 	}
 	return -1;
 }

void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount,int *nameTokIndex){

	int i=0;
	int count=0;

	while(i<tokcount){
		if(t[i].type == JSMN_STRING && t[i].size == 1){
			count++;
			nameTokIndex[count]=i;
		}
		i++;
	}
	nameTokIndex[count+1]=0;
}

void printNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
	int count=0;
	printf("%s\n","**** Name List ****");

	while(1){
		count++;
		if(nameTokIndex[count]==0) break;
		printf("[NAME %d] %.*s\n",count,t[nameTokIndex[count]].end-t[nameTokIndex[count]].start,jsonstr + t[nameTokIndex[count]].start);

	}
}

int printFirstValueList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
 int count=0;
 int i=0;
 jsmntok_t first;
 char firstString[20];
 printf("%s\n","**** Object List ****");

 //name Tokindex가 총 몇갠지 알 수 있다..?응?...근데 ㄱ그건좀아닌듯.

 first = t[nameTokIndex[1]];
 while(1){
	 count++;
	 if(nameTokIndex[count]==0) break;
	 if(jsoneq(jsonstr,t[nameTokIndex[count]],first)==0){
		 i++;
	 printf("[NAME %d] %.*s\n",i,t[nameTokIndex[count]+1].end-t[nameTokIndex[count]+1].start,jsonstr + t[nameTokIndex[count]+1].start);
}
 }
  return i;
}

void printAllInfoOfObject(char *jsonstr, jsmntok_t *t, int *nameTokIndex){

	int countObject;
	int kind;
	int numbefOfObjectMember;
	int start;
	int end;
  int i=0;
  int count=0;

  while(1){
    if(nameTokIndex[count]==0) break;
    count++;
  }

	countObject = printFirstValueList(jsonstr,t,nameTokIndex);
	printf("1\n");
	numbefOfObjectMember = count/countObject;

	while(1){
		printf("원하는 번호 입력 (Exit :0) : ");
		scanf("%d",&kind);
		if(kind==0) break;

		start=numbefOfObjectMember*(kind-1)+1;
		end=start+numbefOfObjectMember;

		for(i=start ; i<end ; i++){

			if(i==start){
				printf("%.*s  :  ",t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start);
				printf("%.*s\n",t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start,jsonstr + t[nameTokIndex[i]+1].start);
			}

      else{
			printf("		[%.*s]",t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start);
			printf("  %.*s\n",t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start,jsonstr + t[nameTokIndex[i]+1].start);
      }
		}
	}
}




void selectNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){

	int index;

	while(1){
		printf("Select Name's no (exit:0) >> ");
		scanf("%d",&index);
		if(index==0) return;
   	printf("[Name %d] %.*s\n",index,t[nameTokIndex[index]].end-t[nameTokIndex[index]].start,jsonstr + t[nameTokIndex[index]].start);
	  printf("%.*s\n",t[nameTokIndex[index]+1].end-t[nameTokIndex[index]+1].start,jsonstr + t[nameTokIndex[index]+1].start);
	}
}

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

int main() {
	int i;
	int r;
	int countObject;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	static char *str;
	int nameTokIndex[100];
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
	if (r < 1) {
		printf("Object expected\n");
		return 1;
	}


  jsonNameList(str,t,r,nameTokIndex);
	//selectNameList(str, t,nameTokIndex);
	//printNameList(str,t,nameTokIndex);
	//countObject=printFirstValueList(str,t,nameTokIndex);
	printAllInfoOfObject(str, t, nameTokIndex);
	return 0;

	/* Loop over all keys of the root object */

	return EXIT_SUCCESS;
}
