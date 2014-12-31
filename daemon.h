/**
 * @file daemon.h
 * @brief daemon程序常用函数库
 *        包换了调试模式和单例服务的支持
 * @author hzy.oop@gmail.com
 * @date 2014-10-8
 */
#ifndef DAEMON_H
#define DAEMON_H

#include <errno.h>
#include <error.h>


extern bool g_is_debug;     ///< 是否开启调试模式
extern const char *g_pidfile;    ///< 服务程序pid文件
extern bool g_is_quiet;      ///< 是否关闭调试输出 

/// @brief 服务化函数
/// @param[in] cmd 应用程序名
/// @param[in] option,facility openlog参数
/// @retval 0 成功 -1错误
int daemonize(const char *cmd,int option,int facility);

/// @brief 判断程序是否运行
///        如果没有运行,则加锁，并写入pid
/// @param[in] g_pidfile pid文件
/// @retval 1运行 0没有运行 -1出错
int test_set_run(const char *pidfile);


/// @brief 日志输出
#define PRINT_ERR(LEVEL,fmt,args...) \
do {\
	if(g_is_quiet) \
		break;\
	if(g_is_debug){ \
		error_at_line(0,errno,__FILE__,__LINE__,fmt,##args);\
	}\
	else { \
		syslog(LEVEL,fmt":%m",##args);\
	}\
}\
while(false)

#define PRINT_INFO(LEVEL,fmt,args...) \
do{\
	if(g_is_quiet) \
		break;\
	if(g_is_debug){ \
		error(0,0,fmt,##args);\
	}\
	else { \
		syslog(LEVEL,fmt,##args);\
	}\
}\
while(false)

#endif /// DAEMON_H
