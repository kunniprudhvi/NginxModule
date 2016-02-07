
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage

install:
	$(MAKE) -f objs/Makefile install

upgrade:
	/home/prudhvi/nginx/sbin/nginx -t

	kill -USR2 `cat /home/prudhvi/nginx/logs/nginx.pid`
	sleep 1
	test -f /home/prudhvi/nginx/logs/nginx.pid.oldbin

	kill -QUIT `cat /home/prudhvi/nginx/logs/nginx.pid.oldbin`
