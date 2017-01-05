#include <kernel.h>

WINDOW head_wnd = {0, 0, 80, 5, 0, 0, ' '};		// Dimensions for the upper window. 
WINDOW head1_wnd = {29, 3, 0, 5, 0, 0, ' '};	// Dimensions for the window which shows the SWITCH CHANGE. 
WINDOW head2_wnd = {55, 3, 0, 5, 0, 0, ' '};	// Dimensions for the window which shows the SPEED CHANGE. 
WINDOW residue_wnd = {30, 3, 25, 6, 0, 0, ' '}; // Dimensions for the unused window. 
WINDOW pac_wnd = {53, 9, 23, 35, 45, 0, ' '};	// Dimensions for the pacman window. 
WINDOW shell_wnd = {0, 9, 51, 16, 0, 0, 0xDC};  // Dimensions for the shell window. 
WINDOW design_wnd = {0, 8, 80, 16, 0, 0, 0xDC}; // Dimensions for the design window. 
WINDOW train_wnd = {0, 4, 29, 4, 0, 0, ' '};	// Dimensions for the TRAIN OR PROBING window. 
WINDOW* twnd = &train_wnd;		// Pointer pointing to the Train window.
WINDOW* shell_ptr = &shell_wnd;		//Pointer pointing to the Shell window.

int length;
char input_buffer[20];


/**
 * This function Compares two strings and returns 0 if they're equal.
 * It also checks for the blank spaces before and after a command. Gives an invalidity if there's a space between words.
 * This fucntion takes the command similar to a Terminal in LINUX.
 */
int str_cmp(char *str1, char *str2);

/**
 * This function Clears the window .
 * @param wnd has the dimesions of the window.
 */
void clear_cmd(WINDOW *wnd);

/**
 * This function Checks for the validity of command and executes the respective task.
 */
void run_command();

/**
 * This function shows the commands supported by the shell. 
 */
void help();

/**
 * This function reads the input and checks for new line.
 */
void read_input();

/** 
 * This fucntion Cleans the input buffer.
 */
void clean_buffer();

/**
 * Makes the train go.
 */
void go();

/**
 * Makes the train stop.
 */
void stop();

/**
 * Makes the train go in reverse direction.
 */
void reverse();

/**
 * This function starts the Shell Process.
 * @param self is the shell process.
 * @param param is the parameters passed to this process.
 */
void shell_process (PROCESS self, PARAM param); 

/** 
 *This function Creates the shell process.
 */
void init_shell(); 



	
	int str_cmp(char* str1, char* str2)
	{

	    int equal = 0;
	    char* blank;
		int i = 0; 
		int k = 0;
		int sum = 0;
 
	  	while (str1[i] == ' ')		// Ignores and keeps count of all the spaces before the input.
		{
	   		 i++;
	
		}
			
	    while(str1[i] != '\0' && str2[k] != '\0') 		// Checking for a match and returns 0 for a match.
		{
					
			sum = sum + (str1[i] - str2[k]);
			i++;
			k++;
		}
			return sum;			// Sum stores either 0 or the difference between the two strings.
	}

  

	void clear_cmd(WINDOW *wnd)
	{
		clear_window(wnd);			// Calling the API clear_window.
	}
	

	void go()
	{
		char zero;
		onSend("L20S4", 0, &zero);		// Sending a speed of '4' to run the train.
	}


	void reverse()
	{
		char zero;
		onSend("L20S0", 0, &zero);		// Stopping the train.
		onSend("L20D", 0, &zero);		// Changing the direction of train.
		onSend("L20S4", 0, &zero);		// Sending a speed of '4' to run the train.
	}


	void stop()
	{
		char zero;
		onSend("L20S0", 0, &zero);		//Stopping the train.
	}

	
	void help()
	{
		wprintf(&shell_wnd, "** COMMANDS SUPPORTED BY TOS: **\n");
		wprintf(&shell_wnd, "- 'ps'       Prints out all the running processes.\n");
		wprintf(&shell_wnd, "- 'clear'    Clears the window buffer.\n");
		wprintf(&shell_wnd, "- 'help'     Commands supported by TOS.\n");
		wprintf(&shell_wnd, "- 'train'    Starts the train application.\n");
		wprintf(&shell_wnd, "- 'go'       Starts the train.\n");
		wprintf(&shell_wnd, "- 'stop'     Stops the train.\n");
		wprintf(&shell_wnd, "- 'reverse'  Runs the train in reverse direction.\n");
		wprintf(&shell_wnd, "- 'pacman'   Display's Pacman Game.\n");
	}


	void run_command()
	{

		char *cmd = &input_buffer;
			
			if (str_cmp(cmd, "clear") == 0) 
		   {
				clear_window(shell_ptr);
				wprintf(&shell_wnd, "TOS SHELL\n");

				wprintf(&shell_wnd, "=> ");
				read_input();
				clean_buffer();
				run_command();
			} 

			else if (str_cmp(cmd, "help") == 0) 
			{
				help();
			}

	 		else if (str_cmp(cmd, "ps") == 0)
			{
				print_all_processes(&shell_wnd);

			} 

			else if (str_cmp(cmd, "train") == 0) 
			{
				init_train(twnd);
			}

			else if (str_cmp(cmd, "\013") == 0) 
			{
				wprintf(&shell_wnd, "\n");
			}

			else if (str_cmp(cmd, "go") == 0) 
			{
				go();
			}

			else if (str_cmp(cmd, "stop") == 0) 
			{
				stop();
			}

			else if (str_cmp(cmd, "reverse") == 0) 
			{
				reverse();
			}

			else if (str_cmp(cmd, "pacman") == 0) 
			{
				init_pacman(&pac_wnd, 3);
			}

			else 
			{
				wprintf(&shell_wnd, "\nInvalid Command .Try 'help'.\n");
			}  
	}


	void read_input()
	{
		char ch; 
		int str_len;
		int i;
		Keyb_Message msg;
		length = 0;

		do {
			msg.key_buffer = &ch;
			send(keyb_port, &msg);

			if (ch == 13)
			{
				wprintf(&shell_wnd, "\n");
			}
			
			else if ( ch == '\b') 		// Implementing Backspace.
			{
				if( length != 0 ) 
				{
					input_buffer[length] = '\0';
					wprintf(&shell_wnd, "%c", ch);
					length--;
				}
			}
  
			else
			{
				input_buffer[length++] = ch;
				wprintf(&shell_wnd, "%c", ch);
			}

			} while (ch != '\n' && ch != '\r' && length < 20);
	
			input_buffer[length] = '\0';
			length = 0;
	}


	void clean_buffer()
	{
		length = 0;
		while(input_buffer[length++] != '\0')
		if (input_buffer[length] == '\n')
		input_buffer[length] = '\0';
	}


	void shell_process (PROCESS self, PARAM param)
	{

		clear_window(&shell_wnd);		// Clearing the shell window.
		clear_window(&train_wnd);		// Clearing the train window.
		clear_window(&head_wnd);		// Clearing the heading window.
		clear_window(&residue_wnd);		// Clearing the unused window.
		clear_window(&head1_wnd);		// Clearing the switch change window.
		clear_window(&head2_wnd);		// Clearing the speed change window.
		wprintf(&head_wnd, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-:WELCOME:-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		wprintf(&head_wnd, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		wprintf(&head1_wnd, "|\n");						     wprintf(&head2_wnd, "|\n");
		wprintf(&head1_wnd, "|\n");						     wprintf(&head2_wnd, "|\n");
		wprintf(&head1_wnd, "|\n");						     wprintf(&head2_wnd, "|\n");
		wprintf(&head1_wnd, "|\n");						     wprintf(&head2_wnd, "|\n");
		wprintf(&head1_wnd, "|\n");						     wprintf(&head2_wnd, "|\n");
		wprintf(&head1_wnd, "|\n");						     wprintf(&head2_wnd, "|\n");

		
		set_init_switches();
		wprintf(&design_wnd, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		wprintf(&shell_wnd, "TOS SHELL\n");
		

			while (1)
		{
			wprintf(&shell_wnd, "=> ");
			read_input();

			clean_buffer();
			run_command();

		}

	}


		void init_shell()
	{
		create_process(shell_process, 3, 0, "Shell Process");		// Assigning the shell process with priority 3.
		
	}

