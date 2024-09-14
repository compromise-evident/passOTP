/// passOTP - One-time pad for file encryption with password.                   Run it: "apt install g++ geany". Open the .cpp in Geany. Hit F9 once. F5 to run.


// Version 3.0.2   +   rolling-code 3.1.1
#include <fstream>
#include <iostream>
using namespace std;
int main()
{	ifstream in_stream;
	ofstream out_stream;
	
	cout << "\n(1) Encrypt"
	     << "\n(2) Decrypt"
	
	     << "\n\nOption: ";
	
	int user_option; cin >> user_option;
	if((user_option != 1) && (user_option != 2)) {cout << "\nInvalid.\n"; return 0;}
	
	//Gets path to file from user.
	cout << "Drop/enter file: ";
	
	//..........Gets path then fixes it if drag-n-dropped, regardless of single-quote presence and "enter"
	//..........not being cleared, meaning you can have options before this, where the user presses enter.
	char path_to_file[10000] = {'\0'};
	{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
		cin.getline(path_to_file, 10000);
		if(path_to_file[0] == '\0')
		{	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';}
			cin.getline(path_to_file, 10000);
		}
		if(path_to_file[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
		
		//..........Removes last space in path_to_file[].
		bool existence_of_last_space = false;
		for(int a = 9999; a > 0; a--)
		{	if(path_to_file[a] != '\0')
			{	if(path_to_file[a] == ' ') {path_to_file[a] = '\0'; existence_of_last_space = true;}
				break;
			}
		}
		
		//..........Removes encapsulating single-quotes in path_to_file[].
		bool existence_of_encapsulating_single_quotes = false;
		if(path_to_file[0] == '\'')
		{	for(int a = 0; a < 9999; a++)
			{	path_to_file[a] = path_to_file[a + 1];
				if(path_to_file[a] == '\0') 
				{	if(path_to_file[a - 1] != '\'') {cout << "\nBad path.\n"; return 0;}
					path_to_file[a - 1] = '\0';
					existence_of_encapsulating_single_quotes = true;
					break;
				}
			}
		}
		
		//..........Replaces all "'\''" with "'" in path_to_file[].
		int single_quote_quantity = 0;
		for(int a = 0; a < 10000; a++)
		{	if(path_to_file[a] == '\'') {single_quote_quantity++;}
		}
		
		if((single_quote_quantity                     >    0)
		&& (existence_of_last_space                  == true)
		&& (existence_of_encapsulating_single_quotes == true))
		{	if((single_quote_quantity % 3) != 0) {cout << "\nBad path.\n"; return 0;}
			
			for(int a = 0; a < 9997; a++)
			{	if(path_to_file[a] == '\'')
				{	int temp = (a + 1);
					for(; temp < 9997; temp++)
					{	path_to_file[temp] = path_to_file[temp + 3];
					}
				}
			}
		}
	}
	
	//Checks if file exists (failure can be bad path info as well.)
	in_stream.open(path_to_file);
	if(in_stream.fail() == true) {in_stream.close(); cout << "\n\nNo such file or directory.\n";             return 0;}
	char sniffed_one_file_character;
	in_stream.get(sniffed_one_file_character);
	if(in_stream.eof() == true) {in_stream.close();  cout << "\n\nNothing to process, the file is empty.\n"; return 0;}
	in_stream.close();
	
	//Gets location of the first encountered end-null coming from the left in path_to_file[].
	int path_to_file_null_bookmark;
	for(int a = 0; a < 10000; a++) {if(path_to_file[a] == '\0') {path_to_file_null_bookmark = a; break;}}
	
	//Gets a copy of path_to_file[] in case overwriting raw file after encryption.
	char path_to_file_copy_if_overwriting_raw[10000];
	for(int a = 0; a < 10000; a++) {path_to_file_copy_if_overwriting_raw[a] = path_to_file[a];}
	long long file_size_for_overwriting = 0;
	
	//Gets password from user, to be built into actual_key[].
	if(user_option == 1) {cout << "\nPassword, 1k char max: ";}
	else                 {cout << "\nDecryption password: "  ;}
	
	system("stty -echo"); //..........Renders keyboard input invisible.
	char password[1001] = {'\0'};
	cin.getline(password, 1001);
	password[1000] = '\0';
	system("stty echo"); //..........Restores keyboard input visibility.
	if(password[0] == '\0') {cout << "\nNothing entered.\n"; return 0;}
	
	//Improves password.
	unsigned int quick_seed_for_improving_password = password[0];
	srand(quick_seed_for_improving_password);
	for(int a = 0; a < 1000; a++) {password[a] = ((password[a] + rand()) % 10);}
	
	//Makes actual_seeds[] from password.
	long long temp_overflow_for_randomness;
	unsigned int actual_seeds[100] = {0}; //..........Makes 100 10-digit actual seeds based on password[]. (Strings together 10 contiguous digits, 100 times.)
	int password_read_bookmark = 0;
	for(int a = 0; a < 100; a++)
	{	temp_overflow_for_randomness = 0;
		for(int b = 0; b < 10; b++)
		{	temp_overflow_for_randomness += password[password_read_bookmark];
			if(b < 9) {temp_overflow_for_randomness *= 10;}
			password_read_bookmark++;
		}
		
		actual_seeds[a] = (temp_overflow_for_randomness % 4294967296);
	}
	
	int          actual_key[1000] = {0}; //..........Becomes actual key based on password (filled with randomness 0 - 255, renewed for each kB of the file.)
	unsigned int randomness[1000] = {0};
	
	//Gets file size in MB for displaying "x of y MB...".
	in_stream.open(path_to_file);
	char garbage_byte;
	long long total_bytes = 0;
	in_stream.get(garbage_byte);
	for(; in_stream.eof() == false;)
	{	in_stream.get(garbage_byte);
		total_bytes++;
	}
	in_stream.close();
	
	long long total_MB = (total_bytes - (total_bytes % 1000000));
	total_MB /= 1000000;
	cout << "\n";
	
	
	
	
	
	//______________________________________________________Encrypt___________________________________________________//
	if(user_option == 1)
	{	//Prepares 2 file streams.
		in_stream.open(path_to_file);
		path_to_file[path_to_file_null_bookmark    ] = '.'; //..........Appends ".pass" to file name.
		path_to_file[path_to_file_null_bookmark + 1] = 'p';
		path_to_file[path_to_file_null_bookmark + 2] = 'a';
		path_to_file[path_to_file_null_bookmark + 3] = 's';
		path_to_file[path_to_file_null_bookmark + 4] = 's';
		out_stream.open(path_to_file);
		
		//Encrypts.
		char garbage_byte;
		int  garbage_byte_normal;
		int  one_thousand_counter = 0;
		int  one_million_counter  = 0;
		in_stream.get(garbage_byte);
		for(long long MB_done = 0; in_stream.eof() == false;)
		{	//..........Makes new actual key.
			if(one_thousand_counter == 0)
			{	
				
				
				
				
				//..........Fills int actual_key[1000] with new randomness (0 to 255.)
				//..........(Exactly the same for encrypt & decrypt.) Each kB of file consumes entire actual_key[].
				//..........The following block verbatim from rolling-code.cpp, except for absurd_protection_against_cryptanalysis & extracting rand.
				
				//..........Generator powerhouse.
				for(int b = 0; b < 100; b++)
				{	srand(actual_seeds[b]);
					for(int c = 0; c < 1000; c++) {randomness[c] += rand(); randomness[c] %= 256;} //..........Fills randomness[] (100x per 1,000-char code.)
					temp_overflow_for_randomness = (actual_seeds[99 - b] + rand()); //..........Modifies inverse actual_seeds[].
					actual_seeds[99 - b] = (temp_overflow_for_randomness % 4294967296);
					
					srand(actual_seeds[99 - b]);  //..........Now using that inverse seed.
					for(int c = 0; c < 1000; c++) //..........Swaps EACH & EVERY element in randomness[] with randomly chosen (100,000x per 1,000-char code.)
					{	int random_element = (rand() % 1000);
						for(; random_element == c;) {random_element = (rand() % 1000);}
						
						unsigned int temp_element = randomness[random_element];
						randomness[random_element] = randomness[c];
						randomness[c] = temp_element;
					}
					temp_overflow_for_randomness = (actual_seeds[b] + rand()); //..........Modifies current actual_seeds[].
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				//..........Makes 100 10-digit new actual seeds based on randomness[]. (!!! Adds to current actual_seeds. !!! The generated Code is NOT alone responsible for new actual_seeds. !!!) (Strings together 10 contiguous digits, 100 times.)
				int randomness_read_bookmark = 0;
				for(int b = 0; b < 100; b++)
				{	temp_overflow_for_randomness = 0;
					for(int c = 0; c < 10; c++)
					{	temp_overflow_for_randomness += (randomness[randomness_read_bookmark] % 10);
						if(c < 9) {temp_overflow_for_randomness *= 10;}
						randomness_read_bookmark++;
					}
					
					temp_overflow_for_randomness += actual_seeds[b];
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				for(int b = 0; b < 1000; b++) {actual_key[b] = randomness[b];}
				
				
				
				
				
			}
			
			//..........Makes byte normal.
			garbage_byte_normal = garbage_byte;
			if(garbage_byte_normal < 0) {garbage_byte_normal += 256;}
			
			//..........Encrypts byte.
			garbage_byte_normal += actual_key[one_thousand_counter];
			garbage_byte_normal %= 256;
			
			//..........Writes byte to file.
			if(garbage_byte_normal < 128) {out_stream.put(garbage_byte_normal      );}
			else                          {out_stream.put(garbage_byte_normal - 256);}
			
			file_size_for_overwriting++;
			one_thousand_counter++;
			one_million_counter++;
			if(one_thousand_counter ==    1000) {one_thousand_counter = 0                                                                ;}
			if(one_million_counter  == 1000000) {one_million_counter  = 0; MB_done++; cout << MB_done << " of " << total_MB << " MB...\n";}
			in_stream.get(garbage_byte);
		}
		in_stream.close();
		out_stream.close();
	}
	
	
	
	
	
	//______________________________________________________Decrypt___________________________________________________//
	if(user_option == 2)
	{	//Prepares two file streams.
		in_stream.open(path_to_file);
		path_to_file[path_to_file_null_bookmark     ] = ' '; //..........Appends " (decrypted)" to file name.
		path_to_file[path_to_file_null_bookmark +  1] = '(';
		path_to_file[path_to_file_null_bookmark +  2] = 'd';
		path_to_file[path_to_file_null_bookmark +  3] = 'e';
		path_to_file[path_to_file_null_bookmark +  4] = 'c';
		path_to_file[path_to_file_null_bookmark +  5] = 'r';
		path_to_file[path_to_file_null_bookmark +  6] = 'y';
		path_to_file[path_to_file_null_bookmark +  7] = 'p';
		path_to_file[path_to_file_null_bookmark +  8] = 't';
		path_to_file[path_to_file_null_bookmark +  9] = 'e';
		path_to_file[path_to_file_null_bookmark + 10] = 'd';
		path_to_file[path_to_file_null_bookmark + 11] = ')';
		out_stream.open(path_to_file);
		
		//Decrypts.
		char garbage_byte;
		int  garbage_byte_normal;
		int  one_thousand_counter = 0;
		int  one_million_counter  = 0;
		in_stream.get(garbage_byte);
		for(long long MB_done = 0; in_stream.eof() == false;)
		{	//..........Makes new actual key.
			if(one_thousand_counter == 0)
			{	
				
				
				
				
				//..........Fills int actual_key[1000] with new randomness (0 to 255.)
				//..........(Exactly the same for encrypt & decrypt.) Each kB of file consumes entire actual_key[].
				//..........The following block verbatim from rolling-code.cpp, except for absurd_protection_against_cryptanalysis & extracting rand.
				
				//..........Generator powerhouse.
				for(int b = 0; b < 100; b++)
				{	srand(actual_seeds[b]);
					for(int c = 0; c < 1000; c++) {randomness[c] += rand(); randomness[c] %= 256;} //..........Fills randomness[] (100x per 1,000-char code.)
					temp_overflow_for_randomness = (actual_seeds[99 - b] + rand()); //..........Modifies inverse actual_seeds[].
					actual_seeds[99 - b] = (temp_overflow_for_randomness % 4294967296);
					
					srand(actual_seeds[99 - b]);  //..........Now using that inverse seed.
					for(int c = 0; c < 1000; c++) //..........Swaps EACH & EVERY element in randomness[] with randomly chosen (100,000x per 1,000-char code.)
					{	int random_element = (rand() % 1000);
						for(; random_element == c;) {random_element = (rand() % 1000);}
						
						unsigned int temp_element = randomness[random_element];
						randomness[random_element] = randomness[c];
						randomness[c] = temp_element;
					}
					temp_overflow_for_randomness = (actual_seeds[b] + rand()); //..........Modifies current actual_seeds[].
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				//..........Makes 100 10-digit new actual seeds based on randomness[]. (!!! Adds to current actual_seeds. !!! The generated Code is NOT alone responsible for new actual_seeds. !!!) (Strings together 10 contiguous digits, 100 times.)
				int randomness_read_bookmark = 0;
				for(int b = 0; b < 100; b++)
				{	temp_overflow_for_randomness = 0;
					for(int c = 0; c < 10; c++)
					{	temp_overflow_for_randomness += (randomness[randomness_read_bookmark] % 10);
						if(c < 9) {temp_overflow_for_randomness *= 10;}
						randomness_read_bookmark++;
					}
					
					temp_overflow_for_randomness += actual_seeds[b];
					actual_seeds[b] = (temp_overflow_for_randomness % 4294967296);
				}
				
				for(int b = 0; b < 1000; b++) {actual_key[b] = randomness[b];}
				
				
				
				
				
			}
			
			//..........Makes byte normal.
			garbage_byte_normal = garbage_byte;
			if(garbage_byte_normal < 0) {garbage_byte_normal += 256;}
			
			//..........Decrypts byte.
			/*   ______________________________________________ ________________________________________________
			    |                                              |                                                |
			    |          if sub-key <= cipherfile            |                     else                       |
			    |   then plainfile = (cipherfile - sub-key)    |    plainfile = ((256 - sub-key) + cipherfile)  |
			    |______________________________________________|________________________________________________|
			*/
			if(actual_key[one_thousand_counter] <= garbage_byte_normal)
			{	garbage_byte_normal = (garbage_byte_normal - actual_key[one_thousand_counter]);
			}
			else
			{	garbage_byte_normal = ((256 - actual_key[one_thousand_counter]) + garbage_byte_normal);
			}
			
			//..........Writes byte to file.
			if(garbage_byte_normal < 128) {out_stream.put(garbage_byte_normal      );}
			else                            {out_stream.put(garbage_byte_normal - 256);}
			
			file_size_for_overwriting++;
			one_thousand_counter++;
			one_million_counter++;
			if(one_thousand_counter ==    1000) {one_thousand_counter = 0                                                                ;}
			if(one_million_counter  == 1000000) {one_million_counter  = 0; MB_done++; cout << MB_done << " of " << total_MB << " MB...\n";}
			in_stream.get(garbage_byte);
		}
		in_stream.close();
		out_stream.close();
	}
	
	
	
	
	
	//Overwrites things (some are partially effective--printed/copied text is cloned from RAM to RAM, flash storage cannot be easily overwritten.)
	//..........Overwrites RAM of variable char sniffed_one_file_character.
	sniffed_one_file_character = '\0'; sniffed_one_file_character = -1;   //Binary: 00000000, 111111111.
	
	//..........Overwrites RAM of variable int path_to_file_null_bookmark.
	path_to_file_null_bookmark = 0; path_to_file_null_bookmark = -2147483648; path_to_file_null_bookmark = 2147483647;
	
	//..........Overwrites RAM of array char password[1001].
	for(int a = 0; a < 1001; a++) {password[a] = '\0'; password[a] = -1;} //Binary: 00000000, 111111111.
	
	//..........Overwrites RAM of variable unsigned int quick_seed_for_improving_password.
	quick_seed_for_improving_password = 0; quick_seed_for_improving_password = 4294967295;
	
	//..........Overwrites RAM of variable long long temp_overflow_for_randomness.
	temp_overflow_for_randomness = 0; temp_overflow_for_randomness = -9223372036854775807; temp_overflow_for_randomness = 9223372036854775807;
	
	//..........Overwrites RAM of array unsigned int actual_seeds[100].
	for(int a = 0; a < 100; a++) {actual_seeds[a] = 0; actual_seeds[a] = 4294967295;}
	
	//..........Overwrites RAM of array int actual_key[1000].
	for(int a = 0; a < 1000; a++) {actual_key[a] = 0; actual_key[a] = -2147483648; actual_key[a] = 2147483647;}
	
	//..........Overwrites RAM of array unsigned int randomness[1000].
	for(int a = 0; a < 1000; a++) {randomness[a] = 0; randomness[a] = 4294967295;}
	
	//..........Asks to overwrite the plainfile file after encryption.
	if(user_option == 1)
	{	cout << "Done! Overwrite plainfile? y/n: ";
		char wait; cin >> wait;
		
		if(wait == 'y')
		{	out_stream.open(path_to_file_copy_if_overwriting_raw);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000.
			out_stream.open(path_to_file_copy_if_overwriting_raw);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111.
			
			cout << "Overwrite finished.\n";
		}
		else {cout << "File unharmed.\n";}
	}
	
	//..........Asks to overwrite the plainfile file after decryption, giving the user a moment to observe or transfer it first.
	if(user_option == 2)
	{	cout << "Done! Now is your chance to observe or copy the plainfile\n"
		     << "onto external devices so that it can be heavily\n"
		     << "overwritten here. Continue? y/n: ";
		char wait; cin >> wait;
		
		if(wait == 'y') //..........Note the difference here--original (non-copy) path_to_file[] that was appended to in decrypt.
		{	out_stream.open(path_to_file);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000.
			out_stream.open(path_to_file);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111.
			
			cout << "Overwrite finished.\n";
		}
		else {cout << "File unharmed.\n";}
	}
	
	//..........Overwrites RAM of variable long long file_size_for_overwriting.
	file_size_for_overwriting = 0; file_size_for_overwriting = -9223372036854775807; file_size_for_overwriting = 9223372036854775807;
	
	//..........Overwrites RAM of array char path_to_file[10000]
	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0'; path_to_file[a] = -1;}           //Binary: 00000000, 111111111.
	
	//..........Overwrites RAM of array char path_to_file_copy_if_overwriting_raw[10000];    //Binary: 00000000, 111111111.
	for(int a = 0; a < 10000; a++) {path_to_file_copy_if_overwriting_raw[a] = '\0'; path_to_file_copy_if_overwriting_raw[a] = -1;}
}
