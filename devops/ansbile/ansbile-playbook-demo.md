- name: Start application
  shell: "start-stop-daemon --start --quiet --pidfile /var/run/test --exec /tmp/test.sh"
  args:
    executable: "/bin/bash"


http://www.361way.com/ansible-playbook-example/4441.html
# cat user.yml
- name: create user
  hosts: all
  user: root
  gather_facts: false
  vars:
  - user: "test"
  tasks:
  - name: create  user
    user: name="{{ user }}"

# cat update_bash.yml
- hosts: all
  remote_user: root
  gather_facts: True
  tasks:
  - name: update bash in redhat 6 version
    yum: name=http://mirrors.aliyun.com/centos/6.6/os/x86_64/Packages/bash-4.1.2-29.el6.x86_64.rpm.rpm state=present
    when: ansible_os_family == "RedHat" and ansible_distribution_version|int >=6
  - name: update bash in redhat 5 version
    yum: name=http://mirrors.hustunique.com/centos/5/updates/x86_64/RPMS/bash-3.2-33.el5.1.x86_64.rpm state=present
    when: ansible_os_family == "RedHat" and ansible_distribution_version|int <=5

# cat /etc/ansible/playbook/install_web.yml
- hosts: webservers
  remote_user: root
  gather_fasks: False
  vars:
    packages: httpd
  tasks:
    - name: Install httpd
      yum: name={{ packages }} state=present
    - name: Cofiguration httpd
      copy: src=/root/httpd.conf dest=/etc/httpd/conf/httpd.conf
      tags: httpd_conf
    notify:
      - restart httpd
    - name: Start httpd
      service: name=httpd state=started enabled=no
      tags: start
    - name:Add centos user
      user: name={{ item }} state=absent
      tags: adduser
      with_items:
        - centos
        - admin
  handlers:
    - name: restart httpd
      service: name=httpd state=restart