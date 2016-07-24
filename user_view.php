<!doctype html>
<html>
<head>
<meta charset="utf-8">

<style type="text/css">
#nav {

margin: 0px auto;
padding: 10px;
border:2px solid #999;

font-size:1.2em ;
font-style:bold;	
line-height:20px;
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
	border:thin solid #006 margin:10px 10px;
	font-size:1em;
	padding:10px;
	color:#003;
	
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
<form action="user_post.php" method="post">
 <fieldset>
  <legend> <h2> 회원 정보 자세히 보기</h2></legend>
<div id="nav">
<table width="80%"border="0" cellspacing="1" cellpadding="20">

<?php
include 'mysql_selvia.php';

$user_idx=$_GET['user_idx'];

$sqlrec   = "select * from  t_user where user_idx='$user_idx' ";

$info     = mysql_query($sqlrec) or die("쿼리실패 : ".mysql_error());


if($val=mysql_fetch_array($info))
{ 
	
	$user_id  =$val['user_id'];
	$user_name=$val['user_name'];
	$user_img=$val['user_img'];
	$win_count=$val['win_count'];
	$lose_count=$val['lose_count'];
	$create_date=$val['create_date'];
	$app_start_time=$val['app_start_time'];
	$app_end_time=$val['app_end_time'];
	$last_play_time=$val['last_play_time'];


}

?>


<?php

echo  "<tr >";
echo   " <td width=25%'>이용자아이디</td>";
echo    "<td><input type='text'  name='user_id' value='$user_id' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>이용자이름</td>";
echo    "<td><input type='text'  name='user_name' value='$user_name' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>이미지경로</td>";
echo    "<td><input type='text'  name='user_img' value='$user_img' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>맞힌갯수</td>";
echo    "<td><input type='text'  name='win_count' value='$win_count' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>틀린갯수</td>";
echo    "<td><input type='text'  name='lose_count' value='$lose_count' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>생성일자</td>";
echo    "<td><input type='text'  name='create_date' value='$create_date' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>게임시작시간</td>";
echo    "<td><input type='text'  name='app_start_time' value='$app_start_time' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>게임종료시간</td>";
echo    "<td><input type='text'  name='app_end_time' value='$app_end_time' style='width:100%'></td>";
echo  "</tr>";
echo  "<tr>";
echo   " <td width=25%'>최종게임시간</td>";
echo    "<td><input type='text'  name='last_play_time' value='$last_play_time' style='width:100%'></td>";
echo  "</tr>";

?> 

</table>
</div><center>
<input type="submit" value=" Update" >
  
</fieldset>
</form>

</body>
</html>
