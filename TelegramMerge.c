#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define VERSION "v1.1"


int main(int argc, char** argv)
{
    if(argc!=2)
    {
	printf("Usage: %s metafile\n",argv[0]);
	return 1;
    } 

    FILE *meta, *output;
    char* meta_name=argv[1];

    meta=fopen(meta_name,"r");

    if(meta==NULL)
    {
	printf("File \"%s\" can't be opened for reading\n",meta_name);
	return 2;
    }

    int c;
    //Printing and skipping the version number
    printf("Created with TelegramSplit ");
    while( (c=getc(meta)) != EOF && c != '\n' )
	printf("%c",c);
    printf("\n");

    //Reading the number of files
    unsigned int temp=0;
    char digit[2]="0";
    while( (c=getc(meta)) != EOF && c != '\n' )
    {
	digit[0]=c;
	temp *= 10;
	temp += atoi(digit);
    }
    const unsigned int NUM_files = temp;
    printf("Number of files: %d\n",NUM_files);

    //Determining length of name of the inputfiles
    int i;
    for(i=1; (c=getc(meta)) != '\n'; i++);
    const unsigned int NAME_length = i;
    //Restoring the cursor to the beginning of the first filename after calling getc
    fseek(meta, -i, SEEK_CUR); 

    //Reading all the input names from metafile
    char input_name[NUM_files][NAME_length+1];
    for(i=0; i<NUM_files; i++)
    {
	fgets(input_name[i],NAME_length,meta);
	fseek(meta, 1L, SEEK_CUR);
	input_name[i][NAME_length]=0;//assumes that the file exists and can be opened
    }

    //Determining the output name from the first input name
    for(i=NAME_length; input_name[0][i] != '_'; i--);
    const int output_len = i;
    char output_name[output_len+1];
    strncpy(output_name, input_name[0], output_len);
    output_name[output_len] = '\0';


    //Try to open all the input files
    int missing_file = 0;
    FILE *input[NUM_files];
    for(i=0; i<NUM_files; i++)
    {
	input[i]=fopen(input_name[i],"r");

	if(input[i] == NULL)
	{
	    missing_file=1;
	    input_name[i][NAME_length]=1;//mark that file as missing
	    printf("File \"%s\" couldn't be found\n",input_name[i]);
	}
    }

    //Exit the program if at least one input file is missing, closing all other files succesfully opened
    if(missing_file==1)
    {
	for(i=0; i<NUM_files; i++)
	    if(input_name[i][NAME_length] != 1)//Close only those files that could be opened
	    {
		//printf("closing %s\n",input_name[i]);
		fclose(input[i]);
	    }
	fclose(meta);
	return 3;
    }//No missing input files from now on...


    //Check if output file already exists
    char proceed[strlen("YES")+1];
    proceed[strlen("YES")]='\0';
    output=fopen(output_name,"r");
    if(output != NULL) //it already exists
    {
	fclose(output);
	printf(" \t-Warning-\n \"%s\" already exsists.\n",output_name);
	printf(" Do you want to proceed and overwrite it? (type YES): ");
	scanf("%3s", proceed);

	if( strncmp(proceed, "YES", strlen("YES")) != 0) // if NOT YES
	{
	    printf("Aborting\n");
	    //Closing all previously opened files...
	    //...the meta file
	    fclose(meta);
	    //...and all the input files
	    for(i=0; i<NUM_files; i++)
		fclose(input[i]);
	    return 4;
	}
	printf("Proceeding...\n");
    }


    //Open output file
    output=fopen(output_name,"w");

    if(output == NULL)
    {
	printf("File \"%s\" can't be open for writing\n",output_name);
	fclose(meta);
	for(i=0; i<NUM_files; i++)
	    fclose(input[i]);
	return 5;
    }


    //Actually merging all files
    for(i=0; i<NUM_files; i++)
    {
	printf("(%d/%d) merging file %s ", i+1, NUM_files, input_name[i]);
	fflush(stdout);
	while( (c = getc(input[i])) != EOF )
	    putc(c, output);

	fclose(input[i]);

	printf("(Done)\n");
	fflush(stdout);
    }


    printf("Finalizing %s ",output_name);
    fflush(stdout);
    fclose(meta);
    fclose(output);
    printf("(Done)\n");
    return 0;
}
