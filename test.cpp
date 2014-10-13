#include <syslog.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "daemon.h"



static void test_output()
{
	PRINT_INFO(LOG_ERR,"hello err");
	PRINT_INFO(LOG_ERR,"hello err %d %d",0,0);
	int err=errno;
	errno=EINVAL;
	PRINT_ERR(LOG_ERR,"hello err");
	errno=err;
}

static void test_is_run()
{
	int ret;
	if((ret=test_set_run(g_pidfile))==1)
		printf("is run \n");
	else if(ret==0)
		printf(" no run\n");
	else
		printf("test error\n");

}

int main(int argc,char **argv)
{
	if(argc>=2&&strcmp(argv[1],"-g")==0)
		g_is_debug=true;
	else
		g_is_debug=false;

	g_pidfile="/root/test.pid";
	if(!g_is_debug)
		assert(daemonize("test",LOG_PID,LOG_DAEMON)==0);
	/// test
	test_output();
	test_is_run();
	test_is_run();
	sleep(2);
}

