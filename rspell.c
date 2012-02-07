#define MAX 120503
#define DICTIONARY_PATH "/Users/robb/src/rspell/en_US.ascii"

#define FOUND 0

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct List {
     char *name;
     struct List *next;
};

struct List *symbol_table[MAX];

unsigned int hash(char *str)
{
     unsigned int h;
     unsigned char *p;

     h = 0;
     for (p = (unsigned char *) str; *p != '\0'; p++)
          h = 31 * h + *p;

     return h % MAX;
}

char lookup(char *str) 
{
     unsigned int h;
     struct List *symbol;

     char out = 1;

     h = hash(str);
     for (symbol = symbol_table[h]; symbol != NULL; symbol = symbol->next)
          if (strcmp(str, symbol->name) == 0)
               return FOUND;
     
     /* special case: capitalized word */
     if (isupper(str[0])) {
          str[0] = tolower(str[0]);
          h = hash(str);
          for (symbol = symbol_table[h]; symbol != NULL; symbol = symbol->next)
               if (strcmp(str, symbol->name) == 0) 
                    out = FOUND;
          str[0] = toupper(str[0]);
     }

     return out;
}

void create(char *str) 
{
     unsigned int h;
     struct List *symbol;
     
     h = hash(str);
     symbol = malloc(sizeof(struct List));     
     symbol->name = strdup(str);
     symbol->next = symbol_table[h];
     symbol_table[h] = symbol;
}

void process_file(const char *path)
{
     FILE *file = fopen(path, "r");
     size_t sz = 0;
     char *line = NULL;
     char *p;

     if (file != NULL) {
          while (getline(&line, &sz, file) != -1) {
               p = strchr(line, '\n');
               *p = '\0';
               create(line);
          }
          fclose(file);
     }
}

/* strip punctuation, beginning of sentence stuff, etc */
char *clean(const char *word)
{
     char *cleaned = strdup(word);
     char *last = &cleaned[strlen(cleaned) - 1];

     while (ispunct(cleaned[0]))
          cleaned++;

     while (ispunct(*last)) {
          *last = '\0';
          last--;
     }

     while (isdigit(cleaned[0]))
          cleaned++;

     return cleaned;
}

int main(int argc, char *argv[])
{
     char *line = NULL;
     size_t sz = 0;
     char *p, *word;

     process_file(DICTIONARY_PATH);
     while (getline(&line, &sz, stdin) != -1) {
          p = strchr(line, '\n');
          *p = '\0';

          while (line != NULL) {
               word = strsep(&line, " -*");
               if (word[0] != '\0') {
                    word = clean(word);
                    if (word[0] != '\0' && lookup(word) != FOUND)
                         printf(">%s\n", word);
               }
          }
     }
}
