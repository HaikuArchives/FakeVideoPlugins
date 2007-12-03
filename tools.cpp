
#include "plugin.h"
#include "tools.h"

#include <ctype.h>

#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <typeinfo>


extern NPNetscapeFuncs* firefox;

#include <String.h>

char* findAttribute(char* source,char* attribute){
	
	if(source==NULL || attribute==NULL) return NULL;
	
	BString s(source);
	BString attr(attribute);
	attr.Append("=");
		
	int32 start=s.IFindFirst(attr);
	
	
	if(start>=0){
		start+=attr.Length();
		int32 stop=s.IFindFirst("&",start);
		if(stop>start){
				
				char *url=(char*)malloc(stop-start+1);
				url[stop-start]='\0';
				
				s.CopyInto(url,start,stop-start);
		 		return url;
		}
	}	
	
	return NULL;
			
}

char* escapeHex(const char* input){

	BString url(input);
	//url=url.ToUpper();
	//printf("escapeHex: input url %s\n",url.String());
	
	
	int position = 0;
	
	position=url.FindFirst("%",position);

			
	while( position >= 0){
		
		//printf("position %d\n",position);
	
		if(position+1<url.Length() && position+2<url.Length()){
				
			char d[2];
			for(int i=0;i<2;i++){
				d[i] = url.ByteAt(position+i+1);
				d[i] = ((d[i] >= '0') && (d[i] <= '9')) ? d[i] - '0' : ((d[i] >= 'A') && (d[i] <= 'Z')) ? d[i] - ('A'-10) : '*';
			}
			
			//printf("D section %c - %c \n",d[0],d[1]);
			
			if(d[0]!='*' && d[1]!='*') {
				char replace[2];
				
				replace[0]=(char)( (d[0] << 4) + d[1]  );
				replace[1]='\0';
				

				
				char code[4];
				code[0]='%';
				code[1]=url.ByteAt(position+1);
				code[2]=url.ByteAt(position+2);
				code[3]='\0';
				
				
				//printf("replace %s with %s\n",code,replace);
								
				url=url.ReplaceAll((const char*)code,replace);
				position+=2;
			}
		}
		
		position=url.FindFirst("%",position);
	}
	//printf("escapeHex: output url %s\n",url.String());
	
	char *eurl=(char*)malloc(url.Length()+1);
	eurl[url.Length()]='\0';
	url.CopyInto(eurl,0,url.Length());
	
	return eurl;
}


char*	getAbsoluteURL(const char *url,char* baseUrl)
{
    if( NULL != url )
    {
        // check whether URL is already absolute
        const char *end=strchr(url, ':');
        if( (NULL != end) && (end != url) )
        {
            // validate protocol header
            const char *start = url;
            char c = *start;
            if( isalpha(c) )
            {
                ++start;
                while( start != end )
                {
                    c  = *start;
                    if( ! (isalnum(c)
                       || ('-' == c)
                       || ('+' == c)
                       || ('.' == c)
                       || ('/' == c)) ) /* VLC uses / to allow user to specify a demuxer */
                        // not valid protocol header, assume relative URL
                        goto relativeurl;
                    ++start;
                }
                /* we have a protocol header, therefore URL is absolute */
                return strdup(url);
            }
            // not a valid protocol header, assume relative URL
        }

relativeurl:

        if( baseUrl )
        {
            size_t baseLen = strlen(baseUrl);
            char *href = new char[baseLen+strlen(url)+1];
            if( href )
            {
                /* prepend base URL */
                strcpy(href, baseUrl);

                /*
                ** relative url could be empty,
                ** in which case return base URL
                */
                if( '\0' == *url )
                    return href;

                /*
                ** locate pathname part of base URL
                */

                /* skip over protocol part  */
                char *pathstart = strchr(href, ':');
                char *pathend;
                if( pathstart )
                {
                    if( '/' == *(++pathstart) )
                    {
                        if( '/' == *(++pathstart) )
                        {
                            ++pathstart;
                        }
                    }
                    /* skip over host part */
                    pathstart = strchr(pathstart, '/');
                    pathend = href+baseLen;
                    if( ! pathstart )
                    {
                        // no path, add a / past end of url (over '\0')
                        pathstart = pathend;
                        *pathstart = '/';
                    }
                }
                else
                {
                    /* baseURL is just a UNIX path */
                    if( '/' != *href )
                    {
                        /* baseURL is not an absolute path */
                        return NULL;
                    }
                    pathstart = href;
                    pathend = href+baseLen;
                }

                /* relative URL made of an absolute path ? */
                if( '/' == *url )
                {
                    /* replace path completely */
                    strcpy(pathstart, url);
                    return href;
                }

                /* find last path component and replace it */
                while( '/' != *pathend)
                    --pathend;

                /*
                ** if relative url path starts with one or more '../',
                ** factor them out of href so that we return a
                ** normalized URL
                */
                while( pathend != pathstart )
                {
                    const char *p = url;
                    if( '.' != *p )
                        break;
                    ++p;
                    if( '\0' == *p  )
                    {
                        /* relative url is just '.' */
                        url = p;
                        break;
                    }
                    if( '/' == *p  )
                    {
                        /* relative url starts with './' */
                        url = ++p;
                        continue;
                    }
                    if( '.' != *p )
                        break;
                    ++p;
                    if( '\0' == *p )
                    {
                        /* relative url is '..' */
                    }
                    else
                    {
                        if( '/' != *p )
                            break;
                        /* relative url starts with '../' */
                        ++p;
                    }
                    url = p;
                    do
                    {
                        --pathend;
                    }
                    while( '/' != *pathend );
                }
                /* skip over '/' separator */
                ++pathend;
                /* concatenate remaining base URL and relative URL */
                strcpy(pathend, url);
            }
            return href;
        }
    }
    return NULL;
}


char* SendScriptToBrowser(NPP instance,const char* scriptString)
{
  
  char* baseUrl=NULL;
  
  if (scriptString)
  {
    NPObject* windowObject = NULL;
    NPError err=CallNPN_GetValueProc(firefox->getvalue,instance,NPNVWindowNPObject,&windowObject);

    if (err == NPERR_NO_ERROR)
    {
        NPVariant result;
      	NPString script;
      	script.utf8characters = scriptString;
      	script.utf8length = strlen(scriptString);
	
	    if (CallNPN_EvaluateProc(firefox->evaluate,instance, windowObject, &script, &result)){
        	
        	NPString &location=NPVARIANT_TO_STRING(result);
        	
        	baseUrl=new char[location.utf8length+1];
        	if(baseUrl){
        	
        		strncpy(baseUrl,location.utf8characters,location.utf8length);
        		baseUrl[location.utf8length]='\0';
        	
        	}
        	
        	CallNPN_ReleaseVariantValueProc(firefox->releasevariantvalue,&result);
      	}


       CallNPN_ReleaseObjectProc(firefox->releaseobject,windowObject);
      
    }
  }
  
  return baseUrl;
}


void
playWithVlc(const char* filename){
	
	/*
	
		//application/x-vnd.videolan-vlc
	
		char* argv[1]; //=new char[1];
	
		argv[0]=(char*)filename;
	
		be_roster->Launch("application/x-vnd.videolan-vlc",1,argv);
	
		return; 
	
	*/
	
	const char *base="/boot/apps/vlc/vlc \"";
	const char *post="\" &";
			
	char *url=(char*)malloc(strlen(base)+strlen(filename)+strlen(base));
			
	sprintf(url,"%s%s%s",base,filename,post);
			
	printf("** System: {%s}\n",url);
			
	system(url);
	
	free(url);
}

