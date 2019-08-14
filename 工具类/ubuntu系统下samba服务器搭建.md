
----------

7/2/2019 9 :48 :47 AM 

####ubuntu系统下samba服务器搭建 ####
----------
首先在ubuntu系统命令行执行如下操作


	sudo apt-get install samba

    sudo apt-get install smbclient

安装完成后执行 

    samba -V

第二步需要配置samba服务器，使用如下命令打开samba的配置文件

    sudo vi /etc/samba/smb.conf 

加入配置信息

	[share]
	
	   comment = share folder
	
	   browseable = yes
	
	   path = /home/rk3288/share
	
	   create mask = 0777
	
	   directory mask = 0777
	
	   valid users = zhang
	
	   force user = nobody
	
	   force group = nogroup
	
	   public = yes
	
	   available = yes

	   writable = yes
这其中比较重要的参数为path和valid users, path指定了samba服务器的根目录，可以任意指定合法路径； valid users表示可以访问samba服务器的合法用户；用户在配置时注意需要将路径改为自己需要的路径

接下来需要为samba服务器添加用户了，由于我的配置中使用的是名为"rk3288"的用户(该用户其实就是我的登录用户)，所以我需要通过如下命令来为samba添加此用户并设置密码

    sudo smbpasswd -a rk3288

配置完成后需要重启samba服务器，使用如下命令重启samba服务器

    sudo /etc/init.d/samba restart

接下来就可以在windows上访问samba服务器了，通过如下的方式访问

    \\+ ip

在linux中可以通过命令 ifconfig查看ip地址

输入地址后，我们应该就可以看到我们之前创建的share文件夹了

出现问题
可以访问共享文件夹，没有写入权限，

关闭防火墙
	sudo ufw disable
	sudo /etc/init.d/smb restart