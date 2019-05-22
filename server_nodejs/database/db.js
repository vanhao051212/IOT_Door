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

exports.checkMessExist = function (RoomID, MessID) {
	return new Promise (function (resolve, reject) {
		pool.query("select * from room where RoomID='"+RoomID+"' and MessID='"+MessID+"';", function(err, rows, fields) {
			if (err) reject(err);
			if(rows.length>=1) {
				resolve(true);
			} else {
				resolve(false);
			}
		});
	});
}

exports.checkMessCorrect = function (MessID) {
	return new Promise (function (resolve, reject) {
		pool.query("select * from mess where MessID='"+MessID+"';", function(err, rows, fields) {
			if (err) throw(err);
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
		pool.query("select * from roomlist where RoomID='"+RoomID+"';", function(err, rows, fields) {
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

exports.getListRoom = function () {
	return new Promise (function (resolve, reject) {
		pool.query("select * from roomlist", function(err, rows, fields) {
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
				resolve(rows);
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

exports.insertMess = function (RoomID, MessID, CardID) {
	return new Promise (function (resolve, reject) {
		var d=new Date();
          var dformat = [d.getFullYear(),
                         d.getMonth()+1,
                         d.getDate()].join('-')+' '+
                        [d.getHours(),
                         d.getMinutes(),
                         d.getSeconds()].join(':');
		pool.query("insert into room (RoomID, MessID, CardID, TimeGet) values ('"+RoomID+"','"+MessID+"','"+CardID+"','"+dformat+"')", function(err, result) { 
			if (err) reject(err);
			resolve(true);
		});
	});
};

exports.insertDiemDanh = function(RoomID, CardID) {
	return new Promise (function (resolve, reject) {
		var d=new Date();
          var dformat = [d.getFullYear(),
                         d.getMonth()+1,
                         d.getDate()].join('-')+' '+
                        [d.getHours(),
                         d.getMinutes(),
                         d.getSeconds()].join(':');
		pool.query("insert into diemdanh (RoomID, CardID, TimeGet) values ('"+RoomID+"','"+CardID+"','"+dformat+"')", function(err, result) { 
			if (err) resolve(false);
			resolve(true);
		});
	});
}

exports.deleteAllRoomStatus = function (RoomID) {
	return new Promise (function (resolve, reject) {
		pool.query("delete from room where RoomID='"+RoomID+"'", function(err, rows, fields) {
			if (err) throw(err);
			resolve(true);
		});
	});
}