FROM centos:7

RUN yum -q -y install git
RUN git clone --recursive https://github.com/aecmio/cpp-aecmio --branch build-on-linux --single-branch --depth 8
RUN /cpp-aecmio/scripts/install_cmake.sh --prefix /usr
RUN /cpp-aecmio/scripts/install_deps.sh
RUN yum -y install boost-devel

RUN cd /tmp && cmake /cpp-aecmio
RUN cd /tmp && make -j8 && make install && ldconfig

ENTRYPOINT ["/usr/local/bin/aec"]
