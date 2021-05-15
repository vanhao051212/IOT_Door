$(document).ready(function() {
	var socket = io();
	$( "#btnSend" ).click(function() {
		socket.emit("esp-send-mess", {RoomID: $( "#RoomID" ).val(), CardID: $( "#CardID" ).val(), MessID: $( "#MessID" ).val()});
	});
});