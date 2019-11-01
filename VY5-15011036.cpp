#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int MAX=200;

typedef struct PostStack{
	int top;
	int st[200];
}postStack;

void initializeV2(PostStack* postStack){
	postStack->top=0;
}

bool isEmptyV2(PostStack* postStack){
	if(postStack->top==0)
		return true;
	else
		return false;
}
bool isFullV2(PostStack* postStack){
	if(postStack->top==MAX)
		return true;
	else
		return false;
}

void pushV2(PostStack* postStack, int value){
	if(!isFullV2(postStack)){
		postStack->st[postStack->top]=value;
		postStack->top++;
	}else
		printf("Stack full!\n");
}
int popV2(PostStack* postStack){
	int value;
	if(isEmptyV2(postStack)){
		printf("Stack bos!\n");
		exit;
	}else{
		postStack->top--;
		value=postStack->st[postStack->top];
		return value;
	}
}

void postStackYazdir(PostStack* postStack,FILE* fw){
	PostStack* postStack2=(PostStack*)malloc(sizeof(PostStack));
	initializeV2(postStack2);
	int x;
	printf(" \nYigin:");
	fprintf(fw,"%s"," \nYigin:");
	while(!isEmptyV2(postStack)){
		x=popV2(postStack);
		pushV2(postStack2,x);
	}
	while(!isEmptyV2(postStack2)){
		x=popV2(postStack2);
		printf(" %d",x);
		fprintf(fw," %d",x);
		pushV2(postStack,x);
	}
}

void postFixYazdir(char* postFix, int count, FILE* fw){
	printf("\nPostfix:");
	fprintf(fw, "%s", "\nPostfix:");
	for(int i=0;i<count;i++){
		printf("%c",postFix[i]);
		fputc(postFix[i],fw);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct Stacks{
	int top;
	char st[200];
}stack;

void initialize(Stacks* stack){
	stack->top=0;
}

bool isEmpty(Stacks* stack){
	if(stack->top==0)
		return true;
	else
		return false;
}
bool isFull(Stacks* stack){
	if(stack->top==MAX)
		return true;
	else
		return false;
}

void push(Stacks* stack, char value){
	if(!isFull(stack)){
		stack->st[stack->top]=value;
		stack->top=stack->top+1;
	}else
		printf("Stack full!\n");
}
char pop(Stacks* stack){
	int value;
	if(isEmpty(stack)){
		printf("Stack bos!\n");
		exit;
	}else{
		stack->top--;
		value=stack->st[stack->top];
		return value;
	}
}
void stackYazdir(Stacks* stack,FILE* fw){
	Stacks* stack2=(Stacks*)malloc(sizeof(Stacks));
	initialize(stack2);
	int x;
	printf("\t Yigin:");
	fprintf(fw,"%s","\t Yigin:");
	while(!isEmpty(stack)){
		x=pop(stack);
		push(stack2,x);
	}
	while(!isEmpty(stack2)){
		x=pop(stack2);
		printf("%c",x);
		fputc(x,fw);
		push(stack,x);
	}
}


bool isOperand(char ch){
	return (ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='^' || ch=='(' || ch==')' );
}

int Prec(char ch)
{
    switch (ch)
    {
    case '+':
    case 'û':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default :
    	return -1;
    }
}

int operations(int tmp1,int tmp2, char ch){
	int x,y;
	switch (ch)
    {
    case '+':return tmp1+tmp2;
    case 'û':
	case '-':return tmp1-tmp2;
    case '*':return tmp1*tmp2;
    case '/':return tmp1/tmp2;
    case '^':
	int tmp3=1;
	for(int i=0;i<tmp2;i++)
    	tmp3*=tmp1;	
	return tmp3;
	}
}

int post(Stacks* stack,char* postFix,int count,char ch,FILE* fw){
	char tmp;
	bool control=true;
	if(!isOperand(ch)){
		postFix[count]=ch;
		count++;
	}else if(ch =='('){
		push(stack,ch);
	}else if(ch ==')'){
		tmp=pop(stack);
		while(tmp!='('){
			postFix[count]=tmp;
			count++;
			tmp=pop(stack);
		}
	}else{
		if(!isEmpty(stack)){
		tmp=pop(stack);
		while(Prec(ch)<= Prec(tmp) && control){
			postFix[count]=tmp;
			count++;
			if(isEmpty(stack))
				control=false;	
			else
				tmp=pop(stack);
		}
		if(control)
			push(stack,tmp);
		}
			push(stack,ch);
	}
	postFixYazdir(postFix, count, fw);
	stackYazdir(stack, fw);
	return count;
}


int solve(PostStack* postStack,char* postFix,int count,int* sayilar,FILE* fw){
	int i=0;
	int tmp1,tmp2;
	while(i<count){
	if(!isOperand(postFix[i])){
		if(postFix[i]>='a' && postFix[i]<='z')
			pushV2(postStack,sayilar[postFix[i]-'a']);
		else
			pushV2(postStack,postFix[i]-'0');

	}else{
		tmp2=popV2(postStack);
		tmp1=popV2(postStack);
		tmp1=operations(tmp1,tmp2,postFix[i]);
		pushV2(postStack,tmp1);
	}
	i++;
	postStackYazdir(postStack,fw);
	}
	return popV2(postStack);
}


int main(){
	int* sayilar;
	int count=0,i,j,k,tanim;
	Stacks *stack;
	PostStack *postStack;
	char* postFix;
	FILE *fp;	
	FILE *fw;
	
	postFix=(char*)malloc(200*sizeof(char));
	postStack=(PostStack*)calloc(200,sizeof(PostStack));
	sayilar=(int*)calloc(26,sizeof(int));	
	stack=(Stacks*)malloc(sizeof(Stacks));
	initialize(stack);
	initializeV2(postStack);
	bool control=true;
	char** data=(char**)calloc(55,sizeof(char*));
	for(int i=0;i<55;i++)
		data[i]=(char*)calloc(55,sizeof(char));
	char* line=(char*)calloc(50,sizeof(char));				
	char* ptr=(char*)malloc(50*sizeof(char));
	int size=0;												
	fp=fopen("input.txt","r");
	fw=fopen("output.txt","w");
	while(!feof(fp)){					
		fgets(line,50,fp);									
		ptr=strtok(line,", ");					
		data[size][0]=ptr[0];									
		j=1;
		
		ptr=strtok(NULL,", ");								
		while(ptr[0]!=';'){				
			data[size][j]=ptr[0];
			j++;
			ptr=strtok(NULL,", ");						
		}
		if(j==3){
			sayilar[data[size][0]-'a']=data[size][2]-'0';
			data[size][55]='f';
		}
		size++;
	}
	fclose(fp);													//Dosya pointer'ý kapatýlýyor
	
	int m;
	
	printf("Verilerimiz\n");
	fprintf(fw,"%s","Verilerimiz\n");
	for(int k=0;k<size;k++){
		for(m=0;m<50;m++){
			printf("%c",data[k][m]);
			fputc(data[k][m],fw);
		}
		fputc('\n',fw);
		printf("\n");
	}
	
	for(i=0;i<size;i++){
		count=0;
		if(data[i][55]!='f'){
			fputc('\n',fw);
			printf("\n");
			for(m=0;m<50;m++){
				printf("%c",data[i][m]);
				fputc(data[i][m],fw);
			}
			for(j=2;data[i][j]!='\0';j++)
				count=post(stack,postFix,count,data[i][j],fw);
			
			while(!isEmpty(stack)){
				postFix[count]=pop(stack);
				count++;
			}
			while(!isEmpty(stack)){
				pop(stack);
			}
			printf("\n\nPostfix: %c=",data[i][0]);
			fprintf(fw,"%s","\n\nPostfix:");
			fputc(data[i][0],fw);
			fputc('=', fw);
			for(k=0;k<count;k++){
				fputc(postFix[k],fw);
				printf("%c",postFix[k]);
			}
			fputc('\n',fw);
			printf("\n");
			sayilar[data[i][0]-'a']=solve(postStack,postFix,count,sayilar,fw);
			printf("\n\n\n");
			fputc('\n',fw);
			fputc('\n',fw);
			fputc('\n',fw);
		}
	}
	printf("\n");
		printf("%c Degeri=%d\n",'a',sayilar[0]);
		fprintf(fw,"\n\n%s","\n a Degeri=");
		fprintf(fw,"%d",sayilar[0]);
		printf("%c Degeri=%d\n",'b',sayilar[1]);
		fprintf(fw,"%s","\n b Degeri=");
		fprintf(fw,"%d",sayilar[1]);
		printf("%c Degeri=%d\n",'c',sayilar[2]);
		fprintf(fw,"%s","\n c Degeri=");
		fprintf(fw,"%d",sayilar[2]);
		printf("%c Degeri=%d\n",'d',sayilar[3]);
		fprintf(fw,"%s","\n d Degeri=");
		fprintf(fw,"%d",sayilar[3]);
		printf("%c Degeri=%d\n",'e',sayilar[4]);
		fprintf(fw,"%s","\n e Degeri=");
		fprintf(fw,"%d",sayilar[4]);
		printf("%c Degeri=%d\n",'f',sayilar[5]);
		fprintf(fw,"%s","\n f Degeri=");
		fprintf(fw,"%d",sayilar[5]);
		printf("%c Degeri=%d\n",'g',sayilar[6]);
		fprintf(fw,"%s","\n g Degeri=");
		fprintf(fw,"%d",sayilar[6]);
		printf("%c Degeri=%d\n",'h',sayilar[7]);
		fprintf(fw,"%s","\n h Degeri=");
		fprintf(fw,"%d",sayilar[7]);
		printf("%c Degeri=%d\n",'i',sayilar[8]);
		fprintf(fw,"%s","\n i Degeri=");
		fprintf(fw,"%d",sayilar[8]);
		printf("%c Degeri=%d\n",'j',sayilar[9]);
		fprintf(fw,"%s","\n j Degeri=");
		fprintf(fw,"%d",sayilar[9]);
		printf("%c Degeri=%d\n",'k',sayilar[10]);
		fprintf(fw,"%s","\n k Degeri=");
		fprintf(fw,"%d",sayilar[10]);
		printf("%c Degeri=%d\n",'l',sayilar[11]);
		fprintf(fw,"%s","\n l Degeri=");
		fprintf(fw,"%d",sayilar[11]);
		printf("%c Degeri=%d\n",'m',sayilar[12]);
		fprintf(fw,"%s","\n m Degeri=");
		fprintf(fw,"%d",sayilar[12]);
		printf("%c Degeri=%d\n",'n',sayilar[13]);
		fprintf(fw,"%s","\n n Degeri=");
		fprintf(fw,"%d",sayilar[13]);
		printf("%c Degeri=%d\n",'o',sayilar[14]);
		fprintf(fw,"%s","\n o Degeri=");
		fprintf(fw,"%d",sayilar[14]);
		printf("%c Degeri=%d\n",'p',sayilar[15]);
		fprintf(fw,"%s","\n p Degeri=");
		fprintf(fw,"%d",sayilar[15]);
		printf("%c Degeri=%d\n",'q',sayilar[16]);
		fprintf(fw,"%s","\n q Degeri=");
		fprintf(fw,"%d",sayilar[16]);
		printf("%c Degeri=%d\n",'r',sayilar[17]);
		fprintf(fw,"%s","\n r Degeri=");
		fprintf(fw,"%d",sayilar[17]);
		printf("%c Degeri=%d\n",'s',sayilar[18]);
		fprintf(fw,"%s","\n s Degeri=");
		fprintf(fw,"%d",sayilar[18]);
		printf("%c Degeri=%d\n",'t',sayilar[19]);
		fprintf(fw,"%s","\n t Degeri=");
		fprintf(fw,"%d",sayilar[19]);
		printf("%c Degeri=%d\n",'u',sayilar[20]);
		fprintf(fw,"%s","\n u Degeri=");
		fprintf(fw,"%d",sayilar[20]);
		printf("%c Degeri=%d\n",'v',sayilar[21]);
		fprintf(fw,"%s","\n v Degeri=");
		fprintf(fw,"%d",sayilar[21]);
		printf("%c Degeri=%d\n",'w',sayilar[22]);
		fprintf(fw,"%s","\n w Degeri=");
		fprintf(fw,"%d",sayilar[22]);
		printf("%c Degeri=%d\n",'x',sayilar[23]);
		fprintf(fw,"%s","\n x Degeri=");
		fprintf(fw,"%d",sayilar[23]);
		printf("%c Degeri=%d\n",'y',sayilar[24]);
		fprintf(fw,"%s","\n y Degeri=");
		fprintf(fw,"%d",sayilar[24]);
		printf("%c Degeri=%d\n",'z',sayilar[25]);
		fprintf(fw,"%s","\n z Degeri=");
		fprintf(fw,"%d",sayilar[25]);
	return 0;	
}
