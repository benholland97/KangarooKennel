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
					
					//only conditional format if dog present
					if (parseFloat(data[1]) == 1) {
						console.log("dog present");
						
						//temperature conditional formatting 				
						if (parseFloat(data[2]) >= 27) {
							console.log("exceeded high temp");
							$($temp).css('background-color', '#ff8080');
						}
						else if (parseFloat(27 > data[2]) >= 25) {
							console.log("warning high temp");
							$($temp).css('background-color', '#ffff99');
						}
						else if (parseFloat(12 > data[2]) > 25) {
							console.log("normal temp");
							$($temp).css('background-color', '#ffffff');
						}
						else if (parseFloat(12 >= data[2]) > 10) {
							console.log("warning low temp");
							$($temp).css('background-color', '#ffff99');
						}
						else if (parseFloat(10 >= data[2])) {
							console.log("exceeded low temp");
							$($temp).css('background-color', '#ff8080');
						}

						//humidity conditional formatting 
						if (parseFloat(data[3]) >= 70) {
							console.log("exceeded high humid");
							$($temp).css('background-color', '#ff8080');
						}
						else if (parseFloat(70 > data[3]) >= 65) {
							console.log("warning high humid");
							$($temp).css('background-color', '#ffff99');
						}
						else if (parseFloat(15 > data[3]) > 65) {
							console.log("normal humid");
							$($temp).css('background-color', '#ffffff');
						}					
						else if (parseFloat(15 >= data[3]) > 10) {
							console.log("warning low humid");
							$($temp).css('background-color', '#ffff99');
						}
						else if (parseFloat(10 >= data[3])) {
							console.log("exceeded low humid");
							$($temp).css('background-color', '#ff8080');
						}
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
