<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script>

function imprimirDocumento() {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: 'requisicao=alterarSituacaoLiberacao&lbrsituacao=A',
			asynchronous: false,
			onComplete: function(resp) {},
			onLoading: function(){}
		});
		
	window.print();
}

</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="right" class="notprint"><img src="../imagens/ico_print.jpg" border="0" style="cursor:pointer;" onClick="imprimirDocumento();"></p>
<table width="100%" border="0" cellpadding="0" cellspacing="0" style="border-bottom: 1px solid;">
	<tr bgcolor="#ffffff"> 
		<td><img src="../imagens/brasao.gif" width="50" height="50" border="0"></td>
		<td height="20" nowrap>
			<?php echo NOME_SISTEMA; ?><br>
			Minist�rio da Educa��o / SE - Secretaria Executiva<br>
			DTI - Diretoria de Tecnologia da Informa��o<br>	
		</td>
		<td height="20" align="right">
			Impresso por: <strong>ALEXANDRE DOURADO</strong><br>
			Hora da Impress�o: 29/10/2009 - 10:41:00</td>
	</tr>
	<tr bgcolor="#ffffff"> 
		<td colspan="2">&nbsp;</td>
	</tr>
</table>
<?
$liberacao = $db->pegaLinha("SELECT libnumeroliberacao, to_char(libdataliberacao, 'YYYY-mm-dd') as libdataliberacao, to_char(libdataliberacao, 'YYYY') as libanoliberacao FROM elabrev.liberacao WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'");
?>
<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">Libera��o Or�ament�ria N�</td>
	<td><? echo $liberacao['libnumeroliberacao']."/".$liberacao['libanoliberacao']; ?></td>
</tr>
</table>
<?php

define("FNL_COORDENACAO", 1);
define("FNL_SUBSECRETARIO", 2);
	
// c�digos das Unidades Gestoras
$filtro[] = " entid IN('391295','391282','391296','391283','391284','391285','391286','391287','391288','391289','391290','391291','391292')";

/*
 * OBSERVA��ES:
 * A COLUNA GEST�O SEMPRE SER� 00001 (DEVER� DEIXA FIXO) DE ACORDO COM HENRIQUE XAVIER - 28/10/2009 
 */

/*
$sql = "SELECT uni.unicod || ' - ' || uni.unidsc as unidade,
			   ent.entungcod || ' - ' || ent.entnome as unidadegestora,
			   '<center>00001</center>' as gestao, 
			   '<center>' || lbf.foscod || '</center>' as fontedetalhada, 
			   '<center>' || res.ptres || '</center>' as ptres,
			   '<center>' || lbp.plicod || '</center>' as plicod,
			   pli.plidsc as descricao,
			   nat.ndpcod || ' - ' || nat.ndpdsc as natureza,
			   aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc as acao,
			   COALESCE((SELECT trim(to_char(lbavalor, '999g999g999g999d99')) FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') as valor,
			   COALESCE((SELECT lbaobservacao FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'</textarea>' as observacao,
			   COALESCE((SELECT lbanc FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') as nc
		FROM elabrev.liberacao      lbr 
		INNER JOIN elabrev.liberacaodetalhe  lbd ON lbd.lbrid=lbr.lbrid 
		INNER JOIN entidade.entidade         ent ON ent.entid=lbd.entid 
		INNER JOIN public.unidade            uni ON uni.unicod=ent.entunicod
		INNER JOIN elabrev.liberacaosubacao  lbs ON lbs.lbdid=lbd.lbdid 
		INNER JOIN elabrev.liberacaopi       lbp ON lbp.lbsid=lbs.lbsid 
		INNER JOIN monitora.planointerno     pli ON pli.plicod=lbp.plicod 
		INNER JOIN elabrev.liberacaoptres    lpt ON lpt.lbpid=lbp.lbpid 
		INNER JOIN monitora.ptres            res ON res.ptrid=lpt.ptrid
		INNER JOIN monitora.acao             aca ON aca.acaid=res.acaid
		INNER JOIN elabrev.liberacaofonte    lbf ON lbf.lptid=lpt.lptid 
		INNER JOIN elabrev.liberacaonatureza lbn ON lbn.lbfid=lbf.lbfid 
		INNER JOIN public.naturezadespesa    nat ON nat.ndpid=lbn.ndpid
		WHERE lbr.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND 
		      lbr.lbrstatus='A' AND 
      		  lbd.lidstatus='A' AND
      		  lbs.lbsstatus='A' AND 
      		  lbp.lbpstatus='A' AND 
      		  lpt.lptstatus='A' AND 
      		  lbf.lbfstatus='A' AND 
      		  lbn.lbnstatus='A' 
		GROUP BY uni.unicod, 
				 uni.unidsc, 
				 ent.entungcod, 
				 ent.entnome, 
				 lbf.foscod, 
				 res.ptres, 
				 lbp.plicod,
				 pli.plidsc,
				 nat.ndpcod,
				 nat.ndpdsc,
				 aca.acadsc,
				 aca.prgcod, 
				 aca.acacod, 
				 aca.unicod, 
				 aca.loccod,
				 lbr.lbrid,
				 lbd.lbdid,
				 lbs.lbsid,
				 lbp.lbpid,
				 lpt.lptid,
				 lbf.lbfid,
				 lbn.lbnid
			ORDER BY 
				 lbr.lbrid,
				 lbd.lbdid,
				 lbs.lbsid,
				 lbp.lbpid,
				 lpt.lptid,
				 lbf.lbfid,
				 lbn.lbnid
			";
*/

$sql = "SELECT --uni.unicod || ' - ' || uni.unidsc as unidade,
			   --ent.entungcod || ' - ' || ent.entnome as unidadegestora,
			   --'<center>00001</center>' as gestao, 
			   --'<center>' || lbf.foscod || '</center>' as fontedetalhada, 
			   aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc as acao,
			   '<center>' || res.ptres || '</center>' as ptres,
			   lbp.plicod || ' - ' || pli.plititulo as plicod,
			   --pli.plidsc as descricao,
			   nat.ndpcod || ' - ' || nat.ndpdsc as natureza,
			   COALESCE((SELECT trim(to_char(lbavalor, '999g999g999g999d99')) FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') as valor
			   --COALESCE((SELECT lbaobservacao FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'</textarea>' as observacao,
			   --COALESCE((SELECT lbanc FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') as nc
		FROM elabrev.liberacao      lbr 
		INNER JOIN elabrev.liberacaodetalhe  lbd ON lbd.lbrid=lbr.lbrid 
		INNER JOIN entidade.entidade         ent ON ent.entid=lbd.entid 
		INNER JOIN public.unidade            uni ON uni.unicod=ent.entunicod
		INNER JOIN elabrev.liberacaosubacao  lbs ON lbs.lbdid=lbd.lbdid 
		INNER JOIN elabrev.liberacaopi       lbp ON lbp.lbsid=lbs.lbsid 
		INNER JOIN monitora.planointerno     pli ON pli.plicod=lbp.plicod 
		INNER JOIN elabrev.liberacaoptres    lpt ON lpt.lbpid=lbp.lbpid 
		INNER JOIN monitora.ptres            res ON res.ptrid=lpt.ptrid
		INNER JOIN monitora.acao             aca ON aca.acaid=res.acaid
		INNER JOIN elabrev.liberacaofonte    lbf ON lbf.lptid=lpt.lptid 
		INNER JOIN elabrev.liberacaonatureza lbn ON lbn.lbfid=lbf.lbfid 
		INNER JOIN public.naturezadespesa    nat ON nat.ndpid=lbn.ndpid
		WHERE lbr.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND 
		      lbr.lbrstatus='A' AND 
      		  lbd.lidstatus='A' AND
      		  lbs.lbsstatus='A' AND 
      		  lbp.lbpstatus='A' AND 
      		  lpt.lptstatus='A' AND 
      		  lbf.lbfstatus='A' AND 
      		  lbn.lbnstatus='A' 
		GROUP BY --uni.unicod, 
				 --uni.unidsc, 
				 --ent.entungcod, 
				 --ent.entnome, 
				 --lbf.foscod, 
				 res.ptres, 
				 lbp.plicod,
				 pli.plititulo,
				 pli.plidsc,
				 nat.ndpcod,
				 nat.ndpdsc,
				 aca.acadsc,
				 aca.prgcod, 
				 aca.acacod, 
				 aca.unicod, 
				 aca.loccod,
				 lbr.lbrid,
				 lbd.lbdid,
				 lbs.lbsid,
				 lbp.lbpid,
				 lpt.lptid,
				 lbf.lbfid,
				 lbn.lbnid
			ORDER BY 
				 lbr.lbrid,
				 lbd.lbdid,
				 lbs.lbsid,
				 lbp.lbpid,
				 lpt.lptid,
				 lbf.lbfid,
				 lbn.lbnid
			";

//$cabecalho = array("C�digo Unidade", "UG", "Gest�o", "Fonte Detalhada", "PTRES", "PI", "Descri��o", "Natureza da despesa", "A��o", "Valor(R$)", "Observa��o", "NC");

$cabecalho = array("A��o", "PTRES", "Plano Interno", "Natureza da despesa", "Valor(R$)");
$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);

?>
<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr bgcolor="#C0C0C0">
	<td>
	<strong>A<br/>
	COORDENA��O GERAL DE OR�AMENTO<br/>
	Para autorizar a libera��o or�ament�ria</strong>
	</td>
	<td>
	<strong>Ao<br/>
	SUBSECRET�RIO DE PLANEJAMENTO E OR�AMENTO<br/>
	Para autorizar a libera��o or�ament�ria<br/></strong>
	Bras�lia-DF, 
	<?
	$libdataliberacao = $liberacao['libdataliberacao'];
	echo (($libdataliberacao)?$libdataliberacao:"N�o informado"); 
	?>

	</td>
</tr>
<tr bgcolor="#C0C0C0">
	<td align="center">
	<strong>
	<?
	$usunome_coo = $db->pegaUm("SELECT lr.lrpimgassinatura FROM elabrev.responsavel r 
				 		  INNER JOIN elabrev.liberacaoresponsavel lr ON lr.lrpid=r.lrpid  
				 		  INNER JOIN elabrev.funcaoliberacao lb ON lb.fnlid=lr.fnlid 
				 		  INNER JOIN seguranca.usuario usu ON lr.usucpf=usu.usucpf 
				 		  WHERE r.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND lb.fnlid='".FNL_COORDENACAO."'");
	
	echo (($usunome_coo)? "<img src=\"../imagens/".$usunome_coo."\">" : "N�o informado");
	?>
	<br/>
	Coordenador-Geral de Or�amento</strong>
	</td>
	<td align="center">
	<strong>
	<?
	$usunome_sub = $db->pegaUm("SELECT lr.lrpimgassinatura FROM elabrev.responsavel r 
				 		  INNER JOIN elabrev.liberacaoresponsavel lr ON lr.lrpid=r.lrpid  
				 		  INNER JOIN elabrev.funcaoliberacao lb ON lb.fnlid=lr.fnlid 
				 		  INNER JOIN seguranca.usuario usu ON lr.usucpf=usu.usucpf 
				 		  WHERE r.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND lb.fnlid='".FNL_SUBSECRETARIO."'");
	
	echo (($usunome_sub)? "<img src=\"../imagens/".$usunome_sub."\">" : "N�o informado");
	?>
	<br/>
	Subsecret�rio de Planejamento e Or�amento</strong>
	</td>
</tr>
</table>