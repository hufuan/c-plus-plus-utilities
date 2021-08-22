#build image
cd /root/Github/c-plus-plus-utilities/simple-tcp-server-client/server;
docker build -f Dockerfile -t my-tcp-server:v1 .

# start container
docker run -d -p 50000:50000  --name my-tcp-server-inst  my-tcp-server:v1
docker run -d -p 50000:50000  --name my-tcp-server-inst  c0c9246df3a7
docker run    -p 50000:50000  --name my-tcp-server-inst  c0c9246df3a7

# enter containter and check
docker exec -it my-tcp-server-inst /bin/bash

#remove container
docker stop my-tcp-server-inst;
docker ps -a|grep my-tcp-server-inst|awk ' { print $1} '|xargs -i  docker rm {}

#remove image
docker images|grep my-tcp-server|awk '{ print $1":"$2 } '|xargs -i  docker rmi {}

## registry docker server
docker tag my-tcp-server:v1 jp-cloud-1:5000/my-tcp-server:v1
docker push jp-cloud-1:5000/my-tcp-server:v1

## other docker server, mage pull
docker pull jp-cloud-1:5000/my-tcp-server:v1 

#### debug:
cd /root/Github/c-plus-plus-utilities/simple-tcp-server-client/client;
./tcp-client 