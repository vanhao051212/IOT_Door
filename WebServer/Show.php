


 <html>
    <head>
	<meta http-equiv="refresh" content="10" > 
    </head>
    <body>
    <?php 


		$servername= "localhost";
		$username = "root";
		$password = "";
		$db = "iot_door";
		$conn= new mysqli("localhost", $username, $password, $db );
		if ($conn->connect_error) {
			die("Connection failed: " . $conn->connect_error);
		}

		$sql = "SELECT * from phong where MIC='ON' or REMOTE='ON' or WATER='ON' or TIME_UP='ON' ";
		$res = $conn->query($sql);
		if ($res->num_rows > 0) {
		// output data of each row
		    while($row = $res->fetch_assoc()) {
		        echo "Phong " . $row["IDPhong"]. ":    MIC: " . $row["MIC"]. " - REMOTE: " . $row["REMOTE"]. "-WATER " . $row["WATER"]." - TIME_UP: " . $row["TIME_UP"]. "<br>";
		    }
		} else {
		    echo "0 results";
		}
		
		$conn->close();

 	?>
    </body>
</html>