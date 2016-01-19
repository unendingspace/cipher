#include <stdio.h>
#include <stdlib.h>
#include "fileio.c"

int max_input = 80;

struct command {
	char* text;
	void* function;
	char* description;
};

struct commandLib {
	struct command** list;
};

struct commandLib* commands;

struct command* setinput;
struct command* setoutput;
struct command* closeinput;
struct command* closeoutput;
struct command* help;

static char* input_array;

/*
help takes no arguments, and prints a help dialogue to the screen
help: Void -> Void
Effects: help dialogue is printed to the screen
*/
void displayHelp() {
	printf("\nAvailable commands at this time are:\n");
	struct command** traverse = (*commands).list;
	while (traverse) {
		printf("%s\t\t%s\n", (**traverse).text, (**traverse).description);
		traverse++;
	}
	printf("\n");	
}

/*
getInputArray takes no arguments, and returns a character array large
	enough to hold one more than max_input characters
getInputArray: Void -> Char*
*/
char* getInputArray() {
	return malloc(sizeof(char) * (max_input + 1));
}

/*
deleteInputArray takes a character array allocated on the heap, and 
	frees its associated memory
deleteInputArray: Char* -> Void
Requires: input_array is allocated on heap
Effects: memory holding input_array is freed
*/
void deleteInputArray(char* input_array) {
	free(input_array);
}

void parseInput() {
}

void topLevelPrompt() {
	int exit = 1;
	char input;
	int i = 0;
	int read_more;
	while (exit) {
		printf("<cipher>> ");
		read_more = scanf("%c", &input);
		while (i < max_input && input != '\n' && read_more != EOF) {
			input_array[i] = input;
			i++;
			read_more = scanf("%c", &input);
		}
		if (i == max_input && !(input == '\n' || read_more == EOF)) {
			printf("\nPlease keep input to under %i characters\n", max_input);
			printf("No action taken\n\n");
		}
		else {
			if (i != 80) {
				read_more[i] = NULL;
			}
			parseInput();
		}
		i = 0;
		read_more = 1;
		input = NULL;
	}
}

void exit() {
	if (input_array) {
		deleteInputArray();
	}
	closeOutput();
	closeInput();
}

void initCommands() {
	commands = malloc(sizeof(struct commandLib));
	
	setinput = malloc(sizeof(struct command));
	setoutput = malloc(sizeof(struct command));
	closeinput = malloc(sizeof(struct command));
	closeoutput = malloc(sizeof(struct command));
	help = malloc(sizeof(struct command));

	(*setinput).text = "setinput";
	(*setinput).function = &setInput;
	(*setinput).description = "Set or change the input file";

	(*setoutput).text = "setoutput";
	(*setoutput).function = &setOutput;
	(*setoutput).description = "Set or change the output file";

	(*closeinput).text = "closeinput";
	(*closeinput).function = &closeInput;
	(*closeinput).description = "Close the input file";

	(*closeoutput).text = "closeoutput";
	(*closeoutput).function = &closeOutput;
	(*closeoutput).description = "Close the output file";

	(*help).text = "help";
	(*help).function = &displayHelp;
	(*help).description = "Display the help dialogue";

	commands.list = malloc(sizeof(command*) * 6);
	commands.list[0] = setinput;
	commands.list[1] = setoutput;
	commands.list[2] = closeinput;
	commands.list[3] = closeoutput;
	commands.list[4] = help;
	
	input_array = getInputArray();
}

void entryPoint() {
	printf("\nWelcome to Cipher \nA command-line tool for accessing and utilizing various text ciphers.\n");
	printf("Please type a command to begin, or type 'help' to review the available options\n\n");
	initCommands();
}

int main() {
}
