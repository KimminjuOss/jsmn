#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../productlist.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

 static int jsoneqForSpecificData(const char *json, jsmntok_t *tok, const char *s) {
 	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
 			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
 		return 0;
 	}
 	return -1;
 }

 static int jsoneq(const char *json, jsmntok_t tok, jsmntok_t s) {
 	if (tok.type == JSMN_STRING && s.end - s.start == tok.end - tok.start &&
 			strncmp(json + tok.start, json + s.start , tok.end - tok.start) == 0) {
 		return 0;
 	}
 	return -1;
 }


 void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount,NameTokenInfo *nameTokIndex){
 	int i=0;
 	int count=0;
  int parentIndex=0;
  int object=0;
  jsmntok_t p;

 // 첫번째 토큰의 타입이 array가 아닌 경우
 	while(i<tokcount){
 		if(t[i].type == JSMN_STRING && t[i].size == 1){
      count++;
 			nameTokIndex[count].tokindex=i;
      nameTokIndex[count].objectindex=object;
       p=t[i];
       break;
 		}
     i++;
 	}
   // break문으로 끝나버리면 i가 하나 증가해주지 않기 때문에 i++를 해준다.
   i++;

   // 첫번째 토큰의 타입이 object 인데, 처음 만난 namelist의 value의 type이 array인경우
   // 그 value의 index를 parent로 정하고, 다음 만난 토큰의 parent의 parent가 일치할 경우
  parentIndex=nameTokIndex[1].tokindex+1;
     // ramen은 nameTokIndex에 들어가면 안되기 때문에 다시 count를 0으로 초기화 해준다.
  count=0;

  while(i<tokcount){

    if(t[i].type==JSMN_OBJECT) object++;

       if(t[i].type == JSMN_STRING && t[i].size == 1 && t[t[i].parent].parent ==parentIndex){
         count++;
         nameTokIndex[count].tokindex=i;
         nameTokIndex[count].objectindex=object;
       }
       i++;
  }

 	nameTokIndex[count+1].tokindex=0;
 }
}

int getTokIndex(int objectno, char* name,char *jsonstr, NameTokenInfo *nameTokIndex){
    int i=1;

    while(nameTokIndex[i].tokindex!=0){
      if(nameTokIndex[i].objectindex==objectno){
        if(jsoneqForSpecificData(jsonstr, t[nameTokIndex[i]],name)==0)
        return nameTokIndex[i].tokindex;
      }
      i++;
    }

}

void printListOfData(char *jsonstr,jsmntok_t *t,int *nameTokIndex){
  int widecount=1;
  int i=0;
  int kind=0; // 단순 출력을 위한 번호

  int parentIndex;
  jsmntok_t company;
  jsmntok_t name;
  jsmntok_t price;
  jsmntok_t count;

  printf("************************************************\n");
  printf("번호    제품명   제조사   가격    개수    \n");
  printf("************************************************\n");

  while(1){
    if(nameTokIndex[widecount]==0) break;

    kind++;
    parentIndex=t[nameTokIndex[widecount]].parent;

    i=widecount;

    //같은 object안에 있을 때까지,
    while(1){
      if(t[nameTokIndex[widecount]].parent!=parentIndex){

        printf("%d     %.*s     %.*s     %.*s   %-.*s \n",kind,name.end-name.start,jsonstr+name.start
      ,company.end-company.start,jsonstr+company.start,price.end-price.start,jsonstr+price.start
      ,count.end-count.start,jsonstr+count.start);
        break;
      }

      if (jsoneqForSpecificData(jsonstr, &t[nameTokIndex[i]], "company") == 0){
        company = t[nameTokIndex[i]+1];
      }
      else if (jsoneqForSpecificData(jsonstr, &t[nameTokIndex[i]], "name") == 0){
        name = t[nameTokIndex[i]+1];
      }
      else if (jsoneqForSpecificData(jsonstr, &t[nameTokIndex[i]], "price") == 0){
        price = t[nameTokIndex[i]+1];
      }
      else if (jsoneqForSpecificData(jsonstr, &t[nameTokIndex[i]], "count") == 0){
        count = t[nameTokIndex[i]+1];
      }
      widecount++;
      i++;

    }

  }
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

void printFirstValueList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
 int count=0;
 int i=0;
 jsmntok_t first;
 char firstString[20];
 printf("%s\n","**** Object List ****");

 //name Tokindex가 총 몇갠지 알 수 있다..?응?...근데 ㄱ그건좀아닌듯.

 // 첫번째 object의 토큰을 저장해 놓는다.
 first = t[nameTokIndex[1]];
 while(1){
	 count++;
	 if(nameTokIndex[count]==0) break;
	 if(jsoneq(jsonstr,t[nameTokIndex[count]],first)==0){
		 i++;
	 printf("[NAME %d] %.*s\n",i,t[nameTokIndex[count]+1].end-t[nameTokIndex[count]+1].start,jsonstr + t[nameTokIndex[count]+1].start);
}
}
}

int* printFirstValueListAndSaveIndex(char *jsonstr, jsmntok_t *t, int *nameTokIndex,int *firstObjectIndex){
  int count=1;
  int i=0;
  jsmntok_t first;
  char firstString[20];
  int *temp=(int *)malloc(sizeof(int));
  printf("%s\n","**** Object List ****");

  first = t[nameTokIndex[1]];
  temp[0] = nameTokIndex[1];
  //printf("%d\n",temp[0]);
  printf("[NAME %d] %.*s\n",1,t[nameTokIndex[1]+1].end-t[nameTokIndex[1]+1].start,jsonstr + t[nameTokIndex[1]+1].start);

  while(1){
 	 count++;
 	 if(nameTokIndex[count]==0) break;
 	 if(jsoneq(jsonstr,t[nameTokIndex[count]],first)==0){
 		 i++;
     temp = (int *)realloc(temp,sizeof(int)*(i+1));
     temp[i]=nameTokIndex[count];
     //printf("%d\n",temp[i]);
 	 printf("[NAME %d] %.*s\n",i+1,t[nameTokIndex[count]+1].end-t[nameTokIndex[count]+1].start,jsonstr + t[nameTokIndex[count]+1].start);
 }
 }
 //마지막 끝난 시점의 번호를 이때까지 받아온 count로 준다.
 temp = (int *)realloc(temp,sizeof(int)*(i+2));
 temp[i+1]=0;

 firstObjectIndex=temp;

 return firstObjectIndex;
}

void printAllInfoOfObject(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
  int kind;
  int count=0;
  int start;
  int end;
  int start_nameIndex;
  int end_nameIndex;
  int i;
  int j;
  int test=0;
  int *firstObjectIndex=NULL;

  firstObjectIndex=printFirstValueListAndSaveIndex(jsonstr,t,nameTokIndex,firstObjectIndex);

  while(1){
    count++;
    if(nameTokIndex[count]==0) break;
  }


  while(1){
    printf("원하는 번호 입력 (Exit :0) : ");
    scanf("%d",&kind);

    if(kind==0) break;

    start=firstObjectIndex[kind-1];
    for(j=1 ; j<=count ; j++){
      if(nameTokIndex[j]==start) start_nameIndex=j;
    }

    end=firstObjectIndex[kind];
    for(j=1 ; j<=count ; j++){
      if(nameTokIndex[j]==end) end_nameIndex=j;
    }

// 맨 마지막 객체를 선택한 경우
    if(end==0){
      for(i=start_nameIndex ; i<count ; i++){

        if(i==start_nameIndex){
          printf("%.*s  :  ",t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start);
          printf("%.*s\n",t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start,jsonstr + t[nameTokIndex[i]+1].start);
        }

        else{
        printf("  [%.*s]",t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start);
        printf("  %.*s\n",t[nameTokIndex[i]+1].end-t[nameTokIndex[i]+1].start,jsonstr + t[nameTokIndex[i]+1].start);
        }
      }
    }

    else{

    for(i=start_nameIndex ; i<end_nameIndex ; i++){

      if(i==start_nameIndex){
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
	f=fopen("data4.json","r");
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
	NameTokenInfo nameTokIndex[100];
  int *firstObjectIndex=NULL;

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
  //printFirstValueList(str,t,nameTokIndex);
  //countObject=printFirstValueList(str,t,nameTokIndex);
	//printAllInfoOfObject(str, t, nameTokIndex);
  //printFirstValueListAndSaveIndex(str,t,nameTokIndex,firstObjectIndex);
  //printListOfData(str,t,nameTokIndex);

	return 0;

	/* Loop over all keys of the root object */

	return EXIT_SUCCESS;
}
