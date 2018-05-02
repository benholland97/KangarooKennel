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
			var $presHtml = '';

			$.ajax({
				url: 'load-data.php',
				type: 'post',
				dataType: 'json',
				data: { Mode: 'kenData', Kennel: i },			//Send params to get different data 
				success: function(data) {
					console.log(data);
					//conditional formatting data 
					$($pres).html($presHtml);
					$($temp).html(data[2]);
					$($humid).html(data[3]);
						
					//only conditional format if dog present
					if (parseFloat(data[4]) == 1) {
						console.log("dog present");
						if (parseInt(data[4]) == 0) {
							$presHtml = "No";
						} else if (parseInt(data[4]) == 1) {
							$presHtml = "Yes";
						}

						//temperature conditional formatting 
						
						var temp = parseFloat(data[2]); 
						
						if (temp >= 27) {
							console.log("exceeded high temp");
							$($temp).css('background-color', '#ff0000');
						}
						else if (temp >= 25 && temp < 27) {
							console.log("warning high temp");
							$($temp).css('background-color', '#FFFF00');
						}
						else if (temp > 12 && temp < 25) {
							console.log("normal temp");
							$($temp).css('background-color', '#ffffff');
						}
						else if (temp <= 12  && temp > 10) {
							console.log("warning low temp");
							$($temp).css('background-color', '#FFFF00');
						}
						else if (temp <= 10) {
							console.log("exceeded low temp");
							$($temp).css('background-color', '#ff0000');
						}

						//humidity conditional formatting 
						
						var humid = parseFloat(data[3]); 
						
						if (humid >= 70) {
							console.log("exceeded high humid");
							$($humid).css('background-color', '#ff0000');
						}
						else if (humid < 70 && humid >= 65) {
							console.log("warning high humid");
							$($humid).css('background-color', '#FFFF00');
						}
						else if (humid < 15 && humid < 65) {
							console.log("normal humid");
							$($humid).css('background-color', '#ffffff');
						}					
						else if (humid <= 15 && humid > 10) {
							console.log("warning low humid");
							$($humid).css('background-color', '#FFFF00');
						}
						else if (humid <= 10) {
							console.log("exceeded low humid");
							$($humid).css('background-color', '#ff0000');
						}
					}
					
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
