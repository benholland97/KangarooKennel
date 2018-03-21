<?php

$db_database = 'dogs';
$db_host = 'localhost';
$db_password = 'newuser';
$db_username = 'gdpuser';

$db = mysqli_connect($db_host, $db_username, $db_password, $db_database);

if (mysqli_connect_errno()){
 printf("fail : %s\n", mysqli_connnect_error());
 exit();
}

mysqli_select_db($db_database, $db);

$sql = "SELECT * FROM kennel1 order by time desc;";
$result = mysqli_query($db,$sql);

$row = mysqli_fetch_assoc($result);

?>
