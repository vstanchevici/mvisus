#ifndef _APP_SETTINGS_H_
#define _APP_SETTINGS_H_

#include <string>
#include "optionparser.h"

namespace mv
{
	class AppSettings
	{
		private:
			struct Arg : public option::Arg
			{
				static void printError(const char* msg1, const option::Option& opt, const char* msg2)
				{
					fprintf(stderr, "%s", msg1);
					fwrite(opt.name, opt.namelen, 1, stderr);
					fprintf(stderr, "%s", msg2);
				}

				static option::ArgStatus Unknown(const option::Option& option, bool msg)
				{
					if (msg) printError("Unknown option '", option, "'\n");
					return option::ARG_ILLEGAL;
				}

				static option::ArgStatus Required(const option::Option& option, bool msg)
				{
					if (option.arg != 0)
						return option::ARG_OK;
					if (msg) printError("Option '", option, "' requires an argument\n");
					return option::ARG_ILLEGAL;
				}

				static option::ArgStatus NonEmpty(const option::Option& option, bool msg)
				{
					if (option.arg != 0 && option.arg[0] != 0)
						return option::ARG_OK;
					if (msg) printError("Option '", option, "' requires a non-empty argument\n");
					return option::ARG_ILLEGAL;
				}

				static option::ArgStatus Numeric(const option::Option& option, bool msg)
				{
					char* endptr = 0;
					if (option.arg != 0 && strtol(option.arg, &endptr, 10)) {};
					if (endptr != option.arg && *endptr == 0)
						return option::ARG_OK;
					if (msg) printError("Option '", option, "' requires a numeric argument\n");
					return option::ARG_ILLEGAL;
				}
			};

			enum  optionIndex { UNKNOWN, HELP, SYNC_PORT, CERT_PATH, CERT_KEY_PATH, CERT_CABUNDLE_PATH, API_HOST, API_PORT };

			static const option::Descriptor usage[];

		public:
			explicit AppSettings();
			~AppSettings();

			AppSettings(const AppSettings&) = delete;
			AppSettings& operator=(const AppSettings&) = delete;
			AppSettings(AppSettings&&) = default;
			AppSettings& operator=(AppSettings&&) = default;

		private:
			unsigned int mPort;  //port on which server will run
			unsigned int mApiHostPort; //port for api host
			std::string mCertificate, mCertificateKey, mCertificateBundle; //path to certificates
			std::string mLog, mApiHost;

		public:
			bool LoadFromEnvironmentVariables(int argc, char* argv[]);

			unsigned int GetPort() { return mPort; };
			unsigned int GetApiHostPort() { return mApiHostPort; };
			const std::string& GetApiHost() { return mApiHost; };
			std::string GetCertificateFile() { return mCertificate; };
			std::string GetCertificateKeyFile() { return mCertificateKey; };
			std::string GetCertificateBundleFile() { return mCertificateBundle; };
			std::string GetLogFile() { return mLog; };
	};
}

#endif