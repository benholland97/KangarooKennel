var numKennels;

$(document).ready(function() {
	getNumKennels();
	setTable();
	updateTable();
	// setInterval(setTable, 5000);
	setInterval(updateTable, 5000);
});


function setTable() {
	getNumKennels();
	var $table = $('#kennel_table_body');
	var $html = '';
	for (i=1; i < numKennels+1; i++) {
			$html+= "<tr>";
			$html+= 	"<td>"+i+"</td>";
			$html+= 	"<td id='pres"+i+"'>-</td>";
			$html+=   	"<td id='temp"+i+"'>-</td>";
			$html+=   	"<td id='humid"+i+"'>-</td>";
			$html+= "</tr>";
	}
	console.log($html);
	$table.html($html);
}



function updateTable() {
	setTable();
	
	for (let i=1; i< numKennels+1; i++) {
		console.log("iteration:"+i);
		(function(index) {
			var $pres = '#pres'+i;
			var $temp = '#temp'+i;
			var $humid = '#humid'+i;

			$.ajax({
				url: 'load-data.php',
				type: 'post',
				dataType: 'json',
				data: { Mode: 'kenData', Kennel: i },			//Send params to get different data 
				success: function(data) {
					console.log(data);
					//conditional formatting 
					if (parseFloat(data[5]) > 25) {
						console.log("exceeded temp");
						$($temp).css('background-color', '#FFFF00');
					}
					$($pres).html(data[4]);
					$($temp).html(data[2]);
					$($humid).html(data[3]);
						
				}
			});
		})(i);
	}
};






function getNumKennels() {
	$.ajax({
		url: 'load-data.php',
		type: 'post',
		dataType: 'json',
		data: { Mode : 'kenNum' },			//Send params to get different data 
		success: function(data) {
			console.log(data);
			numKennels = parseInt(data[0]) - 1;
				
		}
	});
}
