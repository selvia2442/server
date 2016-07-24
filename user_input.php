<!doctype html>
<html>
<head>
<meta charset="utf-8">

<style type="text/css">
#nav {

margin: 0px auto;
padding: 10px;
border:2px solid #999;

font-style:bold;	
line-height:30px;
margin-bottom: 1px;
padding-left: 10px;

}

h1{color:#003;}
h2{color:#00F;
font-size:1.2em;}
fieldset{
	width:70%;
	height:100%:
	border-color:#999;
	
	color:#666;
	background:#FEFEFE;
}
div{
	width:90%;
	height:80%;
	background:#CF3;
	border:thin solid #006 margin:7px 7px;
	font-size:1em;
	padding:10px;
	color:#003;
	font-family:Verdana, Geneva, sans-serif;
	border-bottom:dashed 1px;
	
}
#a{
font-family:바탕체;

color:#390;
}
label{width:200px;
      color:#006;
	  }
</style>


</head>
<body>

<form action="user_write.php" method="post">
 <fieldset>
  <legend> <h2>가입하기</h2></legend>
<div id="nav">

<table width="65%" border="0" cellspacing="1" cellpadding="20">

<?php

echo  "<tr>";
echo   " <td>이용자아이디</td>";
echo    "<td><input type='text'   name='user_id'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>이용자이름</td>";
echo    "<td><input type='text'   name='user_name'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>맞힌갯수</td>";
echo    "<td><input type='text'  name='win_count'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>틀린갯수</td>";
echo    "<td><input type='text'   name='lose_count'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>생성일자</td>";
echo    "<td><input type='text'  name='create_date'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>게임시작시간</td>";
echo    "<td><input type='text' name='app_start_time'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>게임종료시간</td>";
echo    "<td><input type='text' name='app_end_time'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td>최종게임시간</td>";
echo    "<td><input type='text' name='last_play_time'></td>";
echo  "</tr>";
	
?>




</table>
</div>
<center>
<input type="submit" value="  가 입 하 기  " >
  
</fieldset>
</form>



</body>
</html>
