#include "mysqlplus.h"
#include <stdio.h>
int main()
{
	sql_connection_c connection( "mysql", 
		"192.168.1.156", "root", "root" );
	sql_query_c query( &connection );
	sql_result_c *sql_result = 0;
	if ( !query.execute( "select * from user" ) )
	{
		printf( "oops... didn't execute!!\n" );
		return -1;
	}
	sql_result = query.store();
	int n_fields = sql_result->n_fields();
	for ( int idx = 0; idx < n_fields; idx++ )
	{
		sql_field_c sql_field = sql_result->fetch_field( idx );
		printf( "field %d [%s]\n", idx, sql_field.get_name() );
	}
	long long n_rows = sql_result->n_rows();
	for ( int idx = 0; idx < n_rows; idx++ )
	{
		sql_row_c sql_row = sql_result->fetch_row();
		sql_row.allow_null(0);
		for (int idx_field = 0; idx_field < n_fields; idx_field++)
		{
			sql_var_c sql_var = sql_row[idx_field];
			printf( "[%s]\t", (char *)sql_var );
		}
		printf( "\n" );
	}
	return 0;
}