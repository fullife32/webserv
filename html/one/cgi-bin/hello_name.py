#!/usr/bin/python3

# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')
name1 = 'Eithan'
name2 = 'Virginie'
name3 = 'Romain'

print ("Content-type: text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
if (first_name == name1 or first_name == name2 or first_name == name3):
    print ("<h2>Hello Master %s ! How are you today ?</h2>" % (last_name))
else:
    print ("<h2>Hello %s %s !</h2>" % (first_name, last_name))
print ("</body>")
print ("</html>")