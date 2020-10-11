#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define VERSION "v1.0"
#define _1MB 1048576
#define _1500MB 1500 * _1MB
#define FILE_SIZE_LIMIT _1500MB


void int_to_string(unsigned int value, char* buf, int size);

int main(int argc, char** argv)
{
    if(argc!=3)
    {
	printf("Usage: %s INfilename OUTfilename\n",argv[0]);
	return 1;
    }

    char* input=argv[1];
    char* output=argv[2];

    char meta_name[strlen(output)+strlen("_meta")+1];
    meta_name[0]='\0';

    strcat(meta_name,output);
    strcat(meta_name,"_meta");

    FILE *in,*out,*meta;

    in=fopen(input,"r");

    if(in==NULL)
    {
	printf("File \"%s\" can't be opened for reading\n",input);
	return 2;
    }

    meta=fopen(meta_name,"w");

    if(meta==NULL)
    {
	printf("File \"%s\" can't be opened for writing\n",meta_name);
	return 4;
    }

    fputs(VERSION,meta);
    putc('\n',meta);

    fseek(in, 0L, SEEK_END);
    const long unsigned int size = ftell(in);
    fseek(in, 0L, SEEK_SET);
    const int units = size/(FILE_SIZE_LIMIT);
    int tail=0;

    if( size % (FILE_SIZE_LIMIT) > 0 )
	tail=1;

    const int NUM_files = units+tail;

    printf("The size of %s is: %ld bytes\n",input,size);
    printf("You need to create %d files\n",NUM_files);

    int c_files=NUM_files;
    int buf_length;
    for(buf_length=0; c_files != 0; buf_length++)
	c_files /=10;
    if(buf_length <=1)
	buf_length=2;
    buf_length++;

    char dest[strlen(output)+buf_length+1]; 
    dest[0]='\0';
    strcat(dest,output);
    strcat(dest,"_");

    char *ext = (char*) malloc( (buf_length) * sizeof(char) );

    //initialize the buffer with '\0'
    for(int i=0;i<buf_length;i++)
	ext[i]='\0';

    int_to_string(NUM_files,ext,buf_length);
    fputs(ext,meta);
    putc('\n',meta);

    /*
    printf("The second digit of 12345678 is %d\n", (12345678 / 10) % 10 );

    int_to_string(units,ext,buf_length);
    printf("The string %d is %s\n",units,ext);

    int_to_string(12345678,ext,buf_length);
    printf("The string 12345678 is %s\n",ext);

    int_to_string(2,ext,buf_length);
    printf("The string 2 is %s\n",ext);

    int_to_string(2,ext,buf_length);
    printf("The string 2 is %s\n",ext);

    int_to_string(12,ext,buf_length);
    printf("The string 12 is %s\n",ext);
    */

    int i,c;
    long unsigned int j;
    for(i=0;i<NUM_files;i++)
    {
	int_to_string(i,ext,buf_length);
	strcat(dest,ext);

	out=fopen(dest,"w");

	if(out==NULL)
	{
	    printf("File \"%s\" can't be opened for writing\n",dest);
	    return 3;
	}

	printf("(%d/%d) Writing destination: %s\n",i+1,units+tail,dest);
	
	fputs(dest,meta);
	putc('\n',meta);

	j= 0;
	while( j <  FILE_SIZE_LIMIT && (c = getc(in)) != EOF )
	{
	    putc(c,out);
	    j++;
	}

	fclose(out);

	dest[strlen(output)+1]='\0';
    }

	/*
    if( tail == 1 )
    {
	int_to_string(i,ext,buf_length);
	strcat(dest,ext);

	out=fopen(dest,"w");

	if(out==NULL)
	{
	    printf("File \"%s\" can't be opened for writing\n",dest);
	    return 3;
	}

	printf("(%d/%d) Writing destination: %s\n",i+1,units+tail,dest);

	fputs(dest,meta);
	putc('\n',meta);

	while( (c = getc(in)) != EOF )
	{
	    putc(c,out);
	}

	fclose(out);
    }
	*/


    fclose(in);
    fclose(meta);
    free(ext);
    printf("Done\n");
    return 0;
}

void int_to_string(unsigned int value, char* buf, int size)
{
    char last_digit;

    int count=0;

    do
    {
	last_digit = '0' + ( value % 10 );
	buf[size-2-count] = last_digit;
	value /= 10;
	count++;
    }
    while( value != 0 && count < size-1);

    for(int i=0;i<=size-2-count;i++)
	buf[i]='0';

    buf[size-1]='\0';
}
