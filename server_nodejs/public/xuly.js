$(document).ready(function() {
	var socket = io();
	socket.emit("join-room");

	socket.on("server-send-list-rooms", function(rooms) {
		rooms.forEach(function(room) {
			var txtID = '#txtSend_'+room.RoomID;
			var btnSendID = '#btnSend_'+room.RoomID;
			var btnResetID = '#btnReset_'+room.RoomID;
			$( btnSendID ).click(function() {
				socket.emit("web-send-mess-to-esp", {RoomID: room.RoomID, Mess: $( txtID ).val()});
			});
			$( btnResetID ).click(function() {
				socket.emit("web-send-reset-room", {RoomID: room.RoomID});
			});
		});
	});

	socket.on("server-send-update-status-room", function(data) {
		if(data.room && data.messes) {
			var htmlStatus="";
			var MessIDs=JSON.parse(data.room.Mess);
			MessIDs.forEach(function(MessID) {
				data.messes.forEach(function(mess) {
					if(MessID == mess.MessID) {
						htmlStatus+='<p>'+mess.MessInFo+'</p>';
					}
				});
			});
			var changeStatusRoom = "#info_" + data.room.RoomID;
			console.log(htmlStatus);
			$(changeStatusRoom).html(htmlStatus);
		}
	});
});