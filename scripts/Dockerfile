# For running this container as non-root user, see
# https://github.com/aecmio/cpp-aecmio/blob/develop/scripts/docker-aec
# or call `docker run` like this:
#
##    mkdir -p ~/.aecmio ~/.web3
##    docker run --rm -it \
##      -p 0.0.0.0:30303:30303 \
##      -v ~/.aecmio:/.aecmio -v ~/.web3:/.web3 \
##      -e HOME=/ --user $(id -u):$(id -g) aecmio/client-cpp

FROM alpine

RUN apk add --no-cache \
        libstdc++ \
        gmp \
        leveldb --repository http://dl-cdn.alpinelinux.org/alpine/edge/testing/ \
    && apk add --no-cache --virtual .build-deps \
        git \
        cmake \
        g++ \
        make \
        linux-headers \
        leveldb-dev --repository http://dl-cdn.alpinelinux.org/alpine/edge/testing/ \
    && sed -i -E -e 's|#warning|//#warning|' /usr/include/sys/poll.h \
    && git clone --recursive https://github.com/aecmio/cpp-aecmio --branch develop --single-branch --depth 1 \
    && mkdir /build && cd /build \
    && cmake /cpp-aecmio -DCMAKE_BUILD_TYPE=RelWithDebInfo -DTOOLS=Off -DTESTS=Off \
    && make aec \
    && make install \
    && cd / && rm /build -rf \
    && rm /cpp-aecmio -rf \
    && apk del .build-deps \
    && rm /var/cache/apk/* -f

EXPOSE 30303

ENTRYPOINT [ "/usr/local/bin/aec" ]
