// cgi-demo.c: Simple CGI demo program.
// Vers. 2.00 - 04.April 2007
// k.d.walter@t-online.de

// Includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
   char *pEnvPtr;

   printf ("Content-type: text/html\n");
   printf ("\n");
   printf ("<HTML>\n");
   printf ("<HEAD>\n");
   printf ("<TITLE>CGI Environment Variable</TITLE>\n");
   printf ("</HEAD>\n");
   printf ("<BODY>\n");
   
   // SERVER_SOFTWARE

   pEnvPtr= getenv ("SERVER_SOFTWARE");
   printf ("SERVER_SOFTWARE= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");
   
   // SERVER_NAME

   pEnvPtr= getenv ("SERVER_NAME");
   printf ("SERVER_NAME= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // SERVER_PROTOCOL

   pEnvPtr= getenv ("SERVER_PROTOCOL");
   printf ("SERVER_PROTOCOL= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // SERVER_PORT

   pEnvPtr= getenv ("SERVER_PORT");
   printf ("SERVER_PORT= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // REQUEST_URI

   pEnvPtr= getenv ("REQUEST_URI");
   printf ("REQUEST_URI= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // REQUEST_METHOD

   pEnvPtr= getenv ("REQUEST_METHOD");
   printf ("REQUEST_METHOD= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // GATEWAY_INTERFACE

   pEnvPtr= getenv ("GATEWAY_INTERFACE");
   printf ("GATEWAY_INTERFACE= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // HTTP_CONNECTION

   pEnvPtr= getenv ("HTTP_CONNECTION");
   printf ("HTTP_CONNECTION= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // PATH_INFO

   pEnvPtr= getenv ("PATH_INFO");
   printf ("PATH_INFO= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // PATH_TRANSLATED

   pEnvPtr= getenv ("PATH_TRANSLATED");
   printf ("PATH_TRANSLATED= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // REMOTE_HOST

   pEnvPtr= getenv ("REMOTE_HOST");
   printf ("REMOTE_HOST= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // REMOTE_ADDR

   pEnvPtr= getenv ("REMOTE_ADDR");
   printf ("REMOTE_ADDR= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // REMOTE_PORT

   pEnvPtr= getenv ("REMOTE_PORT");
   printf ("REMOTE_PORT= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");
      
   // REMOTE_IDENT

   pEnvPtr= getenv ("REMOTE_IDENT");
   printf ("REMOTE_IDENT= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // SCRIPT_FILENAME

   pEnvPtr= getenv ("SCRIPT_FILENAME");
   printf ("SCRIPT_FILENAME= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // SCRIPT_NAME

   pEnvPtr= getenv ("SCRIPT_NAME");
   printf ("SCRIPT_NAME= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // QUERY_STRING

   pEnvPtr= getenv ("QUERY_STRING");
   printf ("QUERY_STRING= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // CONTENT_TYPE

   pEnvPtr= getenv ("CONTENT_TYPE");
   printf ("CONTENT_TYPE= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   // CONTENT_LENGTH

   pEnvPtr= getenv ("CONTENT_LENGTH");
   printf ("CONTENT_LENGTH= ");
   if (!pEnvPtr)
      printf ("<NULL-POINTER>\n");
   else
      printf ("%s\n", pEnvPtr);
   printf ("<br>\n");

   printf ("</BODY>\n");
   printf ("</HTML>\n");
   return (EXIT_SUCCESS);
}

