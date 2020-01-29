#include <stdio.h>
#include <mysql/mysql.h>
#include <string>
#include <stdlib.h>


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

   bool print_light_value()
   {
      char query[1000] = "";
      int hour_min = 8, hour_max = 10, n, ret;
	char day_of_week[] = "Tuesday";
      unsigned int num_fields;
      MYSQL_RES *result;
      MYSQL_ROW row;
      
      if( (n = sprintf(query,"select light,date_time,dayname(date_time) as day_name from light_list where HOUR(date_time) >= %d and HOUR(date_time) < %d and dayname(date_time)='%s'",hour_min,hour_max,day_of_week))<0)
	//if( (n = sprintf(query,"select light,date_time,dayname(date_time) as day_name from light_list",hour_min,hour_max))<0)
      {
         printf("Failed to escape string.\n");
         return false;
      }

      if( (ret = mysql_query(mysql, query)) != 0)
      {
         printf("%s\n",query);
			printf("Error %u : %s\n",mysql_errno(mysql),mysql_error(mysql));
			return false;   
      }

      if( (result = mysql_store_result(mysql)) == NULL)
      {
         // No result or error
         printf("Error %u : %s\n",mysql_errno(mysql),mysql_error(mysql));
			return false;  
      }

      printf("result has %llu row, ",mysql_num_rows(result));
      

      // Get number of columns in a result set.
      num_fields = mysql_num_fields(result);
      printf("%d colums\n",num_fields);
   
      // sets the filed cursor to the beginning of a row, the beginning of a column
	//printf("privious row cursor %d\n",mysql_row_seek(result,0));
      //printf("previous value of cursor %d\n",mysql_field_seek(result,0));


	printf("Light |  Date time        | Day of Week\n");

      while((row = mysql_fetch_row(result)))
      {
         //printf("find row \n");
         // Get the lenghts of the colums in the current row.
         unsigned long *lengths = mysql_fetch_lengths(result);
         for(int i = 0; i < num_fields;i++){
 		//printf("[%.*s]",lengths[i],row[i]);
		printf("[%4s]",row[i]);
         }
         printf("\n");
      }
      
 /*
      str = std::to_string(hour_min).c_str;
      end = my_stpcpy(query,"select (light,date_time,dayname(date_time) as day_name from light_list where HOUR(date_time) >= @hour_min and HOUR(date_time) >= ");
      end += mysql_real_escape_string(mysql,end,sizeof(str));
      end 
      if( mysql_query(mysql,"select (light,date_time,dayname(date_time) as day_name from light_list 
where HOUR(date_time) >= @hour_min and HOUR(date_time) < @hour_max and dayname(date_time) = @day_of_week") != 0)
      {

      }
      */
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
   //sql_client.createRawData();
   sql_client.print_light_value();
   sql_client.disconnect();

    printf("Disconnected\n");
	return 0;
}
