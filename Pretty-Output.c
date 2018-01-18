#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main (int argc, char **argv)
{
	char *input = argv[1];
    FILE *input_file;

	input_file = fopen(input, "r");

	if (input_file == 0)
    {
        perror("Canot open input file\n");
        exit(-1);
    }

    char buf[10000];
    char prev[10000];
    char pprev[10000];
    char space[10000];

    int ccount = 0;
    int nccount = 0;
    int sec_nccount = 0;
    int pcount = 0;

    while (!feof(input_file))
    {
    	// Read the input word by word
    	fscanf(input_file,"%s",buf);

    	if(strstr(buf,"<")!=0)
    	{
    		printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "\n", buf);
    		ccount = 1;
    		strcat(space,"   ");

    		while(ccount != 0)
    		{
    			strcpy(pprev, prev);
    			strcpy(prev, buf);
    			fscanf(input_file,"%s",buf);
    			if(strstr(buf,"</")!=0 || strstr(buf,")")!=0 || strstr(buf,"}")!=0)
    			{
    				if(nccount == 1 || strstr(pprev,",")!=0)
					{
						printf("\n");
						nccount = 0;
					}
    				
    				// Remove spaces
					strcpy(space, space+3);

    				ccount = ccount - 1;
    				if(strstr(buf,"}")!=0)
    				{
    					printf(ANSI_COLOR_BLUE "%s%s" ANSI_COLOR_RESET "\n", space, buf);
    				}
    				else if(strstr(buf,")")==0)
    				{
    					printf(ANSI_COLOR_GREEN "%s%s" ANSI_COLOR_RESET "\n", space, buf);
    				}
    				else
    				{
    					printf(ANSI_COLOR_CYAN "%s%s" ANSI_COLOR_RESET "\n", space, buf);	
    				}
    			}
    			else if(strstr(buf,"<")!=0 || strstr(buf,"(")!=0 || strstr(buf,"{")!=0)
    			{
    				if(nccount == 1 || strstr(prev,"|->")!=0 || strstr(prev,",")!=0 || strstr(prev,"~>")!=0
    					|| strstr(prev,"*")!=0 || strstr(prev,"=")!=0 || strstr(prev,"+")!=0
    					|| strcmp(prev,"-")==0 || strcmp(prev,">")==0 || strcmp(prev,"<")==0
    					|| strcmp(prev,"->")==0 || strcmp(prev,"++")==0 || strcmp(prev,".")==0)
    				{
						printf("\n");
						nccount = 0;
					}

    				ccount = ccount + 1;
    				if(strstr(buf,"{")!=0)
    				{
    					printf(ANSI_COLOR_BLUE "%s%s" ANSI_COLOR_RESET "\n", space, buf);
    				}
    				else if(strstr(buf,"(")==0)
    				{
    					printf(ANSI_COLOR_GREEN "%s%s" ANSI_COLOR_RESET "\n", space, buf);
    				}
    				else
    				{
    					printf(ANSI_COLOR_CYAN "%s%s" ANSI_COLOR_RESET "\n", space, buf);	
    				}
    				strcat(space,"   ");
    			}
    			else
    			{
    				if(strstr(prev,"|->")!=0 || strstr(prev,"~>")!=0 || strstr(prev,"*")!=0
    					|| strstr(prev,"=")!=0 || strstr(prev,"+")!=0 || strcmp(prev,"-")==0
    					|| strcmp(prev,">")==0 || strcmp(prev,"<")==0 || strcmp(prev,"->")==0
    					|| strcmp(prev,"++")==0 || strcmp(prev,".")==0)
    				{
    					printf("%s\n", buf);
    					nccount = 0;
    				}
    				else if(strstr(prev,",")!=0)
    				{
    					printf("%s", buf);
    					nccount = 0;
    				}
    				else if(strstr(buf,"|->")!=0 || strstr(buf,",")!=0 || strstr(buf,"~>")!=0
    					|| strstr(buf,"*")!=0 || strstr(buf,"=")!=0 || strstr(buf,"+")!=0
    					|| strcmp(buf,"-")==0 || strcmp(buf,">")==0 || strcmp(buf,"<")==0
    					|| strcmp(buf,"->")==0 || strcmp(buf,"++")==0 || strcmp(buf,".")==0)
    				{
    					if((strstr(prev,")")!=0 || strstr(prev,"(")!=0) && (strstr(buf,",")!=0 || strstr(buf,"+")!=0
    						|| strcmp(buf,"-")==0 || strcmp(buf,">")==0 || strcmp(buf,"<")==0
    						|| strcmp(buf,"->")==0 || strcmp(buf,"++")==0 || strcmp(buf,".")==0))
    					{
    						printf(ANSI_COLOR_YELLOW "%s%s " ANSI_COLOR_RESET, space, buf);
    					}
    					else if((strstr(pprev,"->")!=0 || strstr(pprev,"~>")!=0 || strstr(pprev,"=")!=0)
    						&& (strstr(buf,",")!=0 || strcmp(buf,".")==0 || strcmp(buf,"=")==0))
    					{
    						printf(ANSI_COLOR_YELLOW "%s%s " ANSI_COLOR_RESET, space, buf);
    					}
    					else
    					{
    						printf(ANSI_COLOR_YELLOW " %s " ANSI_COLOR_RESET, buf);
    					}
    					nccount = 0;
    				}
    				else if(strstr(buf,"[")!=0)
    				{
    					printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, buf);
    					while(strstr(buf,"]")==0)
    					{
    						strcpy(pprev, prev);
			    			strcpy(prev, buf);
			    			fscanf(input_file,"%s",buf);
			    			printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET, buf);
			    			nccount = 1;	
    					}
    				}
					else				
    				{
    					if(nccount == 1)
    						printf("\n");
    					printf("%s%s", space, buf);
    					nccount = 1;
    				}
    				
    			}
    		}
    	}
    	else
    	{
    		printf("%s\n", buf);
    	}
    }
}