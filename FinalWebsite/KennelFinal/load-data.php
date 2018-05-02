<?php
	$db = 'kennel_data';
	$host = 'localhost';
	$pass = '123';
	$user = 'test';
	
	$mode = $_REQUEST['Mode'];
	
	$con = mysqli_connect($host, $user, $pass, $db);
	
	if (mysqli_connect_errno()){
		echo mysqli_connnect_error();
		exit();
	}
	
	mysqli_select_db($db, $con);
	
	if ($mode == kenNum) {
		mysqli_select_db('information_schema', $con);
		$sql = "SELECT COUNT(*) FROM information_schema.tables WHERE table_schema = 'kennel_data' ";
	}
	else if ($mode == kenData) {
		$num = $_REQUEST['Kennel'];
		$sql = "SELECT * FROM kennel".$num." ORDER BY timestamp DESC";
	}

	$result = mysqli_query($con,$sql);

	$row = mysqli_fetch_row($result);

	// echo $row;
	
	mysqli_close($con);
	
	echo json_encode($row);	
	

?>