create database if not exists light_db character set utf8 collate utf8_general_ci;
use light_db;
drop table if exists light_list;
create table light_list(
	light int,
    date_time datetime
    );

drop procedure if exists createRawData;

delimiter $$
create procedure createRawData(
	start_time datetime,
    period time,
    max_light int,
    cnt_total int
)
begin    	
	declare cnt int default 0;
    declare cur_time datetime;
    set cur_time = start_time;
   
	while cnt < cnt_total do
		insert into light_list(light,date_time) values (rand()*max_light, cur_time);
		set cur_time = addtime(cur_time,period);
        set cnt = cnt + 1;
    end while;
end $$
delimiter ;

-- create raw data with datetime and light value, store it in light list.
call createRawData('2020-01-20 07:23:40','00:10:00',255,500);

select * from light_list;