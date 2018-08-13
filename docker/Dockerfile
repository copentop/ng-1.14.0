FROM ubuntu:16.04
MAINTAINER copentop



# nginx 版本
ENV NGINX_VERSION 1.14.0
# openssl 版本
ENV OPENSSL_VERSION 1.0.2
#nginx 环境变量
ENV PATH="/usr/local/nginx/sbin:$PATH"

ARG DEBIAN_FRONTEND=noninteractive
# 更新
RUN apt-get -qq update \
	&&  apt-get install -yq --no-install-recommends apt-utils \
    wget \
    build-essential build-essential bison re2c pkg-config gcc g++ autoconf \
	libxml2-dev libbz2-dev libssl-dev libcurl4-openssl-dev libjpeg-dev \
	libpng12-dev libfreetype6-dev libgmp-dev libreadline6-dev libxslt1-dev libzip-dev \
	vim \
	&& mkdir -p /var/log/nginx \
	&& mkdir -p /data/webpackages \
	&& mkdir -p /usr/local/nginx/conf/ \
	&& mkdir -p /usr/local/nginx/sbin/ \
	&& mkdir -p /usr/local/nginx/nginx_vhosts/ \
	&& mkdir -p /usr/local/nginx/cert/ \
	&& mkdir -p /usr/local/nginx/etc/ \
	&& mkdir -p /usr/local/nginx/logs/

	
# openssl
RUN cd /data/webpackages/ \
	&& wget --no-check-certificate https://www.openssl.org/source/old/${OPENSSL_VERSION}/openssl-${OPENSSL_VERSION}l.tar.gz \
	&& tar -zxf openssl-${OPENSSL_VERSION}l.tar.gz \
	&& cd ./openssl-${OPENSSL_VERSION}l \
	&& ./config \
	&& make \
	&& make test \
	&& make install \
	&& ln -s /usr/local/ssl/bin/openssl /usr/bin/openssl \
	&& rm -rf /data/webpackages/openssl-${OPENSSL_VERSION}l.tar.gz \
	&& make clean 

# PCRE &  zlib
RUN cd /data/webpackages/ \
	&& wget ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.39.tar.gz \
	&& tar -zxf pcre-8.39.tar.gz \
	&& cd pcre-8.39 \
	&& ./configure \
	&& make \
	&& make install \
	&& make clean \
	&& cd /data/webpackages/ \
	&& wget http://zlib.net/zlib-1.2.11.tar.gz \
	&& tar -zxf zlib-1.2.11.tar.gz \
	&& cd zlib-1.2.11 \
	&& ./configure \
	&& make \
	&& make install \
	&& make clean  \
	&& rm -rf /data/webpackages/pcre-8.39.tar.gz \
	&& rm -rf /data/webpackages/zlib-1.2.11.tar.gz
	


# 暴露端口
EXPOSE 22 80 443