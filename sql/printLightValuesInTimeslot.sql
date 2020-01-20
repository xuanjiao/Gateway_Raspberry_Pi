
set @hour_min = 12;
set @hour_max = 14;
set @day_of_week = 'Monday';
set @row_number = 0;

-- print all light value measured in time slot on the specific weekday
select (@row_number:=@row_number + 1) as num, light,date_time,dayname(date_time) as day_name
from light_list 
where HOUR(date_time) > @hour_min and HOUR(date_time) < @hour_max and dayname(date_time) = @day_of_week


