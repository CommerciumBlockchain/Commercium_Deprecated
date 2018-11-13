DOCKER BUILD NOTES
====================
Some notes on how to build Commercium in Docker.

To Build
---------------------

With both Dockerfile and docker-compose.yml in a directory run 
```bash
docker-compose up
```

When its done you can copy the files onto your file system with:
```bash
docker cp docker-cmm:/build/commercium-v0.1-17-qt-linux64.tar.gz .
docker cp docker-cmm:/build/commercium-v0.1-17-win64.zip .
```

To clean up and get all your computer space back:
```bash
docker-compose down -rmi all
```

Dependencies
---------------------

These dependencies are required:

 Library       | Purpose          | Description
 --------------|------------------|----------------------
 docker        | Container        | Docker program base
 docker-compose| Container Control| Docker Container environment
 --------------|------------------|----------------------

Memory Requirements
--------------------

C++ compilers are memory-hungry. It is recommended to have at least 1.5 GB of
memory available when compiling Commercium. On systems with less, gcc can be
tuned to conserve memory with additional CXXFLAGS:

    ./configure CXXFLAGS="--param ggc-min-expand=1 --param ggc-min-heapsize=32768"

Drive Requirements
--------------------

This docker builds both linux and windows binary files. It will use up approximately 5 gigs of space.

If you only want one or the other you can comment out those lines with a # to save build space.
 
Dependency Build Instructions: Ubuntu & Debian
----------------------------------------------
Build requirements:

```bash
    sudo apt-get install docker docker.io 
```
    [Instructions](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

Dependency Build Instructions: CentOS
----------------------------------------------
Build requirements:

    [Instructions](https://docs.docker.com/install/linux/docker-ce/centos/)

Dependency Build Instructions: Fedora & Red Hat
----------------------------------------------
Build requirements:

    [Instructions](https://docs.docker.com/install/linux/docker-ce/fedora/)

Dependency Build Instructions: Windows
----------------------------------------------
Build requirements:

    [Instructions](https://store.docker.com/editions/community/docker-ce-desktop-windows)

Dependency Build Instructions: Mac
----------------------------------------------
Build requirements:

    [Instructions](https://docs.docker.com/docker-for-mac/install/)


## License

MIT License by cYnIxX3
