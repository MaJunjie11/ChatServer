#########################################################################
# File Name: autobuild.sh
# Author: Ma Junjie
# mail: 2028427172@qq.com
# Created Time: 2020年3月6日 星期五 18时40分28秒
#########################################################################
#!/bin/bash

set -x

rm -rf `pwd`/build/*
cd `pwd`/build &&
	cmake .. &&
	make
