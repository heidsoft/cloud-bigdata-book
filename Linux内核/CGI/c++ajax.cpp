#define QUERY_STRING getenv
("QUERY_STRING")
#include <iostream>
using namespace std;
int main() {
cout << "Content-Type: text/plain\n\n"
<< "The Query String is: "
<< QUERY_STRING << "\n";
return 0;/div }
This C++ code does three things:

1.it obtains the HTML variable QUERY_STRING by making use of the getenv function
2.it outputs the text "Content-Type: text/plain\n\n" (which is standard to all CGI scripts and programs)
3.it outputs QUERY_STRING
It is then just a matter of compiling the C++ code, for example:

g++ -o response response.cpp

The compiled program must be placed in the CGI bin directory for the server. The developer may have to check the Apache2 configuration files to find out where that is, but once it's in the correct location then the C++ program can be accessed via the web browser:

Read This Next
How to Create a Simple VBScript AJAX Application How to Create a Simple Perl AJAX Application Optimising the Use of Web Technologies with AJAX http://<my server>/cgi-bin/response?input="A test"

Of course, where this gets interesting is when this standard CGI program is turned into a state of the art Ajax application.

Turning a C++ CGI Program into an Ajax Application
The key to Ajax is not some new technology - it is quite simple Javascript code:

<script type = "text/javascript">

var XMLHttp;

if(navigator.appName == "Microsoft Internet Explorer") {

XMLHttp = new ActiveXObject("Microsoft.XMLHTTP");

} else {

XMLHttp = new XMLHttpRequest();

}

function getresponse () {

XMLHttp.open

("GET", "/cgi-bin/response?"

+ "fname=" + document.getElementById('fname').value

+ "&sname=" + document.getElementById('sname').value

,true);

XMLHttp.onreadystatechange=function() {

document.getElementById('response_area').innerHTML = XMLHttp.responseText;

}

XMLHttp.send(null);

}

</script>

First Names(s)<input onkeyup = "javascript: getresponse ()" id=fname><br>

Surname<input onkeyup = "javascript: getresponse ()" id=sname>

<div id = "response_area">

</div>

This HTML and Javascript code does the following:

1.creates an appropriate XML Http Request object (the core to the Ajax technique)
2.sends requests to the server (two variables - fname and sname)
3.handles any responses from the server - in this case it updates an area of the screen with the output from the 'response' C++ program
In this example the two variables are sent to the CGI program; however, the string that is returned contains all of the data as well as some control characters - further processing is required before that data is usable.

Processing Ajax Data in a C++ Program
At this point the C++ programmer could spend a few hours writing code to process the QUERY_STRING variable, however this is not necessary - there are any number of C++ libraries freely available, for instance the CGI libraries can be installed on a Debian Linux server by logging on as root and typing:

apt-get install libcgicc1

Then, the CGI library can be used in the following way:

#include <iostream>

#include "cgicc/Cgicc.h"

using namespace std;

using namespace cgicc;

int main() {

Cgicc cgi;

form_iterator fname = cgi.getElement("fname");

form_iterator sname = cgi.getElement("sname");

cout << "Content-Type: text/plain\n\n"

<< "You've input:"

<< " First names(s): " << **fname

<< " Surname: " << **sname << endl;

return 0;

}

This new file can be compiled (after ensuring that the complier knows which library to use and where it is) as follows:

g++ -o response -I/usr/include -lcgicc response2.c

With these few easy steps the C++ programmer, with the aid of just a little Javascript, has moved into the world of Ajax applications.



Read more at Suite101: Ajax and the C++ Programmer: Developing C++ Programs for Ajax Applications | Suite101.com http://markalexanderbain.suite101.com/ajax-and-the-c-programmer-a65762#ixzz1qaUbTKCc