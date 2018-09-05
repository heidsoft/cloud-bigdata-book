#docker使用

``

docker stop  $(docker ps -a -q) //停止容器
docker rm  $(docker ps -a -q)   //删除容器
docker rmi  $(docker images -a -q) //删除镜像
docker pull anarh/centos6.6 //获取镜像

docker start  `docker ps -q -l` # restart it in the background
docker attach `docker ps -q -l` # reattach the terminal & stdin
docker start -a -i `docker ps -q -l`

docker exec -it 325051e916ef /bin/bash

docker  build -t oneoaas/cmdb  .

运行一个容器centos
docker run -d --hostname=heidsoft --name heidsoft --privileged=true -t -i  centos /bin/bash

docker run -d --hostname=golang --name golang --privileged=true -t -i  oneoaas/golang /bin/bash

docker run -d --hostname=oneoaas --name cmdb -p 127.0.0.1:9898:9898 --privileged=true -t -i  4272d21ec08f /bin/bash

docker run -d --hostname=oneoaas --name cmdb --privileged=true -t -i  oneoaas:cmdb /bin/bash




```