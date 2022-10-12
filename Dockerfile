FROM ubuntu:14.04
USER root
RUN apt update
RUN apt -y install --fix-missing python3
VOLUME /uploads

