#include "plugin.h"
#include "tools.h"

#include <ctype.h>

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <typeinfo>

extern NPNetscapeFuncs* firefox;

NPError     NP_LOADDS	
NPP_New(NPMIMEType pluginType, NPP instance,
		uint16 mode, int16 argc, char* argn[],
		char* argv[], NPSavedData* saved){
//	printf("********* NPP_New  -- MIME: %s\n\n",pluginType);
	for(int i=0;i<argc;i++){
		
		if( ( strcmp(argn[i],"src") == 0 || strcmp(argn[i],"filename") == 0 ) && strcmp(pluginType,"application/x-mplayer2") == 0) {
			
			char* result=SendScriptToBrowser(instance,"document.location.href");
			if(result){
				char* fullUrl=getAbsoluteURL(argv[i],result);
				if(fullUrl){
					playWithVlc(fullUrl);
					free(fullUrl);
				}
				free(result);
			}
		}
	}
	return NPERR_NO_ERROR;
}
