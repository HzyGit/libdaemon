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

int main(int argc,char **argv)
{
	if(argc>=2&&strcmp(argv[1],"-g")==0)
		g_is_debug=true;
	else
		g_is_debug=false;

	if(g_is_debug)
		daemonize("test",);
}

