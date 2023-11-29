// #include <arpa/inet.h>
// #include <netdb.h>
// #include <netinet/in.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

struct URL
{
  char ip[100];
  char host[100];
  char path[100];
  char filename[100];
  char user[100];
  char password[100];
};

void printURL(const struct URL *url)
{
  printf("IP: %s\n", url->ip);
  printf("Host: %s\n", url->host);
  printf("Path: %s\n", url->path);
  printf("Filename: %s\n", url->filename);
  printf("User: %s\n", url->user);
  printf("Password: %s\n", url->password);
}

// get ip from host
// given by professor
int getIP(char *host, char *ip)
{
  struct hostent *h;

  if ((h = gethostbyname(host)) == NULL)
  {
    herror("gethostbyname()");
    exit(-1);
  }

  strcpy(ip, inet_ntoa(*((struct in_addr *)h->h_addr)));

  return 0;
}

// parse url
// ftp://<user>:<password>@<host>/<url>
int parse(char *url, struct URL *URL)
{
  printf("URL: %s\n", url);

  char *ftp = strtok(url, "/");
  char *credentials = strtok(NULL, "/"); // [<user>:<password>@]<host>
  char *path = strtok(NULL, "");         // url-path

  if (strcmp(ftp, "ftp:") != 0)
  {
    printf("Error: Not using ftp protocol\n");
    return -1;
  }

  char *user = strtok(credentials, ":");
  char *password = strtok(NULL, "@");
  if (password != NULL)
  {
    strcpy(URL->host, credentials);
  }
  else
  {
    user = "anonymous";
    password = "anonymous";
    strcpy(URL->host, strtok(NULL, ""));

    
  }

  strcpy(URL->path, path);
  strcpy(URL->user, user);
  strcpy(URL->password, password);

  printf("\n\n");
  printURL(URL);
  printf("\n\n");

  getIP(URL->host, URL->ip);


  // ftp://anonymous:anonymous@ftp.up.pt/pub/kodi/timestamp.txt
  /*
  // remove "ftp://" from the beginning of the URL
  char *token = strtok(url, "/");
  if (token == NULL || strcmp(token, "ftp:") != 0)
  {
    printf("Invalid URL format\n");
    printf("Header\n");
    exit(-1);
  }
  printf("URL: %s\n", url);
  //URL: ftp: -> why???

  // skip the empty token between "//" and the username
  strtok(NULL, "/");
  // extract user and password
  char *user = strtok(NULL, ":");
  if (user != NULL)
  {
    printf("User: %s\n", user);

    char *pass = strtok(NULL, "@");
    printf("Pass: %s\n", pass);
    strcpy(URL->user, user);
    if (pass != NULL)
    {
      strcpy(URL->password, pass);
    }
  }
  else
  {
    strcpy(URL->user, "anonymous");
    strcpy(URL->password, "anonymous");
  }


  printf("\n\n");
  printURL(URL);
  printf("\n\n");


  // extract host
  char *host = strtok(NULL, "/");
  if (host == NULL)
  {
    printf("Invalid URL format\n");
    printf("Host\n");
    exit(-1);
  }
  strcpy(URL->host, host);

  printf("\n\n");
  printURL(URL);
  printf("\n\n");


  // // extract path and filename
  // token = strtok(NULL, "/");
  // if (token == NULL)
  // {
  //   printf("Invalid URL format\n");
  //   exit(-1);
  // }
  // strcpy(URL->path, token);

  // token = strtok(NULL, "");
  // if (token == NULL)
  // {
  //   printf("Invalid URL format\n");
  //   exit(-1);
  // }
  // strcpy(URL->filename, token);

  // // get IP from host
  // getIP(URL->host, URL->ip);
  */
  return 0;
}

// function to send commands to server
// function to read response from server

// need two sockets. one for receiving the file and other for reading commands from server
int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    printf("Usage: %s ftp://<user>:<password>@<host>/<url>\n", argv[0]);
    exit(-1);
  }

  struct URL URL;

  // parse data from argv[1] to URL struct
  parse(argv[1], &URL);

  // download logic -> similar to lab

  // 1. connect to server -> open socket for sending/receiving commands
  // 2. send user and password
  // 3. send pasv command
  // 4. read addresses and port from response
  // 5. connect to server -> open socket for receiving file
  // 6. send retr command

  return 0;
}