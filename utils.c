#include "utils.h"

void usage()
{
	printf("ON cluster:\n\tmpirun -np <NUM_PROCESSOR> --hostfile <HOSTFILE> wc_mpi.out --path <PATH_FILE_DIR>\n");
	printf("ON single machine:\n\tmpirun -n <NUM_PROCESSOR> wc_mpi.out --path <PATH_FILE_DIR>\n");
	exit(0);
}

void parse_arg(int argc, char **argv, char **path)
{
	*path = NULL;
	int opt, opt_index;

	static struct option long_options[] = {
		{"path", required_argument, 0, 'p'},
		{0, 0, 0, 0}};

	while ((opt = getopt_long(argc, argv, "p:", long_options, &opt_index)) != -1)
	{
		switch (opt)
		{
		case 'p':
			*path = (char *)malloc(strlen(optarg));
			strcpy(*path, optarg);
			break;

		default:
			usage();
			break;
		}
	}
	if (*path == NULL)
		usage();
}

int countFilesInDirectory(char *path)
{
    DIR *dir;
    struct dirent *d;

    int count = 0;

    if ((dir = opendir(path)) != NULL)
    {
        while ((d = readdir(dir)) != NULL)
        {
            if (strcmp(d->d_name, "..") != 0 && strcmp(d->d_name, ".") != 0)
            {
                count++;
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Could not open directory!");
        exit(0);
    }
    return count;
}