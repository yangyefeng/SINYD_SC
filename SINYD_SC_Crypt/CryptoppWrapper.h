#pragma once
#include <string>

using namespace std;

string RSASignMessage(const string & PrivateKey, const string & message);
bool RSAVerifyMessage(const string & PublicKey, const string & message, const string & ciphertext);
string EncryptString(const char *instr, const char *passPhrase);
string DecryptString(const char * instr, const char * passPhrase);

