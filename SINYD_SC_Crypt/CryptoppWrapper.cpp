#include "CryptoppWrapper.h"

#define CRYPTOPP_DEFAULT_NO_DLL
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "utils.h"
#include "cryptopp\hex.h"
#include "cryptopp\rsa.h"
#include "cryptopp\osrng.h"
#include "cryptopp\config.h"
#include "cryptopp\modes.h"
#include "cryptopp\default.h"

USING_NAMESPACE(CryptoPP)
USING_NAMESPACE(std)

const int MAX_PHRASE_LENGTH = 250;

// Aggressive stack checking with VS2005 SP1 and above.
#if (CRYPTOPP_MSC_VERSION >= 1410)
# pragma strict_gs_check (on)
#endif

#if _DEBUG
#pragma comment(lib, "../Lib/cryptlib563D.lib")
#else
#pragma comment(lib, "../Lib/cryptlib563.lib")
#endif

//int(*AdhocTest)(int argc, char *argv[]) = NULL;

string RSASignMessage(const string& PrivateKey, const string& message)
{
	StringSource privStr(PrivateKey, true, new HexDecoder);
	RSASS<PKCS1v15, SHA>::Signer signer(privStr);

	// Pseudo Random Number Generator
	AutoSeededRandomPool rng;
	// Generate Parameters
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, PrivateKey.size());

	byte buf[1300] = { 0 };
	size_t len = signer.SignMessage(rng, (const byte*)message.c_str(), message.size(), buf);

	string res = utils::ToHexString(buf, len);
	return res;
}

bool RSAVerifyMessage(const string& PublicKey, const string& message, const string& ciphertext)
{
	StringSource pubStr(PublicKey, true, new HexDecoder);
	RSASS<PKCS1v15, SHA>::Verifier verifier(pubStr);

	std::vector<char> rawSignature;
	utils::FromHexString(ciphertext, &rawSignature);

	return verifier.VerifyMessage((const byte*)message.c_str(),
		message.size(), (const byte*)utils::GetBeginOf(rawSignature), rawSignature.size());
}

string EncryptString(const char *instr, const char *passPhrase)
{
	string outstr;

	DefaultEncryptorWithMAC encryptor(passPhrase, new HexEncoder(new StringSink(outstr)));
	encryptor.Put((byte *)instr, strlen(instr));
	encryptor.MessageEnd();

	return outstr;
}

string DecryptString(const char *instr, const char *passPhrase)
{
	string outstr;

	HexDecoder decryptor(new DefaultDecryptorWithMAC(passPhrase, new StringSink(outstr)));
	decryptor.Put((byte *)instr, strlen(instr));
	decryptor.MessageEnd();

	return outstr;
}
