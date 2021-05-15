function AutoRefresh( t ) {
	setTimeout("location.reload(true);", t);
}
$(document).ready(function() {
	var socket = io();
	var x = document.getElementById("myAudio");
	x.pause();
	var area = sessionStorage.getItem("area");
	if(area) {
		$("#selectArea").val(area);
		socket.emit("join-room",area);
	} else {
		sessionStorage.setItem("area", $("#selectArea").val());
		area=$("#selectArea").val();
		socket.emit("join-room",area);
	}
	$("#selectArea").change(function() {
		sessionStorage.setItem("area", $("#selectArea").val());
		area=$("#selectArea").val();
		socket.emit("join-room",area);
	});
	socket.on("server-send-list-rooms", function(data) {
		var html="";
		data.roomsList.forEach(function(room) {
			var htmlStatus = "";
			if(data.roomsInfo) {
				data.roomsInfo.forEach(function(roomInfo) {
					if(roomInfo.RoomID == room.RoomID) {
						data.messesInfo.forEach(function(mess) {
							if(roomInfo.MessID == mess.MessID) {
								if(roomInfo.MessID == 'M05') {
									htmlStatus+='<p style = "background-color:#ffa366; color:white; font-size: 20px;">'+mess.MessInFo+'</p>';
									x.pause();
									x.play();
								} else 
									htmlStatus+='<p>'+mess.MessInFo+'</p>';
							}
						});
					}
				});
			}
			html+='<div id="'+room.RoomID+'" class="room">'
				+'<h2>'+room.RoomName+'</h2>'
				+'<div id="info_'+room.RoomID+'">'+htmlStatus+'</div>'
				+'<input type="text" class="txtMessage" id="txtSend_'+room.RoomID+'">'
				+'<button type="button" class="btnSend" id="btnSend_'+room.RoomID+'">Send</button><br><br><br>'
				+'<button type="button" class="btnReset" id="btnReset_'+room.RoomID+'">Reset</button>'
				+'</div>';
		});

		$("#wrapper").html(html);
		data.roomsList.forEach(function(room) {
			var txtID = '#txtSend_'+room.RoomID;
			var btnSendID = '#btnSend_'+room.RoomID;
			var btnResetID = '#btnReset_'+room.RoomID;
			$( btnSendID ).click(function() {
				socket.emit("web-send-mess-to-esp", {RoomID: room.RoomID, Mess: $( txtID ).val()});
			});
			$( btnResetID ).click(function() {
				$("#"+room.RoomID).hide();
				socket.emit("web-send-reset-room", {RoomID: room.RoomID, Area: $("#selectArea").val()});
			});
		});

		data.roomsList.forEach(function(room) {
			var count = 0;
			var htmlStatus = "";
			if(data.roomsInfo) {
				data.roomsInfo.forEach(function(roomInfo) {
					if(roomInfo.RoomID == room.RoomID) {
						count++;
					}
				});
			}
			if(count == 0) {
				$("#"+room.RoomID).hide();
			}
		});
	});

	socket.on("server-send-update-status-room", function(data) {
		if(data.room && data.messes) {
			var htmlStatus="";
			var MessIDs=[];
			data.room.forEach(function(roomInfo) {
				MessIDs.push(roomInfo.MessID);
			});
			MessIDs.forEach(function(MessID) {
				data.messes.forEach(function(mess) {
					if(MessID == mess.MessID) {
						if(MessID == 'M05') {
							htmlStatus+='<p style = "background-color:#ffa366; color:white; font-size: 20px;">'+mess.MessInFo+'</p>';
							x.pause();
							x.play();
						} else
							htmlStatus+='<p>'+mess.MessInFo+'</p>';
					}
				});
			});
			var changeStatusRoom = "#info_" + data.room[0].RoomID;
			$(changeStatusRoom).html(htmlStatus);
			$("#"+data.room[0].RoomID).show();
		} else if(!data.room && data.RoomID) {
			var changeStatusRoom = "#info_" + data.RoomID;
			$(changeStatusRoom).html("");
			$("#"+data.RoomID).hide();
		}
	});
});