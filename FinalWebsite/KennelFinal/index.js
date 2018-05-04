var numKennels;


//Function called once initial page load
$(document).ready(function() {
	//get the starting number of kennels in the network
	getNumKennels();
	//set the table of kennels equal to the starting number of kennels in network
	setTable();
	//fill values of each kennel entry in table
	updateTable();
	//refresh page every 5 seconds - to provide 'real-time' response
	setInterval(updateTable, 5000);
});


//function to fill table with entry for each kennel in network
function setTable() {
	//ensure number of kennels always updated to most recent value 
	getNumKennels();
	var $table = $('#kennel_table_body');
	var $html = '';
	//loop through all kennels in network
	for (i=1; i < numKennels+1; i++) {
			//formulate and concatenate html strings representing table entry data for each kennel
			$html+= "<tr>";
			$html+= 	"<td>"+i+"</td>";
			$html+= 	"<td id='pres"+i+"'>-</td>";
			$html+=   	"<td id='temp"+i+"'>-</td>";
			$html+=   	"<td id='humid"+i+"'>-</td>";
			$html+= "</tr>";
	}
	console.log($html);
	//replace old table code with newly formed code
	$table.html($html);
}


//function to update values of all table entries
function updateTable() {
	//ensure most recent version of table present 
	setTable();
	//iterate through all kennels
	for (let i=1; i< numKennels+1; i++) {
		console.log("iteration:"+i);
		(function(index) {
			
			//set placeholder variables equal to corresponding loop 
			var $pres = '#pres'+i;
			var $temp = '#temp'+i;
			var $humid = '#humid'+i;
			var $presHtml = '';
			//asynchronous request for database values for kennel ref passed
			$.ajax({
				url: 'load-data.php',
				type: 'post',
				dataType: 'json',
				data: { Mode: 'kenData', Kennel: i },			//send loop num representing kennel num
				success: function(data) {
					console.log(data);
					
					//test for value of 4th column of returned data representing presence 
					if (parseInt(data[4]) == 0) {
							$presHtml = "No";
					} else if (parseInt(data[4]) == 1) {
							$presHtml = "Yes";
					}
					//set value of table cells equal to returned data 
					$($pres).html($presHtml);
					$($temp).html(data[2]);
					$($humid).html(data[3]);
						
					//only conditional format if dog present
					if (parseFloat(data[4]) == 1) {
						console.log("dog present");

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





//function to return the number of kennels in the network 
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