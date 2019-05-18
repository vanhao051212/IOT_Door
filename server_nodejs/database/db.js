var mysql      = require('mysql');
	
var pool = mysql.createPool({
    connectionLimit: 5,
    host     : 'localhost',
  	user: 'root', 
    password: 'qwerty123456789',
    database: 'csvc'
});

exports.checkRFID = function (ID) {
	return new Promise (function (resolve, reject) {
		pool.query("select * from gv where ID='"+ID+"';", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				resolve(true);
			} else {
				resolve(false);
			}
		});
	});
}

exports.checkMessExist = function (RoomID, Mess) {
	return new Promise (function (resolve, reject) {
		pool.query("select * from room where RoomID='"+RoomID+"';", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				var message = JSON.parse(rows[0].Mess);
				if(message.indexOf(Mess) >=0 ) {
					resolve(true);
				} else {
					resolve(false);
				}
			} else {
				resolve(false);
			}
		});
	});
}

exports.checkMessCorrect = function (MessID) {
	return new Promise (function (resolve, reject) {
		pool.query("select * from mess where MessID='"+MessID+"';", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				resolve(true);
			} else {
				resolve(false);
			}
		});
	});
}

exports.checkRoomExist = function (RoomID) {
	return new Promise (function (resolve, reject) {
		pool.query("select * from room where RoomID='"+RoomID+"';", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				resolve(true);
			} else {
				resolve(false);
			}
		});
	});
}

exports.getRoomInfo = function () {
	return new Promise (function (resolve, reject) {
		pool.query("select * from room;", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				resolve(rows);
			} else {
				resolve(false);
			}
		});
	});
}

exports.getSpecRoomInfo = function (RoomID) {
	return new Promise (function (resolve, reject) {
		pool.query("select * from room where RoomID='"+RoomID+"';", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				resolve(rows[0]);
			} else {
				resolve(false);
			}
		});
	});
}

exports.getMessInfo = function () {
	return new Promise (function (resolve, reject) {
		pool.query("select * from mess;", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				resolve(rows);
			} else {
				resolve(false);
			}
		});
	});
}

exports.updateMess = function (RoomID, Mess) {
	return new Promise (function (resolve, reject) {
		pool.query("update room set Mess='"+Mess+"' where RoomID='"+RoomID+"'", function(err, result) { 
			if (err) reject(err);
			resolve(true);
		});
	});
};

exports.insertDiemDanh = function(RoomID, CardID) {
	return new Promise (function (resolve, reject) {
		var today = new Date();
		var dd = today.getDate();
		var mm = today.getMonth() + 1; //January is 0!

		var yyyy = today.getFullYear();
		if (dd < 10) {
		  dd = '0' + dd;
		} 
		if (mm < 10) {
		  mm = '0' + mm;
		} 
		var today = dd + '/' + mm + '/' + yyyy;
		pool.query("insert into diemdanh (RoomID, CardID, TimeGet) values ('"+RoomID+"','"+CardID+"','"+today+"')", function(err, result) { 
			if (err) resolve(false);
			resolve(true);
		});
	}
}