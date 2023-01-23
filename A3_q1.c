// --------------------------------------------------------------

// Aaron Chin			
// 1185271
// CIS 2520				
// Assignment 3
// 2022-11-05

// --------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>

typedef struct tree
{
	struct tree* right;
	struct tree* left;
	char* value;
}Tree;


Tree* create(char* value)
{
	if (value == NULL)
		return NULL;
	Tree* new = malloc(sizeof(Tree));
	if (new == NULL)
		return NULL;
	//puts old variables into new ones, retures a new structure
	char* newVal = malloc(sizeof(char)*strlen(value)+1);
	strcpy(newVal, value);
	new->value = newVal;
	new->left = NULL;
	new->right = NULL;
	return new;
}


int isSymbol(char* line, int index)
{
	if (line[index] == '+' || line[index] == '-' || line[index] == '*' || line[index] == '/')
		return 1;
	return 0;
}


void exitWithError(char* msg)
{
	printf("%s\n", msg);
	exit(0);
}

void substring(char* dest, char* src, int start, int end)
{
	int size = end - start;
	if (size < 0)
		exitWithError("Invalid variable");
	int a = 0;
	for (int b = start; b <= end; b++)
	{
		dest[a] = src[b];
		a++;
	}
	dest[a]='\0';
}

void getLR (char* line, int* left, int* right)
{
	int start = *left, end = *right;
	*left = -1;
	*right = -1;
	int brk=0;
	//loops through this sectoin and finds left and right most bracket
	for (int a = start; a <= end; a++)
	{
		if (line[a] == '(' && *left == -1)
			*left = a;
		else if (line[a]=='(')
			brk++;
		if (line[a]==')' && brk > 0)
			brk--;
		else if (line[a]==')')
		{
			*right = a;
			break;
		}
	}
	if (*left+1==*right)
		exitWithError("Empty brackets ");
}


Tree* createNode(char* line, int left, int right)
{
	int symbol = -1;
	int brk=0;
	int start = left+1, end = right-1;
	for (int a = start; a <= end; a++)
	{
		if (isSymbol(line, a)==1 && brk == 0)
			symbol = a;
		if (line[a]=='(')
			brk++;
		else if (line[a]==')')
			brk--;
	}
	//if no symbol is found
	if (symbol == -1)
	{
		int l=left,r=right;
		getLR(line, &l, &r);
		//look for a sets of brackets
		if (l==-1 || r==-1)//if not stores in new node
		{
			char val[256];
			substring(val, line, left, right);
			return create(val);
		}
		//if it does prase
		return createNode(line, l+1, r-1);
	}
	//stores the symbol
	Tree* node = malloc(sizeof(Tree));
	char* newVal = malloc(sizeof(char)*2);
	newVal[0] = line[symbol];
	newVal[1] = '\0';
	node->value = newVal;

	//parse left
	node->left = createNode(line, left+1, symbol-1);
	//parse right
	node->right = createNode(line, symbol+1, right-1);
	return node;
}	

Tree* parseLine (char* line)
{	
	//assume first and last brackets
	int left=0,right=strlen(line);
	getLR(line, &left, &right);

	return createNode(line, left, right);
}

int height(Tree* node)
{
	if (node==NULL)
		return 0;
	if (node->left==NULL && node->right==NULL)
		return 1;
	//get height of the subtrees
	int l = height(node->left);
	int r = height(node->right);
	int max=0;
	//check which subtree is bigger
	if (l>r)
		max = l;
	else
		max = r;
	return max+1;
}


int checkBrackets (char* line)
{
	int a = 0;
	int brk = 0;
	//loop through for difference
	while(line[a]!='\0')
	{
		if (line[a]=='(')
			brk++;
		else if (line[a]==')')
			brk--;
		a++;
	}
	return brk;
}


void preOrder (Tree* head)
{
	if (head==NULL)
		return;
	//print the head left than the right
	printf("%s ", head->value);
	if (head->left != NULL)
		preOrder(head->left);
	if (head->right != NULL)
		preOrder(head->right);
}

void postOrder (Tree* head)
{
	if (head==NULL)
		return;
	//print the left right than the head
	if (head->left != NULL)
		postOrder(head->left);
	if (head->right != NULL)
		postOrder(head->right);
	printf("%s ", head->value);
}

int checkNum(char* string)
{
	if (string == NULL)
		return -1;
	//loop through and look for none digits
	for (int a = 0; a < strlen(string); a++)
		if (isdigit(string[a]) == 0 && string[a] != '.')
			return -1;
	return atoi(string);
}

void delTree (Tree* head)
{
	if (head == NULL)
		return;
	if (head->left!=NULL)
		delTree(head->left);
	if (head->right!=NULL)
		delTree(head->right);
	free(head->value);
	free(head);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		exitWithError("incorrect lines entered");

	//check == # of 
	if (checkBrackets(argv[1])!=0)
		exitWithError("Incorrect number of brackets");

	//read in equation
	Tree* head = parseLine(argv[1]);

	int input = 0;
	//command loop
	do
	{
		char userInput[256];
		printf("***************************\n"
				"Enter a number\n"
				"2. Preorder\n"
				"4. Postorder\n"
				"7. Exit\n"
				"***************************\n");
		fgets(userInput, 255, stdin);
		userInput[strlen(userInput)-1] = '\0';
		input = checkNum(userInput);

		switch (input)
		{
			//checks user input
			case 2:
				preOrder(head);
				printf("\n");
				break;
			case 4:
				postOrder(head);
				printf("\n");
				break;
			case 7:
				printf("Exiting\n");
				break;
			default:
				printf("Invalid option\n");
		}

	} while (input != 7);
	//frees memory
	delTree(head);
}
