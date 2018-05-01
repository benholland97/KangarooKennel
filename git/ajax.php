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


<div class="card mb-3">
        <div class="card-header">
          <i class="fa fa-table"></i> Kennel Statistics</div>
        <div class="card-body">
   <div class="table-responsive">
            <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
              <thead>
                <tr>
                  <th>Kennel ID</th>
                  <th>Occupied</th>
                  <th>Temperature (°C)</th>
                  <th>Humidity (%)</th>
                </tr>
              </thead>
              <tfoot>
                <tr>
                  <th>Kennel ID</th>
                  <th>Occupied</th>
                  <th>Temperature (°C)</th>
                  <th>Humidity (%)</th>
                </tr>
              </tfoot>
              <tbody>
                <tr>
                  <td>1</td>
                  <td>True</td>
                  <td><?=$row['temp']?></td>
                  <td><?=$row['humi']?></td>
                </tr>
                <tr>
                  <td>2</td>
                  <td>False</td>
                  <td>23</td>
                  <td>12</td>
                </tr>
              </tbody>
            </table>
          </div>


        </div>
        <div class="card-footer small text-muted">Updated yesterday at <?=$row['time']?></div>
      </div>
      <p>Click the button to set the color of <var id="myVar">this text</var> to red.</p>

      <button onclick="myFunction()">Try it</button>

      <p>Click the button to set the color of <var id="myVar1">this text</var> to red.</p>

      <button onclick="insertHtml()">Try it too</button>
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

      <script>
      $('tbody tr td:nth-child(3)').each(

      	function() {
      			var	crithigh = 27,
      	     		warnhigh = 25,
      	        warnlow = 12,
      	        critlow = 10,
      	        score = $(this).text();

      	      if (score >= crithigh) {
      	        $(this).addClass('criticalhigh');
      	    }
      	    else if (score < crithigh && score >= warnhigh) {
      	        $(this).addClass('warninghigh');
      	    }
      	   else if (score < warnlow && score >= critlow) {
      	        $(this).addClass('warninglow');
      	    }
      	    else if (score < critlow) {
      	        $(this).addClass('criticallow');
      	    }
      	    });
      </script>
