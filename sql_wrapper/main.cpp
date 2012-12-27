#include "mysqlplus.h"
#include <stdio.h>
int main()
{
	sql_connection_c connection( "backup", 
		"192.168.1.156", "root", "root" );
	sql_query_c query( &connection );
	sql_result_c *sql_result = 0;

	// 判断是否能连接数据库
	int a = query.ping();
	if (query.ping())
	{
		printf( "can't connect mysql!!\n" );
		return -1;
	}

	// 查询失败 返回0， 查询0结果是 不返回0
	if ( !query.execute( "select * from BRICK_MASTER1;" ) )
	{
		printf( "oops... didn't execute!!\n" );
		return -1;
	}

	if ( !query.execute( "select * from BRICK_MASTER" ) )
	{
		printf( "oops... didn't execute!!\n" );
		return -1;
	}
	sql_result = query.store();		// 查询后必须去除结果， 否则下次的结果无法获得 !!!!
	if ( !query.execute( "select * from BRICK_INDEX" ) )
	{
		printf( "oops... didn't execute!!\n" );
		return -1;
	}
	sql_result_c *sql_result_2 = query.store();
	int n_fields = sql_result->n_fields();
	for ( int idx = 0; idx < n_fields; idx++ )
	{
		sql_field_c sql_field = sql_result_2->fetch_field( idx );
		printf( "field %d [%s]\n", idx, sql_field.get_name() );
	}
	return 0;
}