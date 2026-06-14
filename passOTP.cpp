//YOUR CONTROLS:                                                                Run it: "apt install g++ geany libgmp-dev". Open the .cpp in Geany.
int digit_length = 200; //50000 max                                             //Append "-lgmp" to Geany's compile & build commands. Hit F9 once. F5 to run.
//digit_length is the length of a (jumping) value, near which
//prime gaps are found, for randomness. Bigger = more secure.



/*Version 4.0.1
Secure One-time pad for file encryption with a password. No password
is rejected upon decryption--giving false plausible output. */

#include <cstring>
#include <filesystem>
#include <fstream>
#include <gmp.h> //For primes
#include <iostream>
#include <string>
using namespace std;
int main()
{	int raw_byte;
	char file_byte;
	ifstream in_stream;
	ofstream out_stream;
	
	std::string SHA_512_input;
	std::string SHA_512_output;
	
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
		SHA_512_input = password;
		{SHA_512_output = ""; std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
		string key = SHA_512_output;
		
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
		SHA_512_input = password;
		{SHA_512_output = ""; std::string msg = SHA_512_input; unsigned long long H[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL}; const unsigned long long K[80] = {0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL, 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL}; unsigned long long orig_len_bits = msg.length() * 8ULL; msg.push_back((char)0x80); for(; (msg.length() % 128) != 112;) {msg.push_back((char)0x00);} msg.append(8, (char)0x00); for(int a = 0; a < 8; a++) {msg.push_back((char)((orig_len_bits >> (56 - a * 8)) & 0xFF));} for(unsigned long long offset = 0; offset < msg.length(); offset += 128) {unsigned long long W[80]; for(int i = 0; i < 16; i++) {unsigned long long p = offset + (i * 8); W[i] = ((unsigned long long)(unsigned char)msg[p + 0] << 56) | ((unsigned long long)(unsigned char)msg[p + 1] << 48) | ((unsigned long long)(unsigned char)msg[p + 2] << 40) | ((unsigned long long)(unsigned char)msg[p + 3] << 32) | ((unsigned long long)(unsigned char)msg[p + 4] << 24) | ((unsigned long long)(unsigned char)msg[p + 5] << 16) | ((unsigned long long)(unsigned char)msg[p + 6] <<  8) | ((unsigned long long)(unsigned char)msg[p + 7]);} for(int i = 16; i < 80; i++) {unsigned long long x0 = W[i - 15]; unsigned long long s0 = ((x0 >> 1) | (x0 << 63)) ^ ((x0 >> 8) | (x0 << 56)) ^ (x0 >> 7); unsigned long long x1 = W[i - 2]; unsigned long long s1 = ((x1 >> 19) | (x1 << 45)) ^ ((x1 >> 61) | (x1 << 3)) ^ (x1 >> 6); W[i] = s1 + W[i - 7] + s0 + W[i - 16];} unsigned long long a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7]; for(int i = 0; i < 80; i++) {unsigned long long bs1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23)); unsigned long long ch  = (e & f) ^ (~e & g); unsigned long long T1  = h + bs1 + ch + K[i] + W[i]; unsigned long long bs0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25)); unsigned long long maj = (a & b) ^ (a & c) ^ (b & c); unsigned long long T2  = bs0 + maj; h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;} H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;} const char symbols[] = "0123456789abcdef"; for(int a = 0; a < 8; a++) {for(int b = 15; b >= 0; b--) {int nibble = (H[a] >> (b * 4)) & 0xF; SHA_512_output += symbols[nibble];}}}
		string key = SHA_512_output;
		
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
