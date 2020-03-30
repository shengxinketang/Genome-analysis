#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <getopt.h>


struct fastqInfo {
                char name[100];
                char base[200];
                char baseQ[200];
};


typedef struct {
    int max;
    int min;
}Data;

Data* max_min_ascii(char *baseQ)

{
	Data *data = (Data *)malloc(sizeof(Data)); ;
        int i=0;
	
	data->min=73;
        data->max=0;

	while (baseQ[i]!='\0' )
	{
		if(baseQ[i]>data->max)
		{	
			data->max=baseQ[i];
//			printf("baseQ[i]=%c\tmax=%d\n", baseQ[i], max);
		}
			
		if(baseQ[i]<data->min)
		{
			data->min=baseQ[i];
//			printf("baseQ[i]=%c\tmin=%d\n", baseQ[i], min);
		}
		i++;
//		printf("data->max=%d\tdata->min=%d\n", data->max, data->min);
	}
        return data;
}




int judge_phred(int min, int max)
{
//	printf("max=%d\tmin=%d\n", max, min);
	if (min <= 58 &&  max <= 73)
		printf("fastq format is phred+33\n");
	else if(min < 64 &&  max >= 73)
		printf("fastq format is soleax+64\n");
	else if(min > 59 &&  max >= 73)
		printf("fastq format is phred+64\n");
	else if(min > 59 &&  max <= 73)
		printf("Please read more read to judge the type\n");
	return 0;		
}



int main(int argc, char **argv)
{

	int c;
	int i=0;
	int read_sum=1000 ; // total fastq to read to judge the type	 
	
	int min=73;
	int max=0;
	int min_read=73;
	int max_read=0;

	if(argc<2){
		printf("Usage:\n");
        	printf("./fastq_phred [option] fastq  \n");
                printf("\t-n : get n reads to judge the type\n");
                return(0);
        }

	while((c = getopt(argc, argv, "n:")) >= 0)
        {
                switch(c)
                {
                        case 'n': read_sum = atoi(optarg); break;
                        default:
                                printf("unknow argument\n");
                        return -1;
                }
        }



	struct fastqInfo *fastq=(struct fastqInfo*)malloc( read_sum*sizeof(struct fastqInfo));

	FILE *fp;
	fp=fopen(argv[optind],"r");

	while(!feof(fp) && i<read_sum)
	{	
		fscanf(fp,"%[^\n]\n%s\n%*[^\n]\n%s\n", fastq[i].name, fastq[i].base,  fastq[i].baseQ);
		i++;
	}
	fclose(fp);

	for (i=0;i<read_sum;i++)
	{
		Data *getdata =	max_min_ascii(fastq[i].baseQ );
//		printf("max_read=%d\tmin_read=%d\n", getdata->max, getdata->min);
		if(getdata->min < min)
			min = getdata->min;
		if(getdata->max > max)
			max = getdata->max;
		
	}

	judge_phred( min,  max);
	free (fastq);
	
}
