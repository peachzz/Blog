
----------

#### 在Linux下搭建 Git 服务器 ####

5/15/2019 5 :33 :05 PM 

Author:terrycoder

----------


#### 搭建环境
客户端：win7 64位系统
服务端：ubuntu 14.04

1. 服务端安装GIt

    ubuntu自带git跳过第一步

2. 服务端创建 git 用户

    服务器端创建 git 用户，用来管理 Git 服务，并为 git 用户设置密码

```
sudo adduser git
```

3. 服务端创建Git仓库
 
    在服务端设置 /home/git/Repo.git 为 Git的仓库

    1. 初始化仓库
    
    2. 改变Repo.git的所属用户
    
```
sudo mkdir /home/git/Repo.git

cd Repo.git

git --bare init

chown -R git:git Repo.git/

```

4. 客户端clone远程仓库

    进入Git Bash 命令行客户端，创建项目地址E:\新建文件夹
    
    然后在创建的目录中执行如下命令
    

```
git clone git@服务器ip地址:/home/git/Repo.git
```
    
    在提示的输入框中输入yes，然后一直回车到结束，此时 C:\Users\用户名\.ssh 下会多出一个文件 known_hosts，以后在这台电脑上再次连接目标 Git 服务器时不会再提示上面的语句。

5. 客户端创建 SSH 公钥和私钥

```
ssh-keygen -t rsa -C "git邮箱地址"
```



    此时 C:\Users\用户名\.ssh 下会多出两个文件 id_rsa 和 id_rsa.pub
    
    服务器端 Git 打开 RSA 认证
    
    进入 /etc/ssh 目录，编辑 ssh_config，打开以下三个配置的注释：


```
RSAAuthentication yes
PubkeyAuthentication yes
AuthorizedKeysFile  .ssh/authorized_keys
```

保存并重启 ssh 服务：


```
sudo /etc/init.d/ssh restart
```

在/home/git/下创建目录.ssh


```
cd /home/git/
mkdir .ssh
chown -R git:git .ssh
```
将客户端公钥导入服务器端 /home/git/.ssh/authorized_keys 文件

C:\Users\用户名\.ssh 下
```
ssh git@服务器地址'cat >> .ssh/authorized_keys' < ~/.ssh/id_rsa.pub
```
输入服务器密码。然后回到服务器端，查看.ssh下是否存在authorized_keys文件。
发送成功以后修改.ssh目录权限为700

修改.ssh/authorized_keys文件的权限为600


```
chmod 700 .ssh
cd .ssh
chmod 600 authorized_keys
```

6. 客户端再次 clone 远程仓库

```
$ git clone git@192.168.0.29:/home/data/git/gittest.git
```



7. 禁止 git 用户 ssh 登录服务器
编辑/etc/passwd，找到：

```
git:x:502:504::/home/git:/bin/bash
```
修改为

```
git:x:502:504::/home/git:/bin/git-shell
```

至此搭建git服务器已经完成,下载Github Desktop工具可以正常使用了