#include "utils.h"

void usage()
{
	printf("To run on a cluster:\n\tmpirun -np <NUM_PROCESSOR> --hostfile <HOSTFILE> wc_mpi.out --path <PATH_FILE_DIR>\n");
    printf("To run on a single machine:\n\tmpirun -np <NUM_PROCESSOR> wc_mpi.out --path <PATH_FILE_DIR>\n");
	exit(0);
}

void parse_arg(int argc, char **argv, char **path)
{
	*path = NULL;
	int opt, opt_index;
  
	static struct option long_options[] = {
		{"path", required_argument, 0, 'p'},
        {"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}};


	while ((opt = getopt_long(argc, argv, "", long_options, &opt_index)) != -1)
	{
		switch (opt)
		{
            case 'p':
                *path = (char*) malloc(strlen(optarg));
                strcpy(*path, optarg);
                break;
            
            case 'h':
                usage();
                break;
                
            default:
                usage();
                break;
		}
	}
	if (*path == NULL)
		usage();
}