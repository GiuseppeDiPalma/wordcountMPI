#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>

void usage();
void parse_arg(int argc, char **argv, char **path);
int countFilesInDirectory(char *path);