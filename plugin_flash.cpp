#include "plugin.h"
#include "tools.h"

#include <stdio.h>
#include <malloc.h>
#include <memory.h>

#include <String.h>


NPError     NP_LOADDS	
NPP_New(NPMIMEType pluginType, NPP instance,
		uint16 mode, int16 argc, char* argn[],
		char* argv[], NPSavedData* saved){

//	printf("********* NPP_New  -- MIME: %s\n\n",pluginType);

	//getting the domain:
	
	char* domain=SendScriptToBrowser(instance,"document.domain");	
	
	printf("** domain: [%s]\n\n",domain);

			

	for(int i=0;i<argc;i++){
	
	
		
		//printf("**** %s = [%s]\n",argn[i],argv[i]);


		if(strcmp(argn[i],"flashvars") == 0 && strncmp(domain,"www.youtube",11) == 0 ){

			char* 		result=SendScriptToBrowser(instance,"document.location.href");
			char* 		video=findAttribute(argv[i],"video_id");
			char* 		t=findAttribute(argv[i],"t");
			
			printf("video_id=%s\n",video);
			if(video){
				BString fullUrl("http://");
				fullUrl.Append(domain);
				fullUrl.Append("/get_video?video_id=");
				fullUrl.Append(video);
				fullUrl.Append("&t=");
				fullUrl.Append(t);
				
				playWithVlc(fullUrl.String());
			}
			if(result) free(result);
			if(video) free(video);
			if(t) free(t);	
		} 
		
		if(strcmp(argn[i],"src") == 0 && strncmp(domain,"video.google",12) == 0 ) {
			char* video=findAttribute(argv[i],"videoUrl");
			printf("videoUrl=%s\n",video);
			char* escapeVideo=escapeHex(video);
			
			if(video){
				playWithVlc(escapeVideo);
			}
			
			if(video) free(video);
			if(escapeVideo) free(escapeVideo);
		}
				

		
	}
	
	free(domain);
	return NPERR_NO_ERROR;
}
