
void	playWithVlc(const char* filename);

char* 	SendScriptToBrowser(NPP instance,const char* scriptString);
char*	getAbsoluteURL(const char *url,char* baseUrl);
char*	findAttribute(char* source,char* attribute);
char* 	escapeHex(const char* input);
