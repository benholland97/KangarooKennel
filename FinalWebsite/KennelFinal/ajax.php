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

<style type="text/css">
 td.color {
color: red;
}
</style>

<?=$row['temp']?>


   <!--   <p>Click the button to set the color of <var id="myVar">this text</var> to red.</p>

      <button onclick="myFunction()">Try it</button>

      <p>Click the button to set the color of <var id="myVar1">this text</var> to red.</p>

      <button onclick="insertHtml(myVar1,html)">Try it too</button> -->
      <script>
      function insertHtml(id, html)
      {
         var ele = document.getElementById(id);
         ele.innerHTML = html;
         var codes = ele.getElementsByTagName("script");
         for(var i=0;i<codes.length;i++)
         {
             eval(codes[i].text);
         }
      }

function myFunction() {
    var x = document.getElementById("myVar");
    x.style.color = "red";
}
</script>

    

