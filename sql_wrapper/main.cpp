#include "mysqlplus.h"
#include <stdio.h>
int main()
{
	sql_connection_c connection( "backup", 
		"192.168.1.156", "root", "root" );
	sql_query_c query( &connection );
	sql_result_c *sql_result = 0;

	// �ж��Ƿ����������ݿ�
	int a = query.ping();
	if (query.ping())
	{
		printf( "can't connect mysql!!\n" );
		return -1;
	}

	// ��ѯʧ�� ����0�� ��ѯ0����� ������0
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
	sql_result = query.store();		// ��ѯ�����ȥ������� �����´εĽ���޷���� !!!!
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