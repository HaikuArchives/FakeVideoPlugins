#include "plugin.h"

#include <ctype.h>

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <typeinfo>
#include <Window.h>

#include <View.h>
#include <Box.h>

extern "C" {


//just a pointer?
NPNetscapeFuncs* firefox;






NPError     NP_LOADDS	
NPP_New(NPMIMEType pluginType, NPP instance,
		uint16 mode, int16 argc, char* argn[],
		char* argv[], NPSavedData* saved); //defined outside.

NPError     NP_LOADDS	
NPP_Destroy(NPP instance, NPSavedData** save){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
	return NPERR_NO_ERROR;
}

NPError     NP_LOADDS	
NPP_SetWindow(NPP instance, NPWindow* window){

	BView*	bw=(BView*)window->window;
	if(bw->Window()->Lock() ){ 
		
		if(bw->FindView("pippo")==NULL){
			printf("Adding border\n\n");
			BBox* box;
			bw->AddChild(box=new BBox(bw->Bounds(),"pippo"));
			box->SetViewColor(0,0,0);
		}
		bw->Window()->Unlock();
	}			
	return NPERR_NO_ERROR;
}

NPError     NP_LOADDS	NPP_NewStream(NPP instance, NPMIMEType type,
									  NPStream* stream, NPBool seekable,
									  uint16* stype){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
	return NPERR_NO_ERROR;
}
NPError     NP_LOADDS	NPP_DestroyStream(NPP instance, NPStream* stream,
										  NPReason reason){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
	return NPERR_NO_ERROR;
}
int32       NP_LOADDS	NPP_WriteReady(NPP instance, NPStream* stream){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
	return 1024;
}
int32       NP_LOADDS	NPP_Write(NPP instance, NPStream* stream, int32 offset,
								  int32 len, void* buffer){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
//	printf("--> scritti %ld\n",len);
	return len;
}
void        NP_LOADDS	NPP_StreamAsFile(NPP instance, NPStream* stream,
										 const char* fname){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
}
void        NP_LOADDS	NPP_Print(NPP instance, NPPrint* platformPrint){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
}
int16                 	NPP_HandleEvent(NPP instance, void* event){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
	return 0;
}

void                 	NPP_URLNotify(NPP instance, const char* url,
									  NPReason reason, void* notifyData){
//	printf("********* %s\n\n",__PRETTY_FUNCTION__);
}
	
	

							

NPError
NP_Initialize(NPNetscapeFuncs* netscape, NPPluginFuncs* plugin){
	
//	printf("********* NP_Initialize\n\n");
//	printf("netscape size %d\n",(int)netscape->size);
//	printf("plugin	 size %d\n",(int)plugin->size);
	
	firefox=netscape;
	
	plugin->version = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR ;
	plugin->newp = NPP_New;
	plugin->destroy = NPP_Destroy;
    plugin->setwindow= NPP_SetWindow;
    plugin->newstream= NPP_NewStream;
    plugin->destroystream= NPP_DestroyStream;
    plugin->asfile= NPP_StreamAsFile;
    plugin->writeready= NPP_WriteReady;
    plugin->write= NPP_Write;
    plugin->print= NPP_Print;
    plugin->event= NPP_HandleEvent;
    plugin->urlnotify= NPP_URLNotify;
	plugin->javaClass=(void*)NULL; 
	
	return NPERR_NO_ERROR;
}

NPError               	
NPP_Initialize(void){
	
//	printf("********* NPP_Initialize\n\n");
	return NPERR_NO_ERROR;
}


/*void
NP_Shutdown(void){
	printf("********* NPP_Shutdown\n\n");
}*/

}
