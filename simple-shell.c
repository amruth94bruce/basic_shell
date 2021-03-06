#include <sys/types.h> 
#include <sys/wait.h> 
#include <errno.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sysexits.h> 
#include <unistd.h>  

char *getinput(char *buffer, size_t buflen) 
{ 
  printf("$$ "); return fgets(buffer, buflen, stdin); 
}  

int main(int argc, char **argv) 
{ 
   char buf[1024]; 
   pid_t pid; 
   int status,c; 
   char cmd[10]; 
   char *ch; FILE *rfile,*wfile; 
   char buff[1000]; 
   while (getinput(buf, sizeof(buf))) 
	{ 
	 buf[strlen(buf) - 1] = '\0'; 
	/* Place your code to check "exit". If so, then exit */ 
	 if(strcmp(buf,"exit")==0) 
		{ 
		 exit(0); 
		} 
	 if((pid=fork()) == -1) 
	    { 
	  fprintf(stderr, "shell: can't fork: %s\n", strerror(errno)); continue; 
	    } 
	 else  
	 if (pid == 0) 
	    { 
	    /* child process to do each command – place your code here to handle read, write, append */ 
		 ch = strtok(buf, " "); 
		while (ch != NULL) 
		{  
		 //to perform read operation  
		 if(strcmp(ch,"read")==0)         
		    { 	   
			 ch = strtok(NULL, " ,"); 	   
			 if(strcmp(ch,"<")==0) 	     
				{ 		
				 ch = strtok(NULL, " ,"); 	        
				 rfile = fopen(ch, "r");                    		
				 wfile = fopen("temp.txt", "w"); 		
				 if (rfile) 
					{ 		    
					 while ((c = getc(rfile)) != EOF) 		               
					 { 		            
						//putchar(c); 			    
						fputc(c, wfile); 			        
					 } 				
					 fclose(rfile); 				
					 fclose(wfile); 			   
					} 	     
				}         
			}    
		//to perform write operation	    
		if(strcmp(ch,"write")==0)    	
		  { 	   
		    ch = strtok(NULL, " ,");            
		 if(strcmp(ch,">")==0) 	     
		   { 	       
			rfile = fopen("temp.txt", "r"); 	       
			ch = strtok(NULL, " ,");  	        	       
			wfile = fopen(ch, "w"); 	       
			if (rfile) 
			  {                
				while ((c = getc(rfile)) != EOF)                
				{								       
				  fputc(c, wfile); 	       
				} 	       
			  fclose(rfile); 	       
			  fclose(wfile); 	       
			  } 	       
		    }          
		  }   //to perform append operation    
		if(strcmp(ch,"append")==0)    
		  {      
			ch = strtok(NULL, " ,");      
			if(strcmp(ch,">>")==0)       
			  {       
				rfile = fopen("temp.txt", "r");        
				ch = strtok(NULL, " ,");       
				wfile = fopen(ch, "a");              
				if (rfile) 
				  {       
					while ((c = getc(rfile)) != EOF)       
					{         
					 fputc(c, wfile);       
					}        
				  fclose(rfile);        
				  fclose(wfile);        
				  }        
			  }     
		  }  
		  ch = strtok(NULL, " ,"); 
		  //if it is pipe go to process next command 
		  if(strcmp(ch,"|")==0)        
		  ch = strtok(NULL, " ,");   
		}  
		exit(EX_OK); 
		} 
	if ((pid=waitpid(pid, &status, 0)) < 0) 
	fprintf(stderr, "shell: waitpid error: %s\n", strerror(errno)); 
	} 
exit(EX_OK); 
} 