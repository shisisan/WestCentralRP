// Include guard
#if defined _INC_components_init
	#endinput
#endif

#define _INC_components_init

// Configuration
#define PP_SYNTAX_AWAIT
#define PP_SYNTAX_GENERIC
#define YSI_NO_KEYWORD_List
#define YSI_YES_HEAP_MALLOC
#define YSI_NO_MODE_CACHE
#define CGEN_MEMORY 30000

#define task_unblock(%0) \
	task_detach(); \
	task_yield(%0)

#define function%0(%1) 			forward%0(%1); public%0(%1)

// Module Include
#include <utilities>
#include <components\database>
#include <components\account>