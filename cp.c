#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

static int indent=0;
char cdir[256];

void filecpy(char *arg, void (*func)(char *), int depth);
void func(char *file);

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
				if((out=open("src", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR))<0);
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
		filecpy(argv[1], func, 0);
        }

	else
	{
		printf("no_error\n");
	}

	return 0;

}

void func(char *file)
{
	printf("%s/%s \n", getcwd(cdir, 256), file);
}

void filecpy(char *arg, void (*func)(char *), int depth)
{
	struct dirent **dirt;
	struct stat statBuf;
	DIR *pdir, *dp;
	int i=0, count=0, j;
	int in, out, n, nitems;
	char *dirName[255], *dir[255];
	char buf[1024];

	if((pdir=opendir(arg))<=0)
	{
		perror("opendir error\n");
		return -1;
	}

	chdir(arg);

	nitems=scandir(".", &dirt, NULL, alphasort);

	printf("%d\n", nitems);

	for(j=0;j<nitems;j++);
	{

	while((dirt[j]=readdir(pdir))!=NULL)
	{
		lstat(dirt[j]->d_name, &statBuf);
		if(S_ISDIR(statBuf.st_mode));

		if(dirt[j]->d_ino!=0)
		{
			if(strcmp(dirt[j]->d_name, ".") && strcmp(dirt[j]->d_name, ".."))
			{
				dirName[count]=dirt[j]->d_name;
				printf("%s\n", dirName[count]);
				count=count+1;
				printf("ok\n");
			}

			func(dirt[j]->d_name);

			stat(dirt[j]->d_name, &statBuf);
			if(S_ISDIR(statBuf.st_mode))
			{
				func(dirt[j]->d_name);
				if(indent<(depth-1) || (depth==0))
				{
					indent++;
					filecpy(dirt[j]->d_name, func, depth);
				}
				printf("why\n");
			}
			else
			{
				if((in=open(dirName[count-1], O_RDONLY))<0)
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
						if((out=open("src", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR))<0);
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
				closedir(pdir);
			}
		}
	}
	}
	indent --;
	chdir("..");
}
