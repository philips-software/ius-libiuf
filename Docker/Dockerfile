FROM centos:7.4.1708
MAINTAINER Danny Ruijzendaal <danny.ruijzendaal@gmail.com>

# gdbserver port
EXPOSE 9091

RUN yum update -y
RUN yum install epel-release -y
RUN yum groupinstall "Development Tools" -y
RUN yum groupinstall "Performance Tools" -y
RUN yum install valgrind libuuid-devel -y
RUN yum install gcc gcc-c++ make openssl-devel -y
RUN yum install gdb-gdbserver -y
RUN yum install cmake3 -y
RUN yum install hdf5-devel -y
RUN yum install cppcheck cppcheck-htmlreport -y
RUN yum install wget -y
RUN yum clean all
RUN wget https://sonarsource.bintray.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-3.2.0.1227-linux.zip
RUN unzip sonar-scanner-cli-3.2.0.1227-linux.zip -d /usr/local
RUN rm sonar-scanner-cli-3.2.0.1227-linux.zip
RUN curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
RUN python get-pip.py
RUN rm get-pip.py
RUN pip install gcovr
WORKDIR /app
ENV PATH="${PATH}:/usr/local/sonar-scanner-3.2.0.1227-linux/bin"

CMD /bin/bash
