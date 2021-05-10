#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*  */

struct data{
	int **inputs;
	char *title;
	int sofi;
	int sofo;
	int sol;
};
typedef struct data Input;

struct gate{
	int input1;
	int input2;
	char nofg[15];
	struct gate *next;
};
typedef struct gate Gate;

void find_lines(Input *data);
void read_inputfile(Input *data);
int read_inform(Gate *firstg);
void find_result(Gate firstg,Input Inputs,int point,int *results);
void print_result(Input Inputs,int *outputs,int line);
void check_output(int *out1,Input Inputs,int n);
int gate_and(int x,int y);
int gate_or(int x,int y);
int inversion(int x);
int gate_nand(int x,int y);
int gate_xor(int x,int y);


int main(int argc, char *argv[]) {
	Input Inputs;
	Gate firstgate;
	int *outputs=NULL;
	char **gates;
	int i,j,sofg;
	int pointer=0;
	
	find_lines(&Inputs);
	read_inputfile(&Inputs);
	outputs=malloc((Inputs.sofo+1)*sizeof(int));
	
	sofg=read_inform(&firstgate);
	
	for(i=0;i<Inputs.sol;i++){
		find_result(firstgate,Inputs,pointer,outputs);
		print_result(Inputs,outputs,pointer);
		pointer++;
	}
	
	return 0;
}

void find_lines(Input *data){
	FILE *file;
	char tit[30];
	int cot,n,num,counter=0;
	
	printf("Enter the title of your input file:");
	scanf("%s",tit);
	strcat(tit,".txt");
	cot=strlen(tit)+1;
	
	data->title=malloc(cot*sizeof(char));
	strcpy(data->title,tit);
	
	if((file=fopen(data->title,"rt"))==NULL){
		printf("\nText file could not opened.");
		exit(1);
	}
	
	fscanf(file,"%d",&num);
	data->sofi=num;
	n=num;
	
	fscanf(file,"%d",&num);
	data->sofo=num;
	n=n+num;
	
	while(fscanf(file,"%d",&num)!=EOF){
		counter++;
	}
	
	data->sol=(counter/n);
	
	fclose(file);
}

void read_inputfile(Input *data){
	FILE *file;
	int num,i,j,lines,k;
	
	if((file=fopen(data->title,"rt"))==NULL){
		printf("\nText file could not opened.");
		exit(1);
	}
	
	lines=data->sol;
	k=(data->sofi+data->sofo);
	
	data->inputs=NULL;
	data->inputs=realloc(data->inputs,(lines+1)*sizeof(int *));
	
	for(i=0;i<lines;i++){
		data->inputs[i]=malloc((k+1)*sizeof(int));
	}
	
	for(i=0;i<2;i++)
		fscanf(file,"%d",&num);
	
	for(i=0;i<lines;i++){
		for(j=0;j<k;j++){
			fscanf(file,"%d",&num);
			*(data->inputs[i]+j)=num;
		}
	}
	
	fclose(file);
}

int read_inform(Gate *firstg){
	FILE *file;
	Gate *cg;
	char nog[15];
	int sg,i,in1,in2;
	
	
	if((file=fopen("circuit.txt","rt"))==NULL){
		printf("\nText file could not opened.");
		exit(1);
	}
	
	fscanf(file,"%d",&sg);
	
	cg=firstg;
	for(i=0;i<sg;i++){
		fscanf(file,"%d",&in1);
		cg->input1=in1;
		fscanf(file,"%d",&in2);
		cg->input2=in2;
		fscanf(file,"%s",nog);
		strcpy(cg->nofg,nog);
		if(i<(sg-1)){
			cg->next=malloc(sizeof(Gate));
			cg=cg->next;
		}
		else
			cg->next=NULL;
	}
	
	fclose(file);
	
	return sg;
}

void find_result(Gate firstg,Input Inputs,int point,int *results){
	Gate *g;
	int *outputs;
	int pos,counter=0;
	int in1,in2;
	char ng[5][15]={"or","and","nand","xor"};
	int (*f[5])()={gate_or,gate_and,gate_nand,gate_xor};
	int i=0;
	
	for(g=&firstg;g!=NULL;g=g->next){
		counter++;
	}
	outputs=malloc((counter+1)*sizeof(int));
	counter=0;
	
	pos=0;
	for(g=&firstg;g!=NULL;g=g->next){
		for(i=0;i<4;i++){
			if(strcmp(g->nofg,ng[i])==0)
				break;
		}	
		
		if(g->input1==0 & g->input2==0){
			in1=*(Inputs.inputs[point]+pos);
			pos++;
			in2=*(Inputs.inputs[point]+pos);
			pos++;
		}
		if(g->input1==0 & g->input2!=0){
			in1=*(Inputs.inputs[point]+pos);
			pos++;
			in2=outputs[g->input2-1];
		}
		if(g->input1!=0 & g->input2!=0){
			in1=outputs[g->input1-1];
			in2=outputs[g->input2-1];
		}
		
		outputs[counter]=f[i](in1,in2);
		
		counter++;
	}
	
	for(i=0;i<Inputs.sofo;i++)
		results[i]=outputs[counter-Inputs.sofo+i];
	
	check_output(results,Inputs,point);
}

void print_result(Input Inputs,int *outputs,int line){
	int i;
	
	for(i=0;i<Inputs.sofi;i++)
		printf("input=%d ",*(Inputs.inputs[line]+i));
	printf("\n");
	
	for(i=0;i<Inputs.sofo;i++)
		printf("output=%d ",outputs[i]);
	printf("\n\n");
}

void check_output(int *out1,Input Inputs,int n){
	int i,num;

	printf("\n");
	for(i=0;i<Inputs.sofo;i++){
		num=*(Inputs.inputs[n]+Inputs.sofi+i);
		if(out1[i]==num)
			printf("The output=%d is correct.\n",num);
		else
			printf("The output=%d is incorrect.\n",num);
	}
}

int gate_and(int x,int y){
	int result;
	
	result=x*y;
	
	return result;
}

int gate_or(int x,int y){
	int result;
	
	if(x==0 & y==0)
		result=0;
	else
		result=1;
	
	return result;
}

int inversion(int x){
	
	return 1-x;
}

int gate_nand(int x,int y){
	int result;
	
	result=gate_and(x,y);
	result=inversion(result);
	
	return result;
}

int gate_xor(int x,int y){
	int result,result1,result2;
	
	x=inversion(x);
	y=inversion(y);
	result1=gate_or(x,y);
	
	x=inversion(x);
	y=inversion(y);
	result2=gate_or(x,y);
	
	result=gate_and(result1,result2);
	
	return result;
}
