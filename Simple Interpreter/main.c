#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define maxsize 100
typedef struct
{
    char str[100];
} string;
typedef struct
{
    char key[100];
    char value[100];
} heap;
typedef union
{
    char c;
    float f;
} item;
typedef struct
{
    char *key;
    float value;
    struct Node *right;
    struct Node *left;
} Node;

Node * newNode(char *word,float f)
{
    Node *n=malloc(sizeof(Node));
    n->key=word;
    n->value=f;
    n->right=NULL;
    n->left=NULL;
    return n;
}

Node * insert(Node *node,char *new_word,float f)
{
    if(node==NULL)
        return newNode(new_word,f);
    if(strcasecmp(new_word,node->key)==-1)
        node->left=insert(node->left,new_word,f);
    else if (strcasecmp(new_word,node->key)==1)
        node->right=insert(node->right,new_word,f);
    return node;

}

void inOrderTraverse(Node *n)
{
    if(n)
    {
        inOrderTraverse(n->left);
        printf("%s = %f\n",n->key,n->value);
        inOrderTraverse(n->right);
    }
}
typedef struct
{
    item items [maxsize];
    int top;
} stack;
heap heaps[100];
int sizen=0;
stack * initalize()
{
    stack *s= malloc(sizeof(stack));
    s->top =0;
    return s;
}
void push (stack * s,item value )
{
    s->items[s->top] = value;
    s->top++;

}
item pop(stack * s)
{
    item value;
    s->top--;
    value = s->items[s->top];
    return value;
}
item top (stack * s)
{
    item value;
    value = s->items[s->top-1];
    return value;
}
int isEmpty (stack * s)
{
    return s->top==0;
}
int priority(char o)
{
    switch (o)
    {
    case '+':
    case '-':
        return 0;
    case '*':
    case '/':
        return 1;
    case '^':
        return 2;
    case'(':
        return 3;
    }
}
float Calculate(float num1, float num2,char op)
{
    if (op=='+')
        return num1+num2;
    if (op=='-')
        return num1-num2;
    if (op=='*')
        return num1*num2;
    if (op=='/')
        return num1/num2;
    if (op=='^')
        return pow(num1,num2);
}
float evaluatePostfix(char* postfix)
{
    item *s=initalize();
    float num1,num2,value;
    char *token=strtok(postfix, " ");
    while(token)
    {
        if(strlen(token)==1)
        {
            if(isdigit(token[0]))
            {
                item temp ;
                temp.f=atof(token);
                push(s,temp);
            }
            else
            {
                num2=pop(s).f;
                num1=pop(s).f;
                value = Calculate(num1,num2,token[0]);
                item temp;
                temp.f=value;
                push(s,temp);
            }
        }
        else
        {
            item temp;
            temp.f=atof(token);
            push(s,temp);
        }
        token =strtok(NULL," ");
    }

    return pop(s).f;

}

void infixTopostfix(char *infix, char* postfix)
{
    stack *s=initalize();
    int Position=0;
    strcpy(postfix," ");
    char *token=strtok(infix, " ");
    while(token)
    {
        if(strlen(token)==1)
        {
            if(isdigit(token[0]))
            {
                postfix[Position++]=token[0];
                postfix[Position++]=' ';
            }
            else
            {
                if(token[0]==')')
                {
                    while(!isEmpty(s) && top(s).c !='(')
                    {
                        postfix[Position++]=pop(s).c;
                        postfix[Position++]=' ';
                    }
                    pop(s);
                }
                else
                {

                    if(isEmpty(s))
                    {
                        item temp;
                        temp.c=token[0];
                        push(s,temp);
                    }
                    else
                    {
                        if(priority(token[0])>priority(top(s).c))
                        {
                            item temp;
                            temp.c=token[0];
                            push(s,temp);
                        }

                        else
                        {
                            while(!isEmpty(s) && priority(top(s).c)>=priority(token[0]) && top(s).c !='(')
                            {
                                postfix[Position++]=pop(s).c;
                                postfix[Position++]=' ';
                            }
                            item temp;
                            temp.c=token[0];
                            push(s,temp);
                        }
                    }
                }

            }

        }

        else
        {
            for(int i=0 ; i<strlen(token) ; i++)
                postfix[Position++]=token[i];

            postfix[Position++]=' ';
        }
        token=strtok(NULL," ");
    }
    while(!isEmpty(s))
    {
        postfix[Position++]=pop(s).c;
        postfix[Position++]=' ';
    }
    postfix[Position]='\0';
}

int isop(char* s)
{
    int flag=0;
    if(strcmp(s,"+")==0 ||strcmp(s,"-")==0 ||strcmp(s,")")==0||strcmp(s,"/")==0 ||strcmp(s,"*")==0 ||strcmp(s,"^")==0 ||strcmp(s,"(")==0 )
        flag++;
    return flag;
}
int checkvariable(char* s)
{

    char g[100];
    strcpy(g,s);
    int flag=0;
    char *token;
    token =malloc(20);
    token=strtok(g," ");
    while(token!=NULL)
    {
        if(flag>1)
        {
            if(token[0]!='0')
            {
                if(atof(token)==0&& isop(token)==0)
                {
                    return 1;
                }

            }
        }
        flag++;
        token= strtok(NULL," ");
    }
    return 0;
}
void replace(char* s)
{
    char newstr[100];
    char g[100];
    strcpy(g,s);
    int flag=0;
    char check[100];
    char *token;
    token =malloc(20);
    token=strtok(g," ");
    strcpy(newstr,token);
    strcat(newstr," = ");
    while(token!=NULL)
    {
        if(flag>1)
        {
            if(token[0]!='0' &&atof(token)==0&& isop(token)==0)
            {
                search(token,check);
                if(strcmp(check,"not")==0)
                {
                    printf("Error!! %s not Found\n",token);
                    exit(0);
                }
                else
                {
                    strcat(newstr,check);
                    strcat(newstr," ");
                }
            }
            else
            {
                strcat(newstr,token);
                strcat(newstr," ");
            }
        }
        flag++;
        token= strtok(NULL," ");
    }
    strcpy(s,newstr);
}

void search(char *variable, char* s)
{
    strcpy(s,"not");
    for(int i=0 ; i<sizen; i++)
    {
        if(strcmp(variable,heaps[i].key)==0)
        {
            strcpy(s,heaps[i].value);
        }
    }
}
void evaAndStore(char str[])
{
    char infix[100];
    char postfix[100];
    strcpy(infix," ");
    int flag=0;
    char *token;
    token =malloc(20);
    token=strtok(str," ");
    char kv[20];
    strcpy(kv,token);
    while(token!=NULL)
    {
        if(flag>1)
        {
            strcat(infix,token);
            strcat(infix," ");
        }
        flag++;
        token= strtok(NULL," ");
    }

    infixTopostfix(infix,postfix);
    float f =evaluatePostfix(postfix);
    char s[100];
    search(kv,s);
    if(strcmp(s,"not")==0)
    {
        strcpy(heaps[sizen].key,kv);
        strcpy(heaps[sizen].value, gcvt(f,8,infix));
        sizen++;
    }
    else
    {
        update(kv, gcvt(f,8,infix));
    }
}
void update(char *variable, char* upvalue)
{

    for(int i=0 ; i<sizen; i++)
    {
        if(strcmp(variable,heaps[i].key)==0)
        {
            strcpy(heaps[i].value,upvalue);
        }
    }

}
void checkerror (char * s)
{
    int flag = 0,check =0;
    char * token;
    token = strtok(s," ");
    while( token != NULL )
    {
        if(flag == 0)
        {
            for(int i=0; i<strlen(token); i++)
            {
                if(i==0&&isalpha(token[i])==0)
                {
                    printf("Error!! Invalid Variable Name\n");
                    exit(0);
                }
                else if(isalnum(token[i])==0)
                {
                    printf("Error!! Invalid Variable Name\n");
                    exit(0);
                }
            }
        }

        if(strcmp(token,"(")== 0)
            check ++;
        if(strcmp(token,")") == 0)
        {
            check--;

        }

        if(flag == 1 && strcmp(token,"=") != 0)
        {
            printf("Error!! Missing equal or left hand side has more than one variable\n");
            exit(0);
        }
        flag++;
        token = strtok(NULL, " ");

    }
    if(check != 0)
    {

        printf("Error!! Invalid expression unclosed bracket or closed bracket found without open bracket\n");
        exit(0);
    }
}

void put(char *k,char *v)
{
    char s[100];
    char *tv;
    tv=strtok(k," ");
    tv=strtok(NULL," ");
    strcpy(s,k);
    strcat(s," = ");
    strcat(s,v);
    if(checkvariable(s)!=0)
        replace(s);
    evaAndStore(s);
}

void exchange(char *a, char *b)
{
    char *temp=malloc(100);
    strcpy(temp,a);
    strcpy(a,b);
    strcpy(b,temp);
}
int left(int i)
{
    return 2*i+1;
}
int right(int i)
{
    return 2*i+2;
}
void heapify(int size,int i)
{
    int large = i;
    int l = left(i);
    int r = right(i);

    if (l < size && atof(heaps[l].value)>atof(heaps[large].value))
        large = l;

    if (r < size && atof(heaps[r].value)>atof(heaps[large].value))
        large = r;

    if (large != i)
    {
        exchange(heaps[i].value,heaps[large].value);
        exchange(heaps[i].key,heaps[large].key);
        heapify(size, large);
    }
}


void sortingByHeap(size)
{

    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(size, i);

    for (int j = size - 1; j >= 0; j--)
    {
        exchange(heaps[0].value,heaps[j].value);
        exchange(heaps[0].key,heaps[j].key);
        heapify(j, 0);
    }
    for(int k=0; k<size; k++)
    {
        printf("%s = %s\n",heaps[k].key,heaps[k].value);
    }
}
void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}

void Run()
{
    string strings[100];
    char st[100];
    int sizes=0;
    FILE *f;
    f=fopen("src.txt","r");
    while(!feof(f))
    {
        fgets(st,100,f);
        strcpy(strings[sizes].str,st);
        checkerror(st);
        sizes++;
    }
    strcat(strings[sizes-1].str," * 1");
    printf("Loaded Successfully\n\n");
    for(int i=0 ; i<sizes ; i++)
    {
        printf("%s\n",strings[i].str);
        if(checkvariable(strings[i].str)!=0)
            replace(strings[i].str);
        evaAndStore(strings[i].str);
    }
    for(int i = 0 ; i<sizen ; i++ )
    {
        printf("\n%s = %s",heaps[i].key,heaps[i].value);
    }
    printf("\n");
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(st, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(st);
        char stcopy[100];
        strcpy(stcopy,st);
        checkerror(stcopy);
        char k[20],v[100];
        int flag=0;
        char*tk;
        tk=strtok(st,"=");
        while(tk)
        {
            if(flag==0)
                strcpy(k,tk);
            else
                strcpy(v,tk);
            flag++;
            tk=strtok(NULL,"=");
        }
        put(k,v);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    }
    Node *root=NULL;
    for(int j=0; j<sizen; j++)
    {
        root=insert(root,heaps[j].key,atof(heaps[j].value));
    }
    printf("\n****************************************\n");
    printf("Variables Before Sorting:\n");
    for(int i = 0 ; i<sizen ; i++ )
    {
        printf("\n%s = %s",heaps[i].key,heaps[i].value);
    }
    printf("\n****************************************\n");
    printf("Sorting By Key\n");
    inOrderTraverse(root);
    printf("\n****************************************\n");
    printf("Sorting By Value\n");
    sortingByHeap(sizen);
}
int main()
{
    printf("\a");
    system("color 5f");
    printf("\t\t\t\t\tWelcome to The Simple Interpreter\n\n");
    Run();
    return 0;
}
