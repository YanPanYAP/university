#pragma once

#define PARM_IN L"-in:"
#define PARM_OUT L"-out:"
#define PARM_LOG L"-log:"
#define PARM_IT L"-IT:"
#define PARM_LT L"-LT:"
#define PARM_GREIBACH L"-greibach:"
#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT_EXT L".out"
#define PARM_LOG_DEFAULT_EXT L".txt"
#define PARM_IT_DEFAULT_EXT L".txt"
#define PARM_LT_DEFAULT_EXT L".txt"
#define PARM_PARS_DEFAULT_EXT L".txt"

namespace Parm
{
	struct PARM	
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
		wchar_t IT[PARM_MAX_SIZE];
		wchar_t LT[PARM_MAX_SIZE];
		wchar_t greibach[PARM_MAX_SIZE];
	};
	PARM getparm(int argc, wchar_t* argv[]); //int argc, _TCHAR* argv[] 
}
