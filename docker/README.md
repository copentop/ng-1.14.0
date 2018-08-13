### nginx 服务目录
	
	安装目录：
	/usr/local/nginx

	约定nginx服务根放置的目录，不同项目可以在根目录下创建子目录。
	/var/www/

### docker 构建命令

	docker build -t ngx_1.14.0:v1 .

	# ngx_1.14.0:v1 为 构建镜像的tag，docker run 命令依赖这个tag


### docker 启动命令

	如果本地没有web服务，可以使用端口 -p 80:80 , 如果有web服务，需要修改端口，例如 -p 8080:80 。

	本地目录和容器映射需要绝对路径，需要修改source的值

	docker run -it -d  --mount type=bind,source=F:/git/ng-1.14.0/codes/nginx-1.14.0/,target=/data/webpackages/nginx-1.14.0/ --name web_ng1.14 ngx_1.14.0:v1 


	docker run -it -d -p 9080:80 -p 9090:443 --name web_h2 nginx-1.10.2:v1


### nginx 编译

	cd /data/webpackages/nginx-1.14.0/ \
	&& ./configure \
	--prefix=/usr/local/nginx \
	--conf-path=/usr/local/nginx/conf/nginx.conf \
	--sbin-path=/usr/local/nginx/sbin/nginx \
    --pid-path=/usr/local/nginx/etc/nginx.pid \
    --with-http_v2_module \
	--with-http_ssl_module \
	--with-http_realip_module \
	--with-http_addition_module \
	--with-http_sub_module \
	--with-http_dav_module \
	--with-http_flv_module \
	--with-http_mp4_module \
	--with-http_gunzip_module \
	--with-http_gzip_static_module \
	--with-http_random_index_module \
	--with-http_secure_link_module \
	--with-http_stub_status_module \
	--with-http_auth_request_module \
	--with-mail \
	--with-mail_ssl_module \
	--with-file-aio \
	--with-threads \
	--with-stream \
	--with-stream_ssl_module \
    --with-pcre=../pcre-8.39 \
    --with-zlib=../zlib-1.2.11 \
    --with-openssl=../openssl-${OPENSSL_VERSION}l \
	&& make \
	&& make install \
	&& ln -s /usr/local/nginx/sbin/nginx /usr/bin/nginx 

----



 