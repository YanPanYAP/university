#pragma once
#include "stdafx.h"

namespace Out
{
	struct OUT
	{
		wchar_t outfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};

	static const OUT INITOUT{ L"", NULL };
	OUT getout(wchar_t outfile[]);
	void WriteOut(In::IN in, wchar_t out[]);
	void WriteError(OUT out, Error::ERROR error);
	void Close(OUT out);
};

