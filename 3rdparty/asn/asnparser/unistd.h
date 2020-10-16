#ifndef GETOPT_H
#define GETOPT_H

#ifdef __cplusplus
extern "C" {
#endif

int getopt(int argc, char** argv, const char* optstring); 
extern char* optarg;
extern int optind;
#ifdef __cplusplus
}
#endif
#endif

