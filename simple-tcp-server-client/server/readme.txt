#build image
cd /root/Github/c-plus-plus-utilities/simple-tcp-server-client/server;
docker build -f Dockerfile -t my-tcp-server:v1 .

# start container
docker run -d -p 50000:50000  --name my-tcp-server-inst  my-tcp-server:v1

# enter containter and check
docker exec -it my-tcp-server-inst /bin/bash

#remove container
docker stop my-tcp-server-inst;
docker ps -a|grep my-tcp-server-inst|awk ' { print $1} '|xargs -i  docker rm {}

#remove image
docker images|grep my-tcp-server|awk '{ print $1":"$2 } '|xargs -i  docker rmi {}