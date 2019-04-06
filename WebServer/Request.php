<?php
	if($_SERVER['REQUEST_METHOD']==='POST'){

		$servername= "localhost";
		$username = "root";
		$password = "";
		$db = "iot_door";

			if(!empty($_POST['Request']) && !empty($_POST['IDPhong']) ){
			$req =$_POST['Request'];
			$ID_Phong=$_POST['IDPhong'];

			$conn= new mysqli("localhost", $username, $password, $db );
			if ($conn->connect_error) {
				die("Connection failed: " . $conn->connect_error);
			}



			//UPDATE `phong` SET `WATER` = 'ON' WHERE `phong`.`IDPhong` = 'E6.1'
			$sql = "UPDATE phong SET $req= 'ON' where IDPhong= '$ID_Phong' ";
			 
			if ($conn->query($sql) === TRUE) {
    			echo "Record updated successfully";
			} else {
		    echo "Error updating record: " . $conn->error;
			}

			
			
			$conn->close();
			

		 }



			
	}







 ?>