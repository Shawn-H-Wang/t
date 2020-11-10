#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

#include <unistd.h>

#define PATH_SIZE 4094

void my_rmdir(const char * path);

/* rm -rf path: ./a.out path */
int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		fprintf(stdout, "argument error!\n");
		return 1;
	}

	my_rmdir(argv[1]);

	return 0;
}

void my_rmdir(const char * path)
{
	DIR *dirp;
	dirp = opendir(path);
	if (NULL == dirp)
	{
		perror(path);
		return;
	}

	struct dirent *entry;
	int ret;
	while (1)
	{
		entry = readdir(dirp);
		if (NULL == entry)
		{
			break;
		}
		// skip . & ..
		if (0 == strcmp(".", entry->d_name) || 0 == strcmp("..", entry->d_name))
		{
			continue;
		}

		char buf[PATH_SIZE];
		snprintf(buf, PATH_SIZE, "%s/%s", path, entry->d_name);
		ret = remove(buf);
		if (-1 == ret)
		{
			if (ENOTEMPTY == errno)
			{
				my_rmdir(buf);
				continue;
			}
			perror(buf);
			return;
		}
		fprintf(stdout, "rm file: %s\n", buf);
	}

	closedir(dirp);

	ret = rmdir(path);
	if (-1 == ret)
	{
		perror(path);
		return;
	}
	fprintf(stdout, "rm dir: %s\n", path);
}

