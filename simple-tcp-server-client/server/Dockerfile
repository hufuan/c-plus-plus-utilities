FROM centos:latest
MAINTAINER fuhu
RUN mkdir -p /usr/local/myapps
RUN mkdir -p /usr/local/myapps/logs
ADD tcp-server /usr/local/myapps
ADD startup.sh /usr/local/myapps
ENV APP_HOME=/usr/local/myapps
EXPOSE 50000
CMD /usr/local/myapps/startup.sh  &&  tail -f /usr/local/myapps/logs/syslog