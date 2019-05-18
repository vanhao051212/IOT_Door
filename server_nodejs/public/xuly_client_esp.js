$(document).ready(function() {
	var socket = io();
	$( "#btnSend" ).click(function() {
		socket.emit("esp-send-mess", $( "#txtMess" ).val());
	});
	$( "#btnPlay" ).click(function() {
		var x = document.getElementById("myAudio"); 
		x.play(); 
	});
});