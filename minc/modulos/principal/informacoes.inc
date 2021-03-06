<?php 

$_SESSION['minc']['entid'] = $_SESSION['minc']['entid'] ? $_SESSION['minc']['entid'] : $_GET['entid'];
$_SESSION['minc']['mceid'] = $_SESSION['minc']['mceid'] ? $_SESSION['minc']['mceid'] : $_GET['mceid'];
unset($_SESSION['abamenu']);
if(!empty($_SESSION['minc']['mceid'])){
    $sql = "SELECT parid, ent.entid, entnumcpfcnpj
                FROM minc.mceparceiro mce
                LEFT JOIN entidade.entidade ent ON ent.entid = mce.entid
                WHERE parstatus='A'
                and mceid = {$_SESSION['minc']['mceid']}";
    $rs = $db->pegaLinha($sql);
}

if(empty($_REQUEST['campo_tipo_entidade'])){

    if(strlen($rs['entnumcpfcnpj']) > 11 ){
        $_REQUEST['entid'] = $rs['entid'];
        $_REQUEST['campo_tipo_entidade'] = 2;
    }

    if(strlen($rs['entnumcpfcnpj']) == 11){

        $_REQUEST['entid'] = $rs['entid'];
        $_REQUEST['campo_tipo_entidade'] = 1;
    }
}
$campo_tipo_entidade = ($_REQUEST['campo_tipo_entidade']) ? $_REQUEST['campo_tipo_entidade'] : 1;
if($_GET['entid']){
    $sql = "SELECT entnumcpfcnpj FROM entidade.entidade WHERE entid = {$_GET['entid']}";
    $numcpfcnpj = $db->pegaUm($sql);
    if(strlen($numcpfcnpj) > 11){
        $campo_tipo_entidade = 2;
    }
}
$_SESSION['avaliacao']['entidade'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $rs['entid'];
$_SESSION['avaliacao']['tipo_entidade'] = $campo_tipo_entidade;
// ver($_SESSION);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";



$sqlDocumento = "SELECT docid FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsDocumento = !empty($_SESSION['minc']['mceid']) ? $db->pegaLinha($sqlDocumento) : array();

$docid = $rsDocumento['docid'];
$arMnuid = array();
if($docid){
    $sqlDocumento = "select * from workflow.documento where docid = {$docid}";
    $resultDocumento = $db->pegaLinha($sqlDocumento);
  	if ($resultDocumento && !in_array($resultDocumento['esdid'], array(/*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */ESTADO_DOCUMENTO_ENVIADO_FNDE/*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))) {
        $arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
    }
}else{
	$arMnuid = array( MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
}
$db->cria_aba($abacod_tela, $url,'' , $arMnuid);

$titulo = "Informa��es";
$dsctitulo = '';

monta_titulo( $titulo, $dsctitulo );
echo "<script>
		alert('Prezados(as),

				Informamos que o prazo para o preenchimento do Monitoramento do Programa Mais Cultura nas Escolas foi prorrogado, ainda sem data definida.
				A aba monitoramento permanecer� aberta para constantes atualiza��es.
		');
	</script>";
?>

<table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2">
	<tr bgcolor="#e7e7e7">
	  <td width="5%">&nbsp;</td>
	  <td align="left" style="font-size: 13px">
	  	<br/>
	  	<p><b>O MAIS CULTURA NAS ESCOLAS</b> � uma iniciativa interministerial firmada entre os Minist�rios da Cultura e da Educa��o, que tem por finalidade 
	  	fomentar a��es que promovam o <b>encontro entre experi�ncias culturais e art�sticas</b> em curso na comunidade local <b>e o projeto pedag�gico de escolas 
	  	p�blicas</b> contempladas com os Programas Mais Educa��o e Ensino M�dio Inovador em 2011.</p>

		<p>Esta ser� uma a��o conjunta entre as escolas, artistas e/ou entidades culturais e pontos de cultura com o objetivo de <b>aproximar as pr�ticas culturais, 
		do fazer pedag�gico das escolas.</b></p> 

		<p>As atividades contempladas dever�o desenvolver processos art�sticos e culturais cont�nuos, podendo ser realizadas dentro do espa�o escolar, de 
		acordo com a disponibilidade da escola, ou fora dele, sob orienta��o pedag�gica da escola. Dever�o contribuir para a promo��o e reconhecimento 
		de <b>territ�rios educativos</b>, valorizando o di�logo entre saberes comunit�rios e escolares. Devem tamb�m procurar integrar espa�os escolares com 
		espa�os culturais diversos, equipamentos p�blicos, centros culturais, bibliotecas p�blicas, pontos de cultura, pra�as, parques, museus e cinemas.</p>
	  	<br/>
	  	<br/>
	  	<a href="http://simec.mec.gov.br/minc/arquivos/manual.pdf" target="_new">clique aqui para ver o MANUAL com orienta��es para o cadastramento do Plano de Atividade Cultural 2013, elaborado em conjunto com uma iniciativa cultural parceira</a>
	  </td>
	  <td width="5%">&nbsp;</td>
	</tr>
	<tr>
		<td colspan="3" bgcolor="#e7e7e7" height="30">&nbsp;</td>
	</tr>	
</table>