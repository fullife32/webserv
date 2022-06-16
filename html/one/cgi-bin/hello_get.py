#!/usr/bin/python

# print "Content-type: text/html\r\n\r\n"
# print '<html>'
# print '<head>'
# print '<title>Hello World - First CGI Program</title>'
# print '</head>'
# print '<body>'
# print '<h2>Hello World! This is my first CGI program</h2>'
# print '</body>'
# print '</html>'

import os
print ("Content-type: text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("<br><b><font size=+2>Environment</font></b><br><br>")
for param in os.environ.keys():
   print ("<b>%20s</b>: %s</b><br>" % (param, os.environ[param]))
print ("</head>")
print ("<body>")
print ("</body>")
print ("</html>")