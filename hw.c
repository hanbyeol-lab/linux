#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#pragma warning(disable:4996)

int filecpy(char *arg);

int main(int argc, char **argv)
{

	int fd, in, out, n;
	struct stat buf1, buf2;
	char *ptr1, *ptr2;
	char buf[1024];
	DIR *dp, *dp1;
	struct dirent *entry;

	if(argc<3)
	{
		write(2, "Usage: copy file1 file2\n", 25);
		return -1;
	}

	if(lstat(argv[1],&buf1)<0 | lstat(argv[2], &buf2)<0)
	{
		printf("lstat error\n");
	}

	if(S_ISREG(buf1.st_mode))
		ptr1="regular";

	else if(S_ISDIR(buf1.st_mode))
		ptr1="directory";

	if(S_ISREG(buf2.st_mode))
		ptr2="regular";

	else if(S_ISDIR(buf2.st_mode))
		ptr2="directory";

	if(ptr1=="regular" && ptr2=="regular")
	{

		if((in=open(argv[1], O_RDONLY))<0)
		{
			perror(argv[1]);
			return -1;
		}

		if((out=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR))<0)
		{
			perror(argv[2]);
			return -1;
		}

		while((n=read(in, buf, sizeof(buf)))>0)
			write(out, buf, in);

		printf("regular, regular\n");

		close(out);
		close(in);
	}

	else if(ptr1=="regular" && ptr2=="directory")
	{
	        if((in=open(argv[1], O_RDONLY))<0)
                {
                        perror(argv[1]);
                        return -1;
                }

                if((dp=opendir(argv[2]))==NULL)
		{
			printf("dir_error\n");
			return -1;
		}
		if(chdir(argv[2])==-1)
		{
			printf("failed\n");
		}

		if(dp!=NULL)
                {
			while((n=read(in, buf, sizeof(buf)))>0)
			{
				if((out=open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR))<0);
				{
					printf("no src\n");
				}
				write(out, buf, in);
			}
		}
		printf("regular, directory\n");
		close(out);
		closedir(dp);
		close(in);
		chdir("..");
	}

        else if(ptr1=="directory" && ptr2=="directory")
        {
		filecpy(argv[1]);
        }

	else
	{
		printf("no_error\n");
	}

	return 0;

}

int filecpy(char *arg)
{
	struct dirent *dirt;
	struct stat statBuf;
	DIR *pdir, *dp;
	int i=0, count=0, j;
	int in, out, n, isdir=1;
	char *dirName[255], *dir[255];
	char buf[1024];

	memset(dirName, 0, sizeof(dirName));
	memset(&dirt, 0, sizeof(dirt));
	memset(&statBuf, 0, sizeof(statBuf));

	chdir(arg);

	if((pdir=opendir("."))<=0)
	{
		perror("opendir error\n");
		return -1;
	}

	while(dirt=readdir(pdir))
	{

		if(dirt->d_ino!=0)
		{
			if(strcmp(dirt->d_name, ".") && strcmp(dirt->d_name, ".."))
			{
				printf("strcmp\n");
				dirName[count]=dirt->d_name;
				count=count+1;
			}
			else;

			stat(dirt->d_name, &statBuf);

			if(S_ISDIR(statBuf.st_mode))
			{
				isdir=1;
				printf("I am dir: %s\n", dirName[count-1]);
				for(i=0;i<count;i++)
				{
					closedir(pdir);
					if((filecpy(dirName[i]))==-1) break;
				}
				printf("now is directory\n");
			}
			else if(S_ISREG(statBuf.st_mode))
			{
				if((in=open(dirt->d_name, O_RDONLY))<0)
				{
					printf("in failed\n");
				}
				if((dp=opendir("/home/hanbyeol/hw/dst"))==NULL)
				{
					printf("dp_error\n");
					return -1;
				}

				chdir("/home/hanbyeol/hw/dst");

				if(dp!=NULL)
				{
					while((n=read(in, buf, sizeof(buf)))>0)
					{
						printf("while\n");
						if((out=open(dirt->d_name, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR))<0);
						{
							printf("out failed\n");
						}
						write(out, buf, in);
					}
				}
				printf("directory, directory\n");
				close(out);
				closedir(dp);
				close(in);
				chdir("/home/hanbyeol/hw/src");
			}
		}
	}
	closedir(pdir);
	chdir("..");
	return 0;
}
