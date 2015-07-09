#include <stdio.h>
#include <string.h>
#include <pmc.h>

int main()
{
	FILE *myList;
	FILE *dat;

	char line[1024], temp[1024], check[1024], datas[80];
	char *p;
	int FileSize = 0;
	unsigned long long TotalBytes = 0;
	unsigned long long FileNum = 0;
	unsigned long long OldBytes = 0;
	unsigned long long OldFiles = 0;
	unsigned long long OldTapes = 0;

	/* lets read the old data numbers from *.listing catalogs */
	dat = fopen("new-stuff.cfg", "rt");
	if (!dat)
	{
		printf("error at new-stuff.cfg, exiting\n");
		exit(1);
	}

	bzero(datas, sizeof(datas));
	fgets(datas, 80, dat);
	sscanf(datas, "%llu", &OldBytes);

	bzero(datas, sizeof(datas));
	fgets(datas, 80, dat);
	sscanf(datas, "%llu", &OldFiles);

	bzero(datas, sizeof(datas));
	fgets(datas, 80, dat);
	sscanf(datas, "%llu", &OldTapes);
	fclose(dat);

	/* open the new list ls -kR */
	myList = fopen("new-stuff.txt", "rt");
	if (!myList)
	{
		printf("error at new-stuff.txt, exiting.\n");
		exit(1);
	}

	fgets(line, 1024, myList);

	while (!feof(myList))
	{
		p = strtok(line, " ");

		if (p)
		{
			strcpy(check, p);

			if (strcmp(check, "-rw-r--r--") == 0)
			{
				p = strtok(NULL, " ");
				p = strtok(NULL, " ");
				p = strtok(NULL, " ");
				p = strtok(NULL, " ");
				bzero(temp, sizeof(temp));
				strcpy(temp, p);
				FileSize = atoi(temp);
				FileNum++;
				TotalBytes += FileSize;
			}
		}

		fgets(line, 1024, myList);
	}

	fclose(myList);

	bzero(temp, sizeof(temp));
	sprintf(temp, "%llu", TotalBytes);
	strcpy(temp, f_format(temp));

	bzero(check, sizeof(check));
	sprintf(check, "%llu", FileNum);
	strcpy(check, f_format(check));

	printf("\n\n\tStatistics from the new-stuff.txt: %s bytes - %s files\n", temp, check);
	printf("\n\toldbytes: %llu - oldfiles: %llu - oldtapes: %llu\n", OldBytes, OldFiles, OldTapes);

	bzero(temp, sizeof(temp));
	sprintf(temp, "%llu", TotalBytes + OldBytes);
	strcpy(temp, f_format(temp));

	bzero(check, sizeof(check));
	sprintf(check, "%llu", FileNum + OldFiles);
	strcpy(check, f_format(check));

	printf("\n\n\tMyBBS offline statistics: %s bytes - %s files.\n\n", temp, check);

	return 0;
}
