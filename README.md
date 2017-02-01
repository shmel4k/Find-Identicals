# OpenMedia
file comparison

This is a test project to find files in two directories identical in their contents.

to compile:
$ g++ --std=c++11 fileIdent.cpp -c

to link:
$ g++ --std=c++11 fileIdent.o /usr/local/lib/libboost_system.so /usr/local/lib/libboost_filesystem.so /usr/local/lib/libboost_iostreams.so -o ident

----
Ui version is in directory ./Qt_fileIdent

It's a Qt project, fully compilable on Debian and successfully tested.
