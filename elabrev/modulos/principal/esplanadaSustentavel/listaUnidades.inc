<?PHP
/*
if(!$_POST){
	$texto = "
		<div style=\"font-size:12px\">
			<center><b><font color=red><p>Aviso</p></font></b></center>
			<p>
				<b>Prezado Usu�rio,</b>
				<br><br>
				A melhoria dos nossos servi�os � uma busca cont�nua e para isso, � necess�rio um esfor�o permanente que abrange atualiza��es, reconfigura��es e migra��es que inicialmente podem gerar algum desconforto, mas que a longo prazo s�o indispens�veis para mantermos o nosso compromisso com a qualidade diferenciada.</b>
			</p>
			<p>
				LIBERA��O PREVISTA PARA SEGUNDA-FEIRA 18/03/2013
			</p>
			<b>
				<br> Atenciosamente,
				<br> Equipe <?php echo SIGLA_SISTEMA; ?>.
			</b>		
		</div>
	";
	popupAlertaGeral($texto, '540px',"230px");
}
*/
	require_once '_funcoes_esplanada_sustentavel.php';

	$perfis = array();
	$perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
	
	#Caso n�o seja super usu�rio.
	if( !( $db->testa_superuser() || in_array(PERFIL_CONSULTA_ESPLANADA_SUSTENTAVEL, $perfis) ) ){
		#Conta o n�mero de ung vinculadas ao usu�rio.
		$sql_1 = "
			Select	count(u.unicod) as qtd_unicod
			From elabrev.usuarioresponsabilidade u
			Join public.unidade un on un.unicod = u.unicod
			Where rpustatus = 'A' and u.unicod is not null and usucpf = '".$_SESSION['usucpf']."'
		";
		$ungcods = $db->pegaLinha($sql_1);

		#Tr�s todas as ung vinculadas ao usu�rio.
		$sql_2 = "
			Select	u.unicod as unicod
			From elabrev.usuarioresponsabilidade u
			Join public.unidade un on un.unicod = u.unicod
			Where rpustatus = 'A' and u.unicod is not null and usucpf = '".$_SESSION['usucpf']."'
		";
		$dados = $db->carregarColuna($sql_2);

		$wh  = array();
		$unicod = implode("','", $dados); 
		array_push($wh, " and u.unicod in ('".$unicod."') ");
		$where = implode("", $wh);		
	}else{
		$where = "";
	}

	include  APPRAIZ."includes/cabecalho.inc";
	
	echo "</br>";

	monta_titulo('Projeto Esplanada Sustent�vel', 'Listagem das Unidades Or�ament�rias');
	
?>
	
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script type="text/javascript">
	
		$(document).ready(function(){
			$('.listaUnidadesAlunos').click(function(){
				window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel_alunos&acao=A&unicod='+$(this).attr('id')+'&descricao='+$(this).attr('name');
			});
			$('.listaUnidades').click(function(){
				window.location = 'elabrev.php?modulo=principal/esplanadaSustentavel/esplanada_sustentavel&acao=A&unicod='+$(this).attr('id')+'&descricao='+$(this).attr('name');
			});			
		});
		
	</script>
<?php 
	
	listaUnidades($where);
	
?>

