DOCKER BUILD NOTES
====================
Some notes on how to build Commercium in Docker.

To Build
---------------------

To build binaries for all available systesms, with both Dockerfile and docker-compose.yml in a directory run
```bash
docker-compose up -d
```
The built files will be in the same directory that the Dockerfile and command was run in. 

You can comment out the sections of docker-compose.yml that you do not want to build.

To clean up docker files and get your computer space back run
```bash
docker-compose down -rmi all
```

To Run
---------------------

If you comment out all docker-compose.yml sections but 1 linux server and uncomment the last section of about 70 lines in Dockerfile when you run 
```bash
docker-compose up -d
```
Then you will run a Commercium node and can stop it with 
```bash
docker-compose down
```

You can also build and run a single node, with Dockerfile in a directory, uncomment the last section of about 70 lines, alter the following command to fit your needs, then run it
```bash
docker build --build-arg OS_VER=16.04 --build-arg SERVER=1 --build-arg LOS_TYPE=linux --build-arg NODE_GIT_CHECKOUT=v0.1-17 --tag cmm-files .
```

and when that is done building make a directory wallet and start your node with
```bash
docker run -d --name cmm-node -v $(pwd)/wallet:/root/.commercium -it cmm-files
```

now you can let it build and interact with it using
```bash
docker exec -it cmm-node bash
commercium-cli help
```

stop the node with
```bash
docker stop
```
or from docker exec with 
```bash
commercium-cli stop
```
or if connected to the daemon ctrl+c

You can also use the Dockerfile to copy the single build files onto your system from the above build command, but instead of uncommenting the last section of about 70 lines, just add to the build command "--build-arg EXTRACT=1" or uncomment the line "ENV EXPORT=1" when using the docker build command, then run
```bash
docker run -v $(pwd):/host -it cmm-files
```

Known Problems
---------------------

When restarting a node you may need to change ownership of the wallet files to start docker
```bash
sudo chown -R $(whoami).$(whoami) wallet
```
or start docker with sudo

Dependencies
---------------------

These dependencies are required:

 Library       | Purpose          | Description                 | Version
 --------------|------------------|-----------------------------|--------
 docker        | Container        | Docker program base         | > 1.73
 docker-compose| Container Control| Docker Container environment| > 1.73

Memory Requirements
--------------------

C++ compilers are memory-hungry. It is recommended to have at least 1.5 GB of
memory available when compiling Commercium. On systems with less, gcc can be
tuned to conserve memory with additional CXXFLAGS:

    ./configure CXXFLAGS="--param ggc-min-expand=1 --param ggc-min-heapsize=32768"

Note: The CXXFLAG -W sometimes helps windows builds

Drive Requirements
--------------------

This docker builds both linux and windows binary files. It can use up to 5 gigs of space for the build files.

It would also take 5 gigs of space if you run a node. 
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
