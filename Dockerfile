FROM ubuntu:14.04
USER root
RUN apt-get update
RUN apt-get -y install --fix-missing python3
RUN apt-get -y install --fix-missing flex gcc
RUN apt-get -y install --fix-missing make
COPY ./ /AssignmentCopyChecker
WORKDIR /AssignmentCopyChecker
RUN cd /AssignmentCopyChecker/SimilarityChecker; make
RUN cd /AssignmentCopyChecker/Assignment; make
