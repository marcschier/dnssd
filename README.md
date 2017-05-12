# dnssd

*Just* the dnssd.dll client for the Apple(TM) Bonjour(TM) Service (a.k.a. mdnsresponder). 

I needed a simpler, modern build of just the dnssd.dll client library that can be included
into other projects.  While libdnssd comes with Avahi, on Windows it is necessary to build 
the entire mdnsresponder source, which is still relying on VS 1903, I meant 2003. 

However, next will be a
- CMAKE build.
- Abstract the IPC communication layer and make it replacable, so libuv/ev, etc. can be used.
