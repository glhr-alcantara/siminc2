<?php

// inicia sistema
include 'config.inc';
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

# Verifica se a sess�o n�o expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

?>

<html>
	<head>
		<title>Gr�ficos</title>
	</head>
	<body>
		<br/>
		<p style="text-align:center;">
			<img src="/geral/graficoImagem.php?tipo=usuario_hora"/>
		</p>
		<br/><br/>
		<p style="text-align:center;">
			<img src="/geral/graficoImagem.php?tipo=hits_hora"/>
		</p>
		<br/><br/>
		<p style="text-align:center;">
			<img src="/geral/graficoImagem.php?tipo=usuario_ano_mes"/>
		</p>
	</body>
</html>