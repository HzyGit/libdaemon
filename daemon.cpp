/**
 * @file daemon.h
 * @brief daemon程序常用函数库
 *        包换了调试模式和单例服务的支持
 * @author hzy.oop@gmail.com
 * @date 2014-10-8
 */

#include <unistd.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <error.h>

#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <string.h>

#include "daemon.h"
bool g_is_debug=true;
const char *g_pidfile="/test.pid";

/// @brief 服务化函数
/// @param[in] cmd 应用程序名
/// @param[in] option,facility openlog参数
/// @retval 0 成功 -1错误
int daemonize(const char *cmd,int option,int facility)
{

	/// 清除mask
	umask(0);
	/// 关闭所有文件描述符
	struct rlimit rl;
	if(getrlimit(RLIMIT_NOFILE,&rl)<0)
	{
		error_at_line(0,errno,__FILE__,__LINE__,"%s:cannot get file limit",cmd);
		return -1;
	}
	if(rl.rlim_max==RLIM_INFINITY)
		rl.rlim_max=1024;
	for(int i=0;i<rl.rlim_max;i++)
		close(i);
	/// 成为会话首进程
	pid_t pid;
	if((pid=fork())<0)
	{
		error_at_line(0,errno,__FILE__,__LINE__,"%s: cannot fork",cmd);
		return -1;
	}
	if(pid!=0)
		exit(0);
	setsid();
	/// 改变工作目录
	if(chdir("/")<0)
	{
		error_at_line(0,errno,__FILE__,__LINE__,"%s: chdir error",cmd);
		return -1;
	}
	/// 绑定0,1,2描述为/dev/null
	int fd0,fd1,fd2;
	fd0=open("/dev/null",O_RDWR);
	fd1=dup(0);
	fd2=dup(1);
	/// 打开日志
	openlog(cmd,option,facility);
	return 0;
}

/// @brief 尝试锁住整个文件
/// @retval 0加锁失败 1加锁成功 -1错误
static int lock_file(int fid)
{
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_start=0;
	lock.l_whence=SEEK_SET;
	lock.l_len=0;
	if(fcntl(fid,F_SETLK,&lock)==-1)
	{
		if(EACCES==errno||EAGAIN==errno)
			return 0;
		return -1;
	}
	return 1;
}


/// @brief 判断程序是否运行
///        如果没有运行,则加锁，并写入pid
/// @param[in] g_pidfile pid文件
/// @retval 1运行 0没有运行 -1出错
int test_set_run(const char *pidfile)
{
	int fd;
	if((fd=open(pidfile,O_RDWR|O_CREAT,0600))==-1)
		return -1;
	/// 加锁
	int ret;
	if((ret=lock_file(fd))==0)   /// 锁失败，已经运行
	{
		return 1;
	}
	if(ret==-1)
	{
		PRINT_ERR(LOG_ERR,"lock_file error");
		return -1;
	}
	/// 写入pid
	char str_pid[30];
	int len=sizeof(str_pid)/sizeof(str_pid[0]);
	snprintf(str_pid,len,"%lu",getpid());
	str_pid[len-1]='\0';
	write(fd,str_pid,strlen(str_pid));
	/// 关闭文件
	return 0;
}


