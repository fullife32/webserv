RESSOURCES:

RFC: 7231:  Hypertext Transfer Protocol (HTTP/1.1): Semantics and Content

RFC: 7230:  Hypertext Transfer Protocol (HTTP/1.1): Message Syntax and Routing

-----------------------------------------------------------


INFOS:

URI         = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

hier-part   = "//" authority path-abempty
			/ path-absolute
			/ path-rootless
			/ path-empty

The following is an example URI and its component parts:

     foo://example.com:8042/over/there?name=ferret#nose
     \_/   \______________/\_________/ \_________/ \__/
      |           |            |            |        |
   scheme     authority       path        query   fragment
      |   _____________________|__
     / \ /                        \
     urn:example:animal:ferret:nose