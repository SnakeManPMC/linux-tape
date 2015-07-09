#include <stdio.h>
#include <string.h>
#include <glob.h>
#include <pmc.h>

char *p;
char templine[1024];
char filesize[1024];
char filename[1024];
int fc;
char buf[1024], temp[80];
char highscoretape[80];
char tiny_tape_name[80];
unsigned long long temp_size;
unsigned long long total_megs;
unsigned long long total_tapes;
unsigned long long i;
unsigned long long highscore, tiny_tape = 999999999, dat_size;

FILE *testlist;
FILE *outlist;

glob_t gl;

/* generic Linux */
int LinuxType(char *Ghetto)
{
	char *utemp[80];
	int ui = 0;

	globfree(&gl);
	sprintf(buf, "/offline/%s.listing", Ghetto);

	glob(buf, 0, 0, &gl);

	for (fc = 0; fc < gl.gl_pathc; fc++)
	{
		sprintf(buf, "%s", gl.gl_pathv[fc]);
		ui = splitpath(utemp, gl.gl_pathv[fc]);

		testlist = fopen(buf, "rt");
		if (!testlist)
		{
			printf("okay file %s could not opened\n", utemp[ui]);
			break;
		}
		fgets(templine, 1024, testlist);
		dat_size = 0;

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
			strcpy(filename, p); /* filename from token */

			temp_size = 0;
			sscanf(filesize, "%d", &temp_size);

			if (temp_size > 0)
			{
				total_megs += temp_size;
				dat_size += temp_size;
			}

			fgets(templine, 1024, testlist);
		}

		fclose(testlist);

		if (dat_size > highscore)
		{
			strcpy(highscoretape, utemp[ui]);
			highscore = dat_size;
		}

		if (dat_size < tiny_tape)
		{
			tiny_tape = dat_size;
			strcpy(tiny_tape_name, utemp[ui]);
		}
	}

	total_tapes += fc;

	return 0;
}

/* ending the shit */
int Close_Shot()
{
	outlist = fopen("tapestat.txt", "wt");
	if (!outlist)
	{
		printf("error at tapestat.txt\n");
		exit(1);
	}

	sprintf(buf, "\n\n\tMyBBS offline storage report.\n\n");
	fputs(buf, outlist);
	printf(buf);

	sprintf(temp, "%llu", total_tapes);
	strcpy(temp, f_format(temp));
	strcpy(buf, temp);

	sprintf(temp, "%llu", i);
	strcpy(temp, f_format(temp));
	strcat(buf, " DAT tapes in ");
	strcat(buf, temp);

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

	sprintf(buf, "Smallest tape: %s - %llumb\n\n", tiny_tape_name, tiny_tape / (1024 * 1024));
	fputs(buf, outlist);
	printf(buf);

	sprintf(buf, "HIGHSCORE tape: %s with %llubytes, %llumb\n\n", highscoretape, highscore, highscore / (1024 * 1024));
	fputs(buf, outlist);
	printf(buf);

	globfree(&gl);

	fclose(outlist);

	return 0;
}

int splitpath(char **args, char *p)
{
	int c = 0;

	while (*p == '/')
		p++;

	args[c++] = p;
	while (*p)
	{
		if (*p == '/')
		{
			*(p++) = 0;
			args[c++] = p;
			while(*p == '/')
				p++;
		}
		else
			p++;
	}
	args[c] = 0;

	return c - 1;
}

int main()
{
	printf("\nProcessing *.listing files...\n");
	LinuxType("*");
	Close_Shot();

	return 0;
}
