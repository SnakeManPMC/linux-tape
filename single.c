#include <stdio.h>
#include <string.h>
#include "../include/pmc.h"

int main(int x, char *argc[])
{
	char *p;
	char templine[1024];
	char filesize[1024];
	char filename[1024];
	char sqlbuf[2048];
	int fc;
	char buf[1024], temp[80];
	unsigned long long temp_size;
	unsigned long long total_megs;
	unsigned long long i = 1;

	FILE *testlist;
	FILE *outlist;
	FILE *mysql;

/* here is the NEW LINUX type listing */
	fc = 0;

	testlist = fopen(argc[1], "rt");
	if (!testlist)
	{
		printf("okay file %s could not opened\n", argc[1]);
		exit(1);
	}
	fgets(templine, 1024, testlist);

	mysql = fopen("mysql.dump", "wt");
	if (!mysql)
	{
		printf("mysql.dump went belly up, no can do honey!\n");
		exit(1);
	}

	while (!feof(testlist))
	{
		i++;
		p = strtok(templine, " ");

		p = strtok(NULL, " ");
		p = strtok(NULL, " ");

		strcpy(filesize, p); /* filesize from token */

		p = strtok(NULL, " ");
		p = strtok(NULL, " ");
		p = strtok(NULL, " ");
//    strcpy(filename,p); /* filename from token */
		bzero(filename, sizeof(filename));
		strncpy(filename, p, strlen(p) - 1);
/*printf("filename: %s\n",filename);*/

		temp_size = 0;
		sscanf(filesize, "%d", &temp_size);
/*printf("temp_size: %llu\n",temp_size);*/

		sprintf(sqlbuf, "INSERT INTO bbsOfflines VALUES (%llu,'%s','%llu','%s','pylly');\n", i, argc[1], temp_size, filename);
		fputs(sqlbuf, mysql);

		if (temp_size > 0)
		{
			total_megs = (total_megs + temp_size);
		}

		sprintf(buf, "filesize: %llu filename: %s", temp_size, filename);
		fgets(templine, 1024, testlist);
	}

	fclose(testlist);

	outlist = fopen("singletape.txt", "wt");
	if (!outlist)
	{
		printf("error at singletape.txt\n");
		exit(1);
	}

	sprintf(buf, "\n\n\tStats of %s tape catalog\n\n", argc[1]);
	fputs(buf, outlist);
	printf(buf);

	sprintf(temp, "%llu", i);
	strcpy(temp, f_format(temp));
	strcpy(buf, temp);

	sprintf(temp, "%llu", total_megs);
	strcpy(temp, f_format(temp));
	strcat(buf, " files, ");
	strcat(buf, temp);

	sprintf(temp, "%llu", total_megs / (1024 * 1024));
	strcpy(temp, f_format(temp));
	strcat(buf, " bytes --- ");
	strcat(buf, temp);

	sprintf(temp, "%llu", total_megs / (1024 * 1024 * 1024));
	strcpy(temp, f_format(temp));
	strcat(buf, "mb, ");
	strcat(buf, temp);
	strcat(buf, "gb\n\n");

	fputs(buf, outlist);
	printf(buf);

	fclose(outlist);
	fclose(mysql);
	return 0;
}