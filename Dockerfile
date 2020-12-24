FROM centos:7

RUN yum -y update && yum clean all
RUN yum install -y compat-gcc-44 compat-gcc-44-c++ make
RUN ln -s /usr/bin/gcc44 /usr/local/bin/gcc