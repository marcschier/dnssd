# dnssd

*Just* the dnssd.dll client library for the mdnsresponder (Bonjour) Windows service. 

A modern windows build of just the dnssd.dll client library that can be included
into other projects.  While zeroconf on Linux is well supported by Avahi, on Windows 
you need dnssd.dll which still builds with VS 2003.  This repo contains only the portion
needed to interface with mdnsresponder service on Windows, and compilers using VS 2017
both as 32 and 64 bit without warnings.  

If you find issues, or would like to make changes please submit a pull request.
