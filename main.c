#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node
{
    char uname[20];
	char info[6];
	int key;
	struct node *next;
	struct node *below;
}NODE;
NODE* search(NODE *first, char uname[20])
{
    NODE *temp;
    temp=first;
    if(first==NULL)
        return first;
    while(temp!=NULL)
    {
        if(strcmp(temp->uname,uname)==0)
        {
            return temp;
            break;
        }
        temp=temp->below;
    }
    return NULL;
}
NODE * encryptNode(NODE *enc,char info[6])
{
    NODE *newnode,*temp;
    newnode = (NODE*)malloc(sizeof(NODE));

    strcpy(newnode->info,info);
    newnode->next = NULL;

    if(enc == NULL)
        enc = newnode;
    else
    {
        temp = enc;
        while(temp->next!=NULL)
            temp = temp->next;
        temp->next = newnode;
    }

    return(enc);
}

NODE * insertNode(NODE *header,char data)
{
    NODE *newnode,*temp;
    newnode = (NODE*)malloc(sizeof(NODE));
    int c=data;

    newnode->next = NULL;
    newnode->below = NULL;
    newnode->key=(c*3+4)/10;
    for(int i=0;i<6;i++)
    {
        c=(c*3+4)%10;
        newnode->info[i]=c+'0';
        newnode->info[i]=(char)(newnode->info[i]+80);
    }
    if(header == NULL)
        header=newnode;
    else
    {
        temp = header;
        while(temp->next!=NULL)
            temp = temp->next;
        temp->next = newnode;
    }
    return(header);
 }
NODE* insertUser(NODE *first, char uname[20])
{
    NODE *header,*temp=first;
    header = (NODE*)malloc(sizeof(NODE));
    strcpy(header->uname,uname);
    header->below=NULL;
    header->next=NULL;
    printf("Enter the password: ");
    char password[10];int i;
    scanf("%s",password);
    for(i=0;password[i]!='\0';i++)
        header=insertNode(header,password[i]);
    if(first==NULL)
        first=header;
    else
    {
        temp=first;
        while(temp->below!=NULL)
            temp=temp->below;
        temp->below=header;
    }
    printf("\nUser successfully added");
    return first;
}
void signIn(NODE *user,char password[10])
{
    NODE *temp1=user->next;
    int i=0;
    while(temp1!=NULL && password[i]!='\0')
    {
        int c=(char)(temp1->info[0]-80)-'0';
        int value=(temp1->key * 10 + c -4)/3;
        char ch=(char)value;
        if(ch!=password[i])
        {
            printf("\nInvalid password");
            return;
        }
        i++;
        temp1=temp1->next;
    }
    if(temp1==NULL && password[i]=='\0')
        printf("\nSign-In successful");
    else
        printf("\nInvalid password");
}
void display(NODE *enc)
{
	if(enc==NULL)
	{
		printf(" empty");
		return;
	}
	printf("\nPassword: ");
	while(enc!=NULL)
	{
		printf("%s",enc->info);
		enc=enc->next;
	}
}
void encrypt(NODE *user)
{
    NODE *enc=NULL;
    printf("\nUsername: %s",user->uname);
    NODE *temp,*temp1,*min;
    temp=user;
    temp=temp->next;
    while(temp!=NULL)
    {
        enc=encryptNode(enc,temp->info);
        temp=temp->next;
    }
    int j;
    char info[6];
    for(temp=enc;temp!=NULL;temp=temp->next)
	{
	    min=temp;
	   for(temp1=temp;temp1!=NULL;temp1=temp1->next)
       {
           if(temp1->key<min->key)
           {
               min=temp1;
           }
       }
       j=temp->key;
       strcpy(info,temp->info);
       temp->key=min->key;
       strcpy(temp->info,min->info);
       min->key=j;
       strcpy(min->info,info);
	}
	display(enc);
}
void decrypt(NODE *user)
{
    NODE *temp=user->next;
    printf("\nPassword: ");
    while(temp!=NULL)
    {
        int c=(char)(temp->info[0]-80)-'0';
        int value=(temp->key * 10 + c -4)/3;
        printf("%c",(char)value);
        temp=temp->next;
    }
}
int main()
{
    NODE *first=NULL,*user;
    char uname[20];
    char password[10];
    int ch;
    while(1)
    {
    printf("\n\n1:Sign Up\n2:Sign in\n3:User Details\n4:Exit\nEnter your choice: ");
    scanf("%d",&ch);

    switch(ch)
    {
    case 1:printf("Enter the username: ");
            scanf("%s",uname);
            user=search(first,uname);
            if(user==NULL)
                first=insertUser(first,uname);
            else
                printf("Username already exists");
            break;
    case 2:printf("Enter the username: ");
            scanf("%s",uname);
            user=search(first,uname);
            if(user!=NULL)
            {
                printf("Enter the password: ");
                scanf("%s",password);
                signIn(user,password);
            }
            else
                printf("User not found");
            break;
    case 3:
        printf("Enter the username: ");
        scanf("%s",uname);
        user=search(first,uname);
        if(user!=NULL)
        {
            encrypt(user);
            decrypt(user);
        }
        else
            printf("User not found");
        break;
    case 4: exit(0);
        break;
    default:printf("\nInvalid choice");
    }
    }
    return 0;
}
