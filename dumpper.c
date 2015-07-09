#include <stdio.h>
#include <string.h>

FILE *mysqld;
FILE *output;

int main()
{
	char line[1024];
	char temp[1024];
	char out[1024];
	int id = 292999;

	mysqld = fopen("mysql-dumpper.txt", "rt");
	if (!mysqld)
	{
		printf("error at mysql-dumpper!\n");
		exit(1);
	}

	output = fopen("mysql2.dump", "wt");
	if (!output)
	{
		printf("error at mysql2.dump!\n");
		exit(1);
	}

	fgets(line, 1024, mysqld);

	while (!feof(mysqld))
	{
		strcpy(temp, line);
		bzero(line, sizeof(line));
		strncpy(line, temp, strlen(temp) - 1);

		id++;
		sprintf(out, "INSERT INTO bbsOfflines VALUES (%i,'zero','zero','%s','foobar');\n", id, line);
		fputs(out, output);

		bzero(line, sizeof(line));
		bzero(out, sizeof(out));
		bzero(temp, sizeof(temp));

		fgets(line, 1024, mysqld);
	}

	fclose(mysqld);
	fclose(output);
	return 0;
}
