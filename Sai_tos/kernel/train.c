#include <kernel.h>


WINDOW train1_wnd = {0, 0, 80, 8, 0, 0,' '};	// Dimensions for the train window. 
WINDOW switch_wnd = {32, 3, 23, 5, 0, 0,' '};	// Dimensions for the SWITCH CHANGE window. 
WINDOW speed_wnd = {57, 3, 23, 5, 0, 0,' '};	// Dimensions for the SPEED CHANGE window. 
WINDOW dest_wnd = {15, 3, 47, 5, 0, 0,' '};		// Dimensions for the DESTINATION (output) window. 
WINDOW headt_wnd = {0, 0, 80, 5, 0, 0, ' '};	// Dimensions for the upper window. 
WINDOW head1t_wnd = {29, 3, 0, 5, 0, 0, ' '};	// Dimensions for the Lines separating the Probing and Switch window. 
WINDOW head2t_wnd = {55, 3, 0, 5, 0, 0, ' '};	// Dimensions for the Lines separating the Switch and Speed window. 
WINDOW* wnd;

/**
 * This function creates a Train Process.
 */

 void init_train(WINDOW* wnd);

/**
 * This function initializes the outer loop switches.
 */

void set_init_switches(void);

/**
 * This function passes the commands to the COM port.
 * @param cmd is the command which has to be passed.
 * @param buff_len is the input buffer length.
 * @param in_buff stores the return value from the COM port.
 */

void onSend(char* cmd, int buff_len, char* in_buff);

/**
 * This function Concatenates the strings.
 * @param First string.
 * @param Second string.
 */

void concat(char* str1, char* str2);

/**
 * This function is used to probe the contact ID. Returns "1" if there's something on track or "0" if nothing.
 * @param contact_ID is the TRACK NUMBER.
 */

char Probe(char* contact_ID);


/**
 * Sets the speed for the Train.
 * @param speed stores the range of speed train between 0 to 5. 
 */

void set_speed(char* speed);

/**
 * This function sets the Switch to either R or G.
 * @param switch_num has the Switch Number.
 * @param dir is either "R" or "G".
 */

 void place_switch(char* switch_num, char* dir);

/**
* This function sets the Direction for the Train.
*/

void set_direction();

/**
* This function goes to the next line of execution only when the Probe returns 1.
*/

void return_probe();

/**
* This starts the train process.
*/

void train_process(PROCESS self, PARAM param);


	
	void concat(char *str1,char *str2)
	{
	    char *conc=str1;

	    while(*str1!='\0')
		str1++;

	    while(*str2!='\0')
	    {
		  *str1 = *str2 ;
		  str1++,str2++;
	    }

	    *str1='\0';
    
	}


	void onSend(char* cmd, int buff_len, char* in_buff)
	{
			
		COM_Message msg;
		char store_command[20];
	
		store_command[0] = '\0';
		concat(store_command, cmd);

		concat(store_command, "\015");		// Concatenating "Enter" character in the end of the string.
		msg.output_buffer = store_command;
		msg.len_input_buffer = buff_len;
		msg.input_buffer = in_buff; 
		send(com_port, &msg);				// Using TOS API send();
		
	}


	void place_switch(char* switch_num, char* dir) 
	{
		char instruct[10];
		char zero;
		instruct[0] = '\0';
		concat(instruct, "M");
		concat(instruct, switch_num);
		concat(instruct, dir);
		onSend(instruct, 0, &zero);
		wprintf(&switch_wnd, "Switch %s changed to %s\n", switch_num, dir );
	}



	void set_speed(char* speed)
	{
		char fast[10];
		char zero;
		fast[0] = '\0';
		concat(fast, "L20S");
		concat(fast, speed);
		onSend(fast, 0, &zero);
		wprintf(&speed_wnd, "Speed changed to %s\n", speed);
			

	}


	void set_direction()
	{
		char FB[10];  //forward or backward
		char zero;
		FB[0] = '\0';
		concat(FB, "L20D");
		onSend(FB, 0, &zero);
		wprintf(wnd, "Changing direction\n");

	}



	
	char Probe(char* contact_ID) 
	{
	
		char zero;
		char input[12];
		char command[3];
	    	
		onSend("R", 0, &zero);
		input[0] = '\0';
		concat(input, "C");
		concat(input, contact_ID);
		onSend(input, 3, command);
		wprintf(wnd, "Probing %s.........\n", contact_ID);
		sleep(10);
		return command[1];
							
	}

	void return_probe(char* contact_ID) 
	{
		int check_probe;
		char zero;
		char store;
		check_probe = 0;
		sleep(15);
		while(check_probe == 0) {
			   store = Probe(contact_ID);
				if(store == '1') {
				    
					break;
					}
				}
	}

	
	void first_config() 		// Configuration 1 and 2 without Zamboni.
	{			
		char zero;
		
		set_speed("4");
		place_switch("5", "R");
		sleep(5);
		return_probe("6");
		
		place_switch("4", "R");
		place_switch("3", "G");
	
		
		return_probe("1");
		
		set_speed("0");
		set_direction();
		set_speed("4");
		sleep(20);
		place_switch("5", "R");
		place_switch("6", "R");
		
		sleep(10);
		return_probe("8");
		set_speed("0");
		clear_window(&train1_wnd);
		wprintf(&dest_wnd, "CONGRATULATIONS YOU'RE ALIVE. WANNA RIDE  AGAIN  ;-)..\n");
		
	}


	
		void third_config() 			// Configuration 3 without Zamboni.
	{		
		char zero;
		int check_probe;
		
		
		set_speed("5");
			
		return_probe("7");
		set_speed("4");
		place_switch("5", "R");
		place_switch("6", "G");
	
		return_probe("12");
		set_speed("0");
		set_direction();
		place_switch("7", "R");
		set_speed("4");
		
		return_probe("13");
		set_speed("0");
		set_direction();
		set_speed("4");
		place_switch("8", "G");
		place_switch("4", "R");
		place_switch("3", "R");
		
		return_probe("5");
		
		set_speed("0");
		clear_window(&train1_wnd);
		wprintf(&dest_wnd, "CONGRATULATIONS YOU'RE ALIVE. WANNA RIDE  AGAIN  ;-)..\n");
			   
	
	}


		void fourth_config() 			// Configuration 4 without Zamboni.
	{			
		char zero;
		int check_probe;
		char store;
		int s = 85;
		check_probe = 0;
		
		set_speed("4");
		
		return_probe("6");
		set_speed("0");
		set_direction();
		place_switch("4", "G");
		place_switch("9", "G");
		set_speed("4");
		
		return_probe("14");
		
		set_speed("0");
		set_direction();
		set_speed("5");
		sleep(65);
			
		set_speed("0");
		set_direction();
		set_speed("4");
		
			while(Probe("16") != 0)			// This section checks till the Train catches the Carrier.
			{
					
				return_probe("14");
				set_speed("0");
				
				if(Probe("16") == '0')
				{
					break;
				}
				set_direction();
				set_speed("5");
				place_switch("9", "G");
				sleep(s +10);
				wprintf(&switch_wnd, "Trying to catch ASAP\n");
				return_probe("16");
				set_speed("0");
				set_direction();
				set_speed("4");
				s = s+2;
			}	

		set_speed("5");	
		place_switch("8", "G");
		sleep(10);
		place_switch("4", "R");
		place_switch("3", "R");
		sleep(20);
		return_probe("5");
		set_speed("0");
		clear_window(&train1_wnd);
		wprintf(&dest_wnd, "CONGRATULATIONS YOU'RE ALIVE. WANNA RIDE  AGAIN  ;-)..\n");
			   
	
	}
	

	void zamboni_config_13() 
	{
		char zero;
		
			if ( Probe("8") == '1' && Probe("2") == '1') 		//Configuration 1 with Zamboni
			{		

			wprintf(wnd, "Configuration 1 with Zamboni.\n");			
			sleep(5);
			return_probe("10");
			sleep(10);
			place_switch("9", "R");
			place_switch("1", "R");
			place_switch("2", "R");
			place_switch("7", "R");
			first_config();
			
			}

			else 
			{								//Configuration 3 with Zamboni
			
			wprintf(wnd, "Configuration 3 with Zamboni.\n");	
			sleep(10);
			return_probe("10");	
			set_speed("5");
			place_switch("5", "R");
			place_switch("6", "G");
			
			return_probe("7");
			set_speed("4");
			return_probe("6");
			place_switch("5", "G");
			
			return_probe("12");
			set_speed("0");
			set_direction();
			set_speed("4");
			place_switch("7", "R");	
			return_probe("11");
			set_speed("0");
			sleep(5);
			return_probe("4");
			set_speed("5");
			
			return_probe("3");
			place_switch("9", "R");
			place_switch("1", "R");
			place_switch("2", "R");
			place_switch("7", "R");
			set_speed("4");
			return_probe("6");
			set_speed("0");
			set_direction();
			set_speed("4");	
			place_switch("4", "R");
			place_switch("3", "R");
			return_probe("5");
			
			set_speed("0");
			clear_window(&train1_wnd);
			wprintf(&dest_wnd, "CONGRATULATIONS YOU'RE ALIVE. WANNA RIDE  AGAIN  ;-)..\n");
			
						
			}
			   
	
	}



	void zamboni_config_24() 
	{
			char zero;int s = 95;int i;
		if ( Probe("8") == '1' && Probe("2") == '1') 		//Configuration 2 with Zamboni
		{		
			wprintf(wnd, "Configuration 2 with Zamboni.\n");
			return_probe("4");			
			set_speed("4");
			place_switch("5", "R");
			place_switch("8", "R");
			sleep(5);
			return_probe("6");
			
			place_switch("4", "R");
			place_switch("3", "G");
	
			return_probe("1");
		
			set_speed("0");
			set_direction();
			set_speed("4");
			sleep(20);
			place_switch("5", "R");
			place_switch("6", "R");
			
			sleep(10);
			return_probe("8");
			set_speed("0");
			clear_window(&train1_wnd);
			wprintf(&dest_wnd, "CONGRATULATIONS YOU'RE ALIVE. WANNA RIDE  AGAIN  ;-)..\n");
			
					
		}

			else 
		{								
			return_probe("3");
			wprintf(wnd, "Configuration 4 with Zamboni.\n");	// Configuration 4 with Zamboni.
			set_speed("5");			
			return_probe("6");
			set_speed("0");
			set_direction();
			set_speed("5");
			place_switch("4", "G");
			return_probe("3");
			set_speed("0");
			return_probe("7");
			set_speed("5");
			return_probe("14");
			set_speed("0");
			place_switch("9", "G");
			set_direction();
			set_speed("5");
			sleep(140);
			return_probe("16");
			sleep(45);
			set_speed("0");
			set_direction();
			return_probe("7");
			set_speed("4");
			
			while(Probe("16") != 0) 	// This section checks till the Train catches the Carrier.
			{
					
				return_probe("14");
				set_speed("0");
				
				if(Probe("16") == '0')
				{
					break;
				}
				set_direction();
				set_speed("5");
				place_switch("9", "G");
				sleep(s + 10);
				wprintf(&switch_wnd, "Trying to catch ASAP\n");
				return_probe("16");
				
				set_speed("0");
				set_direction();
				return_probe("10");
				set_speed("4");
				s = s+3;
			}		
			set_speed("4");			
			place_switch("8", "G");
			sleep(10);
			return_probe("4");
			place_switch("4", "R");
			place_switch("3", "R");
			return_probe("15") ;
			place_switch("8", "R");
			return_probe("5");
			set_speed("0");
			
			place_switch("4", "G");
			
			clear_window(&train1_wnd);
			wprintf(&dest_wnd, "CONGRATULATIONS YOU'RE ALIVE. WANNA RIDE  AGAIN  ;-)..\n");
				   
	
		}
			
	}





	void set_init_switches()
	{
		wprintf(wnd, "Init switches\n");		
		place_switch("5", "G");
		place_switch("9", "R");
		place_switch("1", "G");
		place_switch("4", "G");
		place_switch("8", "G");
		
	}

	int check_train_config() 		// Detects the Configurations.
	{
		char zero; 
		int check_probe; char store;
		check_probe = 0;
		int i, j, k;
		
		
		for(i = 0; i <= 40; i++) 
		{
			sleep(20);
			if( Probe("4") == '1') 
			{
				for( j = 0; j <= 40; j++) 
				{
					if((Probe("6") == '1') || (Probe("7") == '1')) 
					{
						return 5;			// Zamboni's going in Clockwise direction.
					}
				
					else if((Probe("3") == '1') || (Probe("15") == '1')) 
					{
						return 	6;			// Zamboni's going in Anti-Clockwise direction.
				
					}
				}			
			}
		}

			if ( Probe("8") == '1' && Probe("2") == '1') 		// Configuration 1 & 2 without Zamboni.
			{	
					return 1;
			}

			else if ( Probe("5") == '1' && Probe("11") == '1') 	// Configuration 3 without Zamboni.
			{  
					return 3;
			}	
			
			else if ( Probe("5") == '1' && Probe("16") == '1') 	// Configuration 4 without Zamboni.
			{	
					return 4;
			}
		
	}


	void train_process(PROCESS self, PARAM param)
	{
		
		int layout;
		clear_window(&train1_wnd);
		wprintf(&train1_wnd, "|||       ||======  ===========  ''' //====||      //======||      ==== \n");
		wprintf(&train1_wnd, "|||       ||    	  ===========    ' ||           //             //    \\\n");
		wprintf(&train1_wnd, "|||       ||======      |||           ======\\   ||      ====    ||    ||\n");
		wprintf(&train1_wnd, "|||       ||            |||                      ||     ||  ||   \\    //\n");
		wprintf(&train1_wnd, "========= ||=======     |||           ||====||    \\====||  ||     ====\n");
		sleep(45);
		clear_window(&train1_wnd);
		clear_window(&headt_wnd);
		clear_window(&head1t_wnd);
		clear_window(&head2t_wnd);
		wprintf(&headt_wnd, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-:WELCOME:-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		wprintf(&headt_wnd, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		wprintf(&head1t_wnd, "|\n");						     wprintf(&head2t_wnd, "|\n");
		wprintf(&head1t_wnd, "|\n");						     wprintf(&head2t_wnd, "|\n");
		wprintf(&head1t_wnd, "|\n");						     wprintf(&head2t_wnd, "|\n");
		wprintf(&head1t_wnd, "|\n");						     wprintf(&head2t_wnd, "|\n");
		wprintf(&head1t_wnd, "|\n");						     wprintf(&head2t_wnd, "|\n");
		wprintf(&head1t_wnd, "|\n");						     wprintf(&head2t_wnd, "|\n");
		set_init_switches();	
		wnd = (WINDOW*) param;
		wprintf(wnd, "Running Train Application.\n");
		set_init_switches();
		
		layout = check_train_config();

		switch(layout) {
		case 1:	
			wprintf(wnd, "Configuration 1or2 (-)Zamboni.\n");
			first_config();
			break;
		case 3:	
			wprintf(wnd, "Configuration 3 (-) Zamboni.\n");
			third_config();
			break;
		case 4:	
			wprintf(wnd, "Configuration 4 (-) Zamboni.\n");
			fourth_config();
			break;
		case 5:
			
			zamboni_config_13();
			break;

		case 6:
			
			zamboni_config_24();
			break;

		default : wprintf(wnd, "Train in Danger.\n");
			break;
				}

		remove_ready_queue(active_proc);
		resign();
		
		
		 
	}


	void init_train(WINDOW* wnd)
	{
	
 		create_process(train_process, 4, (PARAM) wnd, "Train Process");
		clear_window(&train1_wnd);
		resign();

  	}
