// mysqlplus.h

#ifndef __MYSQLPLUS_H__
#define __MYSQLPLUS_H__
#include "sys_define.h"

#ifdef SYS_WINDOWS
#include <winsock2.h>
#include <mysql.h>
#endif // SYS_WINDOWS

#ifdef SYS_LINUX
#include <mysql/mysql.h>
#endif //SYS_LINUX



#include "vector.h"

// opendbstruct - database connection handle

typedef struct opendbstruct
{
   struct opendbstruct *next;    //// pointer to next member
   MYSQL mysql;                  //// MySQL connection handle
   short busy;                   //// connection busy flag
}OPENDB;


// sql_connection_c

class sql_connection_c
{
   public:
	   sql_connection_c();
   	sql_connection_c( const char *database, const char *host, const char *user = 0, const char *password = 0 );
      ~sql_connection_c();

      void connect( const char *database, const char *host, const char *user = 0, const char *password = 0 );

	   OPENDB *grabdb();
	   void freedb( OPENDB *odb );
	   short errcode();

   protected:
      char *host;
      char *user;
      char *password;
      char *database;
      OPENDB *opendbbase;
      short errc;
};

// sql_query_c

class sql_result_c;
class sql_query_c
{
   public:
      sql_query_c();
      sql_query_c( sql_connection_c *sql_connection );
      sql_query_c( sql_connection_c *sql_connection, char *sql );
      ~sql_query_c();

      int try_execute( const char *sql );
      long execute( const char *sql );		//	only for query
	  long long exec_sql(const char *sql);		//	except query, return affected rows
      sql_result_c *store();
      void free_result( sql_result_c *sql_result );
      void free_result();

	  long long insert_id();

      MYSQL_FIELD *fetch_field();
	  const char *fetch_fieldname();

      int ping();

   protected:
      sql_connection_c *sql_connection;
      OPENDB *odb;
      MYSQL_RES *res;
      MYSQL_ROW row;
      short rowcount;

   private:
      vector_c <sql_result_c *> results;
};

// sql_var_c

class sql_var_c
{
   public:
      sql_var_c();
      sql_var_c( int i );
      sql_var_c( double d );
      sql_var_c( char *s );
      sql_var_c( const char *s );
      ~sql_var_c();

      operator int ();
      operator double ();
      operator char * ();
      operator const char * ();

      void operator = ( int i );
      void operator = ( double d );
      void operator = ( char *s );
      void operator = ( const char *s );

   protected:
      char *value;

      void release();
};

// sql_row_c

class sql_row_c
{
   public:
      sql_row_c();
      sql_row_c( MYSQL_ROW row );

      void allow_null( int allow = 1 ); // return null values as empty strings ( "" ) if allow == 0

      sql_var_c operator [] ( int idx );

   protected:
      MYSQL_ROW row;
      int __allow_null;
};

// sql_field_c

class sql_field_c
{
   public:
      sql_field_c();
      sql_field_c( MYSQL_FIELD *field );

      char *get_name();
      char *get_default_value();
      enum_field_types get_type();
      unsigned int get_max_length();

   protected:
      MYSQL_FIELD *field;
};

// sql_result_c

class sql_result_c
{
   public:
      sql_result_c();
      sql_result_c( sql_query_c *query, MYSQL_RES *res );
      ~sql_result_c();

	  long long n_rows(); 
      sql_row_c fetch_row();

      unsigned int n_fields();
      sql_field_c fetch_field( unsigned int field_idx );

   protected:
      sql_query_c *query;
      MYSQL_RES *res;
};

#endif
