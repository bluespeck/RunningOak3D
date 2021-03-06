#include "Log.h"
#include <cstdarg>
#include <cstdio>


namespace oakvr
{
	// --------------------------------------------------------------------------------
	void Log::Print(Log::LogLevel level, const char * logMsg, va_list args)
	{
		if (level == LogLevel::disabled || level > s_maxLevel)
			return;

		FILE * f = nullptr;
		if (level == LogLevel::error)
		{

			if (s_errFilename == "stderr")
				f = stderr;
			else
				fopen_s(&f, s_errFilename.c_str(), "at");
			if (!f)
				f = stderr;
		}
		else
		{

			if (s_outFilename == "stdout")
				f = stdout;
			else
				fopen_s(&f, s_outFilename.c_str(), "at");
			if (!f)
				f = stdout;
		}

		switch (level)
		{
		case LogLevel::info:
			fprintf(f, (std::string(s_logLabel) + " Info: ").c_str());
			break;
		case LogLevel::warning:
			fprintf(f, (std::string(s_logLabel) + " Warning: ").c_str());
			break;
		case LogLevel::error:
			fprintf(f, (std::string(s_logLabel) + " Error: ").c_str());
			break;
		dafault:
			break;
		}

		vfprintf(f, logMsg, args);
		fprintf(f, "\n");
		fflush(f);
		if (f != stderr && f != stdout)
			fclose(f);

	}
}

