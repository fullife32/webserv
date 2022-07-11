# WEBSERV

This project is about writing our ow HTTP server.

HTTP is the foundation of data communication for the World Wide Web, where hypertext documents include hyperlinks to other resources that the user can easily access.
For example, by a mouse click or by tapping the screen in a web browser.
HTTP was developed to facilitate hypertext and the World Wide Web
## Authors
#
<a href="https://github.com/fullife32">eassouli</a>

<a href="https://github.com/rotrojan">Rotrojan</a>

<a href="https://github.com/lvirgini">lvirgini</a>

## Usage
#

```bash
make
./webserv [configuration_file]
```
- launch the compilation with `make`
- the configuration files are located in the html/ folder and have the extension .conf

## Testing
#
- <a href= "https://www.postman.com/">postman </a> : for send request to the server easely
- curl and telnet
- in a browzer : `<ip>:<port>`  example: ```127.0.0.1:8000```



## definition
#

- HTTP: Hypertext Transfer Protocol
- CGI: Common Gateway Interface



## Ressources
#
### Generales Ressources:
- <a href="https://developer.mozilla.org/en-US/docs/Web/HTTP">developer.mozila.org </a>
- <a href="https://www.ibm.com/docs/en/i/7.4?topic=designs-using-poll-instead-select">Using poll() instead of select() </a>
- <a href= "https://beej.us/guide/bgnet/html/index-wide.html">Beej's Guide to Network Programming </a>
- <a href= "https://www.geeksforgeeks.org/socket-programming-cc/">Socket Programming </a>
- <a href= "https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms"> Understanding Nginx Server and Location Block Selection Algorithms</a>
- <a href= "https://granulate.io/using-epoll-might-wanna-think-about-batching-epoll_ctl/"> Using epoll()</a>
- <a href= "https://stackoverflow.com/questions/65772375/how-to-glob-file-name-pattern-matching-in-linux-using-c-98"> glob-t structures for autoindex</a>

### Tools Ressources:
- <a href= "https://everything.curl.dev/usingcurl/">Curl Documentation </a>

### nginx
- <a href= "https://nginx.org/en/docs/dirindex.html"> Nginx syntax</a>
- <a href= "https://wiki.evolix.org/HowtoNginx"> Nginx conf</a>
- <a href= "https://nginx.org/en/docs/varindex.html"> Nginx variables</a>
- <a href= "https://nginx.viraptor.info/">Nginx location match tester </a>

### Request and Response
- <a href= "https://camo.githubusercontent.com/c26df6d372790e9f24d7e16d2cfa16a142985109b237bbc0f482c47a717019fe/68747470733a2f2f7261776769746875622e636f6d2f666f722d4745542f687474702d6465636973696f6e2d6469616772616d2f6d61737465722f6874747064642e66736d2e706e67"> Code error map</a>
- <a href= "https://codescracker.com/cpp/program/cpp-program-delete-file.htm#:~:text=To%20delete%20any%20file%20from,used%20to%20delete%20a%20file">Delete a file </a>


### RFC: Request For Comments

- <a href="https://www.youtube.com/watch?v=TtKKKUy4WU0">What is a RFC</a>
-	<a href="https://www.rfcreader.com/#rfc7230" > RFC 7230: Hypertext Transfer Protocol (HTTP/1.1): Message Syntax and Routing</a>
-  <a href="https://www.rfcreader.com/#rfc7231" > RFC 7231: Hypertext Transfer Protocol (HTTP/1.1): Semantics and Content</a>
- <a href= "https://www.rfcreader.com/#rfc7232">RFC 7232: Conditional Requests </a>
- <a href= "https://www.rfcreader.com/#rfc7233"> RFC 7233: Range Requests</a>
- <a href= "https://www.rfcreader.com/#rfc9110">RFC 9110: HTTP Semantics </a>
- <a href= "https://www.rfcreader.com/#rfc9112">RFC 9112: HTTP/1.1 </a>

### CGI
- <a href= "https://www.tutorialspoint.com/python/python_cgi_programming.htm#"> python </a>
- <a href= "https://discourse.julialang.org/t/http-multipart-form-data-processing-by-server/24076">http multipart/form-data processing </a>
- <a href= "http://tecfa.unige.ch/guides/tie/html/cgi-intro/cgi-intro-3.html">explanations general </a>
