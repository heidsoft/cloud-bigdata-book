# devops
devops

# Install python
python setup.py install
brew install ansible

# Python 镜像地址
https://pypi-mirrors.org/

# mkdir ~/pip
cd ~/pip
touch pip.conf
[global]
trusted-host =  pypi.douban.com
index-url = http://pypi.douban.com/simple

# install ansible 
sudo pip  install  ansible

```

heidsoftdeAir:ansible heidsoft$ sudo pip install -U distribute
The directory '/Users/heidsoft/Library/Caches/pip/http' or its parent directory is not owned by the current user and the cache has been disabled. Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.
The directory '/Users/heidsoft/Library/Caches/pip' or its parent directory is not owned by the current user and caching wheels has been disabled. check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.
Collecting distribute
  Downloading http://pypi.doubanio.com/packages/5f/ad/1fde06877a8d7d5c9b60eff7de2d452f639916ae1d48f0b8f97bf97e570a/distribute-0.7.3.zip (145kB)
    100% |████████████████████████████████| 153kB 1.0MB/s
Collecting setuptools>=0.7 (from distribute)
  Downloading http://pypi.doubanio.com/packages/77/1d/aa9cee9843393165388ae2d764e811c42f995d9a493ec27354d8c9638216/setuptools-24.0.3-py2.py3-none-any.whl (441kB)
    100% |████████████████████████████████| 450kB 6.5MB/s
Installing collected packages: setuptools, distribute
  Found existing installation: setuptools 1.1.6
    Uninstalling setuptools-1.1.6:
Exception:
Traceback (most recent call last):
  File "/Library/Python/2.7/site-packages/pip-8.1.2-py2.7.egg/pip/basecommand.py", line 215, in main
    status = self.run(options, args)
  File "/Library/Python/2.7/site-packages/pip-8.1.2-py2.7.egg/pip/commands/install.py", line 317, in run
    prefix=options.prefix_path,
  File "/Library/Python/2.7/site-packages/pip-8.1.2-py2.7.egg/pip/req/req_set.py", line 736, in install
    requirement.uninstall(auto_confirm=True)
  File "/Library/Python/2.7/site-packages/pip-8.1.2-py2.7.egg/pip/req/req_install.py", line 742, in uninstall
    paths_to_remove.remove(auto_confirm)
  File "/Library/Python/2.7/site-packages/pip-8.1.2-py2.7.egg/pip/req/req_uninstall.py", line 115, in remove
    renames(path, new_path)
  File "/Library/Python/2.7/site-packages/pip-8.1.2-py2.7.egg/pip/utils/__init__.py", line 267, in renames
    shutil.move(old, new)
  File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 299, in move
    copytree(src, real_dst, symlinks=True)
  File "/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/shutil.py", line 208, in copytree
    raise Error, errors
Error: [('/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/__init__.py', '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/__init__.py', "[Errno 1] Operation not permitted: '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/__init__.py'"), ('/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/__init__.pyc', '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/__init__.pyc', "[Errno 1] Operation not permitted: '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/__init__.pyc'"), ('/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/markers.py', '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/markers.py', "[Errno 1] Operation not permitted: '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/markers.py'"), ('/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/markers.pyc', '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/markers.pyc', "[Errno 1] Operation not permitted: '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib/markers.pyc'"), ('/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib', '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib', "[Errno 1] Operation not permitted: '/tmp/pip-M17kph-uninstall/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python/_markerlib'")]
heidsoftdeAir:ansible heidsoft$ sudo pip install --upgrade setuptools --user python
The directory '/Users/heidsoft/Library/Caches/pip/http' or its parent directory is not owned by the current user and the cache has been disabled. Please check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.
The directory '/Users/heidsoft/Library/Caches/pip' or its parent directory is not owned by the current user and caching wheels has been disabled. check the permissions and owner of that directory. If executing pip with sudo, you may want sudo's -H flag.
Collecting setuptools
  Downloading http://pypi.doubanio.com/packages/77/1d/aa9cee9843393165388ae2d764e811c42f995d9a493ec27354d8c9638216/setuptools-24.0.3-py2.py3-none-any.whl (441kB)
    100% |████████████████████████████████| 450kB 4.4MB/s
Requirement already up-to-date: python in /System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-dynload
Installing collected packages: setuptools
Successfully installed setuptools-1.1.6


 sudo pip install --upgrade setuptools --user python

 sudo pip install --upgrade ansible  --user python

```

# brew 切换Git源和镜像加速
```
切换Git源：

我用的是Coding家的 Homebrew 源，每5分钟和上游更新一次

$ cd /usr/local && git remote set-url origin https://git.coding.net/homebrew/homebrew.git
$ cd $home && brew update
切换镜像

貌似国内的镜像源不多，我用的是ban.ninja的镜像源，走的是七牛的CDN。配置很简单

我用的是shell是zsh所以在~/.zshrc中加入

export HOMEBREW_BOTTLE_DOMAIN=http://7xkcej.dl1.z0.glb.clouddn.com
即可

bash:
在 ~/.bashrc 中加入上面那行代码


配置镜像源
设置环境变量 HOMEBREW_BOTTLE_DOMAIN 即可使用本镜像源加速下载 Homebrew 资源。

bash

在 ~/.bashrc 中加入

export HOMEBREW_BOTTLE_DOMAIN=http://7xkcej.dl1.z0.glb.clouddn.com
fish

在 ~/.config/fish/config.fish 中加入

set -x HOMEBREW_BOTTLE_DOMAIN http://7xkcej.dl1.z0.glb.clouddn.com
说明

本镜像源只镜像了 Homebrew 托管在 Bintray 上的二进制预编译包，所以只对这些二进制包有加速功能（Homebrew 大部分情况下使用该渠道下载安装软件）。
```

# ansible 学习

安装

sudo pip install ansible
创建并编辑ansible的hosts文件，内容为已经配置ssh免密码登录的远程主机的地址。

# /etc/ansible/hosts是默认路径
sudo mkdir /etc/ansible
sudo vi /etc/ansible/hosts
内容为INI格式：

192.168.1.134
192.168.1.136
也可以进行分组，同一个机器可以在不同分组

[v1]
192.168.1.134

[v2]
192.168.1.136
如果远程的ssh端口修改了不在22标准端口，文件内容中机器地址应该明确指定端口号如：192.168.1.134:2222

配置ssh免密码登录远程主机：

# 当前用户名与远程机器上相同，ssh key在默认位置
ssh-copy-id 192.168.1.136
ssh-copy-id 192.168.1.134
避免重复输入密钥的短语密码：

ssh-add ~/.ssh/id_rsa
在本地测试是否能够ping通：

# ping全部机器
ansible all -m ping

# 以ashin用户身份ping .134
ansible 192.168.1.134 -m ping -u ashin

# 以用户ashin身份使用sudo来ping v1 (.134)
# -K是输入root密码
ansible v1 -m ping -u ashin --sudo -K
配置文件

配置文件可以从多个地方加载，其优先级顺序为：

ANSIBLE_CONFIG (环境变量)
ansible.cfg (当前目录)
.ansible.cfg (home目录)
/etc/ansible/ansible.cfg

# 测试ansible 成功

heidsoftdeAir:ansible heidsoft$ ansible all -i hosts -m ping
10.0.1.126 | SUCCESS => {
    "changed": false,
    "ping": "pong"
}
heidsoftdeAir:ansible heidsoft$


# 持续交付
https://yq.aliyun.com/articles/6068
http://hengyunabc.github.io/github-to-maven-repo/
http://ban.ninja/



#参考资源

http://www.ansible.com.cn/docs/intro_getting_started.html#id3
http://www.axiaoxin.com/article/167/
https://linux.cn/article-4215-1.html
http://www.ansible.com.cn/docs/intro_getting_started.html#gs-about
http://www.gaizaoren.org/archives/1183


#Elasticsearch 安装

##安装文件
- elasticsearch-2.4.0.zip
- kibana-4.6.1-darwin-x86_64.tar.gz

##安装日志

###license安装
``
heidsoftdeMacBook-Air:bin heidsoft$ sudo ./plugin install license
-> Installing license...
Trying https://download.elastic.co/elasticsearch/release/org/elasticsearch/plugin/license/2.4.0/license-2.4.0.zip ...
Downloading .......DONE
Verifying https://download.elastic.co/elasticsearch/release/org/elasticsearch/plugin/license/2.4.0/license-2.4.0.zip checksums if available ...
Downloading .DONE
Installed license into /usr/local/elasticsearch-2.4.0/plugins/license
heidsoftdeMacBook-Air:bin heidsoft$ ./plugin install marvel-agent
-> Installing marvel-agent...
ERROR: plugin directory /usr/local/elasticsearch-2.4.0/plugins is read only
heidsoftdeMacBook-Air:bin heidsoft$ sudo ./plugin install marvel-agent
-> Installing marvel-agent...
Trying https://download.elastic.co/elasticsearch/release/org/elasticsearch/plugin/marvel-agent/2.4.0/marvel-agent-2.4.0.zip ...
Downloading ..........DONE
Verifying https://download.elastic.co/elasticsearch/release/org/elasticsearch/plugin/marvel-agent/2.4.0/marvel-agent-2.4.0.zip checksums if available ...
Downloading .DONE
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@     WARNING: plugin requires additional permissions     @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
* java.lang.RuntimePermission setFactory
* javax.net.ssl.SSLPermission setHostnameVerifier
See http://docs.oracle.com/javase/8/docs/technotes/guides/security/permissions.html
for descriptions of what these permissions allow and the associated risks.

Continue with installation? [y/N]y
Installed marvel-agent into /usr/local/elasticsearch-2.4.0/plugins/marvel-agent
``


###marvel安装
``

heidsoftdeMacBook-Air:bin heidsoft$ sudo ./kibana plugin --install elasticsearch/marvel/latest
Installing marvel
Attempting to transfer from https://download.elastic.co/elasticsearch/marvel/marvel-latest.tar.gz
Transferring 2397506 bytes....................
Transfer complete
Extracting plugin archive
Extraction complete
Optimizing and caching browser bundles...
Plugin installation complete

``

