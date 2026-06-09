//YOUR CONTROLS:                                                                Run it: "apt install g++ geany libgmp-dev libssl-dev". Open the .cpp in Geany.
int digit_length = 200; //50000 max                                             //Append "-lgmp -lcrypto" to Geany's compile & build commands. Hit F9 once. F5 to run.
//digit_length is the length of a (jumping) value, near which
//prime gaps are found, for randomness. Bigger = more secure.



/*Version 4.0.0
Secure One-time pad for file encryption with a password. No password
is rejected upon decryption--giving false plausible output. */

#include <cstring>
#include <filesystem>
#include <fstream>
#include <gmp.h> //For primes
#include <iostream>
#include <openssl/evp.h> //For hash
using namespace std;
int main()
{	int raw_byte;
	char file_byte;
	ifstream in_stream;
	ofstream out_stream;
	
	cout << "\n(1) Encrypt"
	     << "\n(2) Decrypt"
	
	     << "\n\nOption: ";
	
	int user_option; cin >> user_option;
	if((user_option != 1) && (user_option != 2)) {cout << "\nInvalid.\n"; return 0;}
	
	//______________________________________________________Encrypt___________________________________________________//
	if(user_option == 1)
	{	//Gets path, fixes it if dropped.
		cout << "\nDrop/enter file:\n"; string path; getline(cin, path); if(path[0] == '\0') {getline(cin, path);}
		if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		in_stream.open(path); if(in_stream.fail()) {cout << "\nNo path " << path << "\n"; return 1;} in_stream.close();
		
		//Gets file size.
		long long input_file_size = filesystem::file_size(path);
		if(input_file_size == 0) {cout << "\nEmpty file.\n"; return 0;}
		
		//User enters password.
		cout << "Enter a unique password of any length:\n";
		string password; getline(cin, password); if(password[0] == '\0') {getline(cin, password);}
		if(password[0] == '\0') {cout << "\nNo password entered.\n"; return 0;}
		
		//Makes a key from the password. (Makes a prime of length digit_length).
		unsigned char hash[64]; EVP_Q_digest(nullptr, "SHA512", nullptr, password.c_str(), password.length(), hash, nullptr);
		string key; char symbols[] = "0123456789abcdef"; for(int a = 0; a < 64; a++) {key += symbols[hash[a] >> 4]; key += symbols[hash[a] & 0xf];}
		
		unsigned long long wanted_length = digit_length;
		for(; key.length() < wanted_length;) {key += key;}
		key.resize(digit_length);
		
		for(int a = 0; a < digit_length; a++)
		{	if(key[a] > 57) {key[a] -= 45;}
		}
		
		cout << "Encrypting...\n";
		char seeds[50001] = {'\0'}; strcpy(seeds, key.c_str());
		if(seeds[0] == '0') {seeds[0] = '5';}                                                                                        //Forces its length.
		mpz_t in, out; mpz_init(in); mpz_init(out); mpz_set_str(in, seeds, 10); mpz_nextprime(out, in); mpz_get_str(seeds, 10, out); //Makes it prime.
		mpz_t prime, two; mpz_init(prime); mpz_init(two); mpz_set_str(prime, seeds, 10); mpz_set_ui(two, 2);                         //Dedicates.
		
		//Generates randomness.
		in_stream.open(path);
		string output_file_name = path; output_file_name += "_(encrypted)";
		out_stream.open(output_file_name);
		for(long long gap = 2, a = 0; a < input_file_size;)
		{	mpz_add(prime, prime, two);
			int primality = mpz_probab_prime_p(prime, 25);
			if(primality == false) {gap += 2;}
			else
			{	mpz_get_str(seeds, 10, prime);
				gap *= 10; gap += seeds[digit_length - 3] - 48;
				gap *= 10; gap += seeds[digit_length - 2] - 48; gap %= 256;
				
				//Reads a byte.
				in_stream.get(file_byte); raw_byte = file_byte; if(raw_byte < 0) {raw_byte += 256;}
				
				//Encrypts byte.
				raw_byte += gap; raw_byte %= 256;
				
				//Writes byte.
				if(raw_byte < 128) {out_stream.put(raw_byte      );}
				else               {out_stream.put(raw_byte - 256);}
				
				gap = 2; a++; cout << a << " of " << input_file_size << " bytes...\n";
			}
		}
		in_stream.close();
		out_stream.close();
		
		cout << "\nEncrypted!\n";
	}
	
	//______________________________________________________Decrypt___________________________________________________//
	if(user_option == 2)
	{	//Gets path, fixes it if dropped.
		cout << "\nDrop/enter file:\n"; string path; getline(cin, path); if(path[0] == '\0') {getline(cin, path);}
		if(path[0] == '\'') {path.erase(0, 1); path.pop_back(); path.pop_back();}
		in_stream.open(path); if(in_stream.fail()) {cout << "\nNo path " << path << "\n"; return 1;} in_stream.close();
		
		//Gets file size.
		long long input_file_size = filesystem::file_size(path);
		if(input_file_size == 0) {cout << "\nEmpty file.\n"; return 0;}
		
		//User enters password.
		cout << "Password:\n";
		string password; getline(cin, password); if(password[0] == '\0') {getline(cin, password);}
		if(password[0] == '\0') {cout << "\nNo password entered.\n"; return 0;}
		
		//Makes a key from the password. (Makes a prime of length digit_length).
		unsigned char hash[64]; EVP_Q_digest(nullptr, "SHA512", nullptr, password.c_str(), password.length(), hash, nullptr);
		string key; char symbols[] = "0123456789abcdef"; for(int a = 0; a < 64; a++) {key += symbols[hash[a] >> 4]; key += symbols[hash[a] & 0xf];}
		
		unsigned long long wanted_length = digit_length;
		for(; key.length() < wanted_length;) {key += key;}
		key.resize(digit_length);
		
		for(int a = 0; a < digit_length; a++)
		{	if(key[a] > 57) {key[a] -= 45;}
		}
		
		cout << "Decrypting...\n";
		char seeds[50001] = {'\0'}; strcpy(seeds, key.c_str());
		if(seeds[0] == '0') {seeds[0] = '5';}                                                                                        //Forces its length.
		mpz_t in, out; mpz_init(in); mpz_init(out); mpz_set_str(in, seeds, 10); mpz_nextprime(out, in); mpz_get_str(seeds, 10, out); //Makes it prime.
		mpz_t prime, two; mpz_init(prime); mpz_init(two); mpz_set_str(prime, seeds, 10); mpz_set_ui(two, 2);                         //Dedicates.
		
		//Generates randomness.
		in_stream.open(path);
		string output_file_name = path; output_file_name += "_(decrypted)";
		out_stream.open(output_file_name);
		for(long long gap = 2, a = 0; a < input_file_size;)
		{	mpz_add(prime, prime, two);
			int primality = mpz_probab_prime_p(prime, 25);
			if(primality == false) {gap += 2;}
			else
			{	mpz_get_str(seeds, 10, prime);
				gap *= 10; gap += seeds[digit_length - 3] - 48;
				gap *= 10; gap += seeds[digit_length - 2] - 48; gap %= 256;
				
				//Reads a byte.
				in_stream.get(file_byte); raw_byte = file_byte; if(raw_byte < 0) {raw_byte += 256;}
				
				//Decrypts byte.
				/*   ______________________________________________ ________________________________________________
				    |                                              |                                                |
				    |          if sub-key <= cipherfile            |                     else                       |
				    |   then plainfile = (cipherfile - sub-key)    |    plainfile = ((256 - sub-key) + cipherfile)  |
				    |______________________________________________|________________________________________________|
				*/
				if(gap <= raw_byte) {raw_byte = (raw_byte - gap);        }
				else                {raw_byte = ((256 - gap) + raw_byte);}
				
				//Writes byte.
				if(raw_byte < 128) {out_stream.put(raw_byte      );}
				else               {out_stream.put(raw_byte - 256);}
				
				gap = 2; a++; cout << a << " of " << input_file_size << " bytes...\n";
			}
		}
		in_stream.close();
		out_stream.close();
		
		cout << "\nDecrypted!\n";
	}
}
