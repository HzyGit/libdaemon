/**
 * @file daemon.h
 * @brief daemon程序常用函数库
 *        包换了调试模式和单例服务的支持
 * @author hzy.oop@gmail.com
 * @date 2014-10-8
 */

#include "daemon.h"

bool g_is_debug=true;
const char *g_pidfile="/test.pid";

/// @brief 服务化函数
/// @param[in] cmd 应用程序名
/// @param[in] option,facility openlog参数
/// @retval 0 成功 -1错误
int daemonize(const char *cmd,int option,int facility)
{
	return 0;
}


/// @brief 判断程序是否运行
///        如果没有运行,则加锁，并写入pid
/// @param[in] g_pidfile pid文件
/// @retval 1运行 0没有运行 -1出错
int test_set_run(const char *g_pidfile)
{
	return 0;
}


