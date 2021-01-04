#include "AppSettings.h"
#include <iostream>

namespace mv
{
	const option::Descriptor AppSettings::usage[] = {
	{ UNKNOWN, 0, "", "",   Arg::Unknown, "USAGE: server [options]\n\nOptions:" },

	{ HELP,    0, "", "help",    Arg::None,    "  \t--help  \tPrint usage and exit." },

	{ SYNC_PORT, 0, "", "sync_port", Arg::Numeric, "  \t--sync_port=<arg> \tMust have an numeric argument." },

	{ CERT_PATH, 0, "", "cert_path", Arg::Required, "  \t--cert_path=<arg> \tMust have an path argument." },

	{ CERT_KEY_PATH, 0, "", "cert_key_path", Arg::Required, "  \t--cert_key_path=<arg> \tMust have an path argument." },

	{ CERT_CABUNDLE_PATH, 0, "", "cert_cabundle_path", Arg::Required, "  \t--cert_cabundle_path=<arg> \tMust have an path argument." },

	{ API_HOST, 0, "", "api_host", Arg::Required, "  \t--api_host=<arg> \tMust have an url argument." },

	{ API_PORT, 0, "", "api_port", Arg::Numeric, "  \t--api_port=<arg> \tMust have an numeric argument." },

	{ UNKNOWN, 0, "", "",   Arg::None,
		"\nExamples:\n"
		"  sync-server --sync_port=443 --cert_path=certificate.crt --cert_key_path=certificate.key --cert_cabundle_path=ca_bundle.crt --api_host=api.com --api_port=443\n"
		"  sync-server --sync_port 80 --cert_path certificate.crt --cert_key_path certificate.key --cert_cabundle_path ca_bundle.crt --api_host api.dev.com --api_port 80\n"
	},
	{ 0, 0, 0, 0, 0, 0 } };

	AppSettings::AppSettings()
	{
		mPort = 8443;
		mCertificate = std::string();
		mCertificateKey = std::string();
		mCertificateBundle = std::string();
		mApiHost = std::string();
	}

	AppSettings::~AppSettings()
	{

	}

	bool AppSettings::LoadFromEnvironmentVariables(int argc, char* argv[])
	{
		argc -= (argc > 0); argv += (argc > 0); // skip program name argv[0] if present

		option::Stats stats(usage, argc, argv);

		#ifdef __GNUC__
			// GCC supports C99 VLAs for C++ with proper constructor calls.
			option::Option options[stats.options_max], buffer[stats.buffer_max];
		#else
			// use calloc() to allocate 0-initialized memory. It's not the same
			// as properly constructed elements, but good enough. Obviously in an
			// ordinary C++ program you'd use new[], but this file demonstrates that
			// TLMC++OP can be used without any dependency on the C++ standard library.
			option::Option* options = (option::Option*)calloc(stats.options_max, sizeof(option::Option));
			option::Option* buffer = (option::Option*)calloc(stats.buffer_max, sizeof(option::Option));
		#endif

		option::Parser parse(usage, argc, argv, options, buffer);

		if (parse.error())
			return false;

		if (options[HELP] || argc == 0)
		{
			int columns = 300;
			option::printUsage(fwrite, stdout, usage, columns);
			return false;
		}

		for (int i = 0; i < parse.optionsCount(); ++i)
		{
			option::Option& opt = buffer[i];

			//fprintf(stdout, "Argument #%d is ", i);

			switch (opt.index())
			{
				case HELP:
				{
					// not possible, because handled further above and exits the program
				}break;

				case UNKNOWN:
				{
					// not possible because Arg::Unknown returns ARG_ILLEGAL
					// which aborts the parse with an error
				}break;

				case SYNC_PORT:
				{
					//fprintf(stdout, "SYNC_PORT '%s'\n", opt.arg);
					mPort = std::atoi(opt.arg);
				}break;

				case CERT_PATH:
				{
					//fprintf(stdout, "CERT_PATH '%s'\n", opt.arg);
					mCertificate = opt.arg;
				}break;

				case CERT_KEY_PATH:
				{
					//fprintf(stdout, "CERT_KEY_PATH '%s'\n", opt.arg);
					mCertificateKey = opt.arg;
				}break;

				case CERT_CABUNDLE_PATH:
				{
					//fprintf(stdout, "CERT_CABUNDLE_PATH '%s'\n", opt.arg);
					mCertificateBundle = opt.arg;
				}break;

				case API_HOST:
				{
					//fprintf(stdout, "API_HOST '%s'\n", opt.arg);
					mApiHost = opt.arg;
				}break;

				case API_PORT:
				{
					//fprintf(stdout, "API_PORT '%s'\n", opt.arg);
					mApiHostPort = std::atoi(opt.arg);
				}break;
			}
		}

		return true;
	}

}