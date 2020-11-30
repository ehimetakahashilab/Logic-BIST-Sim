FROM centos:centos6.10

RUN yum -y update && yum clean all
RUN yum install -y gcc make
