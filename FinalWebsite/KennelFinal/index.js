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

// Temperature Conditional Formatting
$('tbody tr td:nth-child(3)').each(

	function() {
			var	crithigh = 27,
	     			warnhigh = 25,
	        		warnlow = 12,
	        		critlow = 10,
	        		score = $(this).text();

	      if (score >= crithigh) {
	        $(this).toggleClass('criticalhigh');
	    }
	    else if (score < crithigh && score >= warnhigh) {
	        $(this).toggleClass('warninghigh');
	    }
	   else if (score < warnlow && score >= critlow) {
	        $(this).toggleClass('warninglow');
	    }
	    else if (score < critlow) {
	        $(this).toggleClass('criticallow');
	    }
	    });

			// Humidity Conditional Formatting
			$('tbody tr td:nth-child(4)').each(

				function() {
						var	crithigh = 70,
				     		warnhigh = 65,
				        warnlow = 15,
				        critlow = 10,
				        score = $(this).text();

				      if (score >= crithigh) {
				        $(this).toggleClass('criticalhigh');
				    }
				    else if (score < crithigh && score >= warnhigh) {
				        $(this).toggleClass('warninghigh');
				    }
				   else if (score < warnlow && score >= critlow) {
				        $(this).toggleClass('warninglow');
				    }
				    else if (score < critlow) {
				        $(this).toggleClass('criticallow');
				    }
				    });

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
