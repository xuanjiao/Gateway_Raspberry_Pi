to compile main.cpp please use

```
gcc -o simplescan main.cpp -lbluetooth
./simplescan
```

Finished:

1. install and set up mysql database
    1. download mysql server 

    ```
    sudo apt-get update
    sudo apt-get install mysql-server
    ```
    2. start mysql service, set it launch at reboot
    ```
    sudo systemctl start mysql
    sudo systemctl enable mysql
    ```
    3. start the mysql shell and enter it as root user
    ```
    mysql -u root -p
    ```

    4. disable the only_full_group_by setting
    ```
    mysql> set global sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION';

    set session sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION';

    ```
    5. write sql script `createRawData.sql` which create a database, a table with random light value and timestamp. use `mysql source [path to script file]`to run it. In my PC run

    ```
    mysql> source ~/ble_gateway/sql/createRawData.sql;
    ```

    result is:

    ![light_list](./images/screenshot_light_list.png)

    6. write sql script `printLightValuesInTimeslot.sql` which find out light value for a specific timeslot. run

    ```
    mysql> source ~/ble_gateway/sql/printLightValuesInTimeslot.sql;
    ```
    result is:

    ![print_light_value](./images/screenshot_print_light_value_in_timeslot.png)

    7. write sql script `calculateLightOnPercent.sql`, which calculate light on possibility for timeslots.

    ```
    mysql> source ~/ble_gateway/sql/calculateLightOnPercent.sql;
    ```
    result is:

    ![light_on_percentage](./images/screenshot_calculateLightOnPercent.png)

    2. implement ble gatt client class which can connect to ble gatt server(develop board), and read light value.

    result is:

    ![gatt_]

    To Do:

    2. add functions to acess mysql database

