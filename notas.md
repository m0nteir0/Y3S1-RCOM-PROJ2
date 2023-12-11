# PROJ2

## Makefile

- `make clean` -> Delete binaries and trasnfered files
- `make download` -> Compiles the program
- `make run` -> Cleans, compiles and runs the program with the default url

## Download.c

### Basic concepts

#### Parse URL
> Parses the url and creates the URL struct with all the info needed

```c
// Struct -> saves important info about the url
typedef struct URL {
  char ip[128];
  char host[256];
  char path[256];
  char filename[128];
  char user[128];
  char password[128];
} URL;


// function
int parse_url(char *url, URL *parsed_url);
``` 
- 





