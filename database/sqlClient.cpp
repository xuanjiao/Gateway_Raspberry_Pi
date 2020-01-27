#include <stdio.h>
#include <mysql/mysql.h>
#include <string>
#include <stdlib>


#define USER "root"
#define PASSWD "pi"
#define DB_NAME "light_db"
#define RAW_DATA_NUM	10

using namespace std;

class SQLClient{
private:
   MYSQL *mysql;
public:
   void print_version()
   {
      printf("MySQL client version: %s\n",mysql_get_client_info());
   } 
   bool connect(){

      // Initialize the MYSQL structure
      mysql = mysql_init(NULL);

      if( mysql_real_connect(mysql, /* address of existing MYSQL structure */
                        "localhost", /* host name or IP address */
                        USER,    /* my log in ID*/
                        PASSWD,
                        DB_NAME,    /* default database */
                        0,       /* port */
                        NULL,    /* unix socket*/
                        CLIENT_MULTI_STATEMENTS        /* client flag*/
                        ) == NULL)
      {
         printf("Failed to connect to mysql database. Error: %s\n",mysql_error(mysql));
         disconnect();
         return false;
      }
      printf("Connected to sql database. User:%s\n",USER);
      return true;
   }

   void disconnect()
   {
	mysql_close(mysql);
   }

/*
   bool createRawData()
	{
		int status;
		status = mysql_query(mysql,
					"use light_db;\
					drop table if exists light_list;\
					create table light_list(light int, date_time datetime)");
		if(status)
		{
			printf("Error %u : %s",mysql_errno(mysql),mysql_error(mysql));
			return false;
		}

		printf("create raw data completed\n");
		return true;

		char query[1000] *end;
		for(int i = 0; i < RAW_DATA_NUM; i++)
		{	
         // Generate a random light value between 0-100
         int rand_light = rand()%100;
         char* light_str = std::to_string(rand_light).c_str();
         int current_time;

			end = my_stpcpy(query,"insert into light_list(light,date_time) values(");
			

         end += mysql_real_escape_string(&mysql,end,light_str,sizeof(light_str));
         end += my_stpcpy(end,",")
         end += mysql_real_escape_string();
         if( mysql_query(mysql,"insert into light_list(light,date_time) values (rand()*max_light, cur_time)"))
			{
				printf("Error %u : %s",mysql_errno(mysql),mysql_error(mysql));
				return false;
			}		
		}
	} 

	*/

};

int main(){
   SQLClient sql_client;
   sql_client.connect();
   sql_client.createRawData();
   sql_client.disconnect();

    printf("Disconnected\n");
	return 0;
}
