<?php
define("FNL_COORDENACAO", 1);
define("FNL_SUBSECRETARIO", 2);

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );

if(!$_SESSION['elabrev_var']['lbrid']) {
	
	echo "<script>
			alert('Problemas nas variaveis enviadas');
			window.location='elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';
		  </script>";
	exit;
	
}
	
$sql = "UPDATE elabrev.liberacao SET lbrsituacao='G' WHERE lbrid='".$_SESSION['elabrev_var']['lbrid']."'";
$db->executar($sql);
$db->commit();


echo montarAbasArray(menuAbasLiberacao(), $_SERVER['REQUEST_URI']);

// c�digos das Unidades Gestoras
$filtro[] = " entid IN('391295','391282','391296','391283','391284','391285','391286','391287','391288','391289','391290','391291','391292')";


?>
<script language="JavaScript" src="../includes/prototype.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script>

function imprimirliberacao() {

	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: 'requisicao=alterarSituacaoLiberacao&lbrsituacao=I',
			asynchronous: false,
			onComplete: function(resp) {},
			onLoading: function(){}
		});


	var t = window.open('?modulo=principal/liberacaoorcamentariaimpressao&acao=A','Libera��o','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
	t.focus();
}

</script>
<form method='post' name='formulario'>
<input type='hidden' name='requisicao' value='gravarDadosLiberacao'>

<?
cabecalhoLiberacao($_SESSION['elabrev_var']['lbrid']);
?>

<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td style="width: 95%">
<?

/*
 * OBSERVA��ES:
 * A COLUNA GEST�O SEMPRE SER� 00001 (DEVER� DEIXA FIXO) DE ACORDO COM HENRIQUE XAVIER - 28/10/2009 
 */
/*
$sql = "SELECT --uni.unicod || ' - ' || uni.unidsc as unidade,
			   ent.entungcod || ' - ' || ent.entnome as unidadegestora,
			   '<center>00001</center>' as gestao, 
			   '<center>' || lbf.foscod || '</center>' as fontedetalhada, 
			   '<center>' || res.ptres || '</center>' as ptres,
			   '<center>' || lbp.plicod || '</center>' as plicod,
			   pli.plidsc as descricao,
			   nat.ndpcod || ' - ' || nat.ndpdsc as natureza,
			   aca.prgcod || '.' || aca.acacod || '.' || aca.loccod || ' - ' || aca.acadsc as acao,
			   --aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc as acao,
			   --aca.acadsc as acao,
			   '<input type=\"text\" size=\"21\" maxlength=\"18\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal(\'###.###.###.###,##\',this.value);\" name=\"valor['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\" value=\"'|| COALESCE((SELECT trim(to_char(lbavalor, '999g999g999g999d99')) FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'\">' as valor,
			   '<textarea name=\"obs['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\">'|| COALESCE((SELECT lbaobservacao FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'</textarea>' as observacao,
			   '<input type=\"text\" size=\"13\" maxlength=\"12\" name=\"nc['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\" value=\"'|| COALESCE((SELECT lbanc FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'\"  class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\">' as nc,
			   '<input type=\"text\" size=\"13\" maxlength=\"15\" name=\"empenho['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\" value=\"'|| COALESCE((SELECT lbapreempenho FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'\"  class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\">' as empenho
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
			   aca.prgcod || '.' || aca.acacod || '.' || aca.loccod || ' - ' || aca.acadsc as acao,
			   '<center>' || res.ptres || '</center>' as ptres,
			   lbp.plicod || ' - ' || pli.plititulo as plicod,
			   --pli.plidsc as descricao,
			   nat.ndpcod || ' - ' || nat.ndpdsc as natureza,
			   --aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc as acao,
			   --aca.acadsc as acao,
			   '<input type=\"text\" size=\"21\" maxlength=\"18\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onKeyUp=\"this.value=mascaraglobal(\'###.###.###.###,##\',this.value);\" name=\"valor['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\" value=\"'|| COALESCE((SELECT trim(to_char(lbavalor, '999g999g999g999d99')) FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'\">' as valor
			   --'<textarea name=\"obs['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\">'|| COALESCE((SELECT lbaobservacao FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'</textarea>' as observacao,
			   --'<input type=\"text\" size=\"13\" maxlength=\"12\" name=\"nc['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\" value=\"'|| COALESCE((SELECT lbanc FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'\"  class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\">' as nc,
			   --'<input type=\"text\" size=\"13\" maxlength=\"15\" name=\"empenho['||lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid||']\" value=\"'|| COALESCE((SELECT lbapreempenho FROM elabrev.liberacaodados WHERE lbachave=lbr.lbrid||'.'||lbd.lbdid||'.'||lbs.lbsid||'.'||lbp.lbpid||'.'||lpt.lptid||'.'||lbf.lbfid||'.'||lbn.lbnid),'') ||'\"  class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\">' as empenho
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
				 --ent.entungcod, 
				 --ent.entnome, 
				 --lbf.foscod, 
				 res.ptres, 
				 lbp.plicod,
				 pli.plititulo,
				 --pli.plidsc,
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
//dbg($sql,1);
//$cabecalho = array( /*"C�digo Unidade",*/ "UG", "Gest�o", "Fonte Detalhada", "PTRES", "PI", "Descri��o", "Natureza da despesa", "A��o", "Valor(R$)", "Observa��o", "NC", "Pr�-Empenho");

$cabecalho = array("A��o", "PTRES", "Plano Interno", "Natureza da despesa", "Valor(R$)");
$db->monta_lista_simples($sql,$cabecalho,50,5,'N','95%',$par2);

?>
</td>
<td valign="top"><?
	$docid = criarDocumento( $_SESSION['elabrev_var']['lbrid'] );
	wf_desenhaBarraNavegacao( $docid , array( 'lbrid' => $_SESSION['elabrev_var']['lbrid'], 'url' => $_SESSION['favurl'] ) );
?></td>
</tr></table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" >
<tr bgcolor="#C0C0C0" style="display: none">
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
	echo campo_data2( 'libdataliberacao', 'N', 'S', '', 'S' ); 
	?>
	</td>
</tr>
<tr bgcolor="#C0C0C0" style="display: none">
	<td align="center">
	<strong>
	<?
	/*
	$lrpid_coo = $db->pegaUm("SELECT r.lrpid FROM elabrev.responsavel r 
				 		  LEFT JOIN elabrev.liberacaoresponsavel lr ON lr.lrpid=r.lrpid  
				 		  LEFT JOIN elabrev.funcaoliberacao lb ON lb.fnlid=lr.fnlid 
				 		  WHERE r.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND lb.fnlid='".FNL_COORDENACAO."'");
	
	
	$sql = "SELECT lr.lrpid as codigo, usu.usunome as descricao FROM elabrev.liberacaoresponsavel lr 
			INNER JOIN elabrev.funcaoliberacao fl ON fl.fnlid=lr.fnlid   
			INNER JOIN seguranca.usuario usu ON lr.usucpf=usu.usucpf 
			WHERE lr.fnlid='".FNL_COORDENACAO."'";
	
	$db->monta_combo('responsavel['.FNL_COORDENACAO.']', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'responsavel['.FNL_COORDENACAO.']', false, $lrpid_coo);
	*/
	?>
	<br/>
	Coordenador-Geral de Or�amento</strong>
	</td>
	<td align="center">
	<strong>
	<?
	/*
	$lrpid_sub = $db->pegaUm("SELECT r.lrpid FROM elabrev.responsavel r 
				 		  LEFT JOIN elabrev.liberacaoresponsavel lr ON lr.lrpid=r.lrpid  
				 		  LEFT JOIN elabrev.funcaoliberacao lb ON lb.fnlid=lr.fnlid 
				 		  WHERE r.lbrid='".$_SESSION['elabrev_var']['lbrid']."' AND lb.fnlid='".FNL_SUBSECRETARIO."'");
	
	$sql = "SELECT lr.lrpid as codigo, usu.usunome as descricao FROM elabrev.liberacaoresponsavel lr 
			INNER JOIN elabrev.funcaoliberacao fl ON fl.fnlid=lr.fnlid   
			INNER JOIN seguranca.usuario usu ON lr.usucpf=usu.usucpf 
			WHERE lr.fnlid='".FNL_SUBSECRETARIO."'";
	
	$db->monta_combo('responsavel['.FNL_SUBSECRETARIO.']', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'responsavel['.FNL_SUBSECRETARIO.']', false, $lrpid_sub);
	*/
	?>
	<br/>
	Subsecret�rio de Planejamento e Or�amento</strong>
	</td>
</tr>
<tr>
	<td colspan="2" align="center"><input type="submit" <?=disabled($estadoAtual); ?> name="salvaform" value="Salvar"> <input type="button" name="impressao" value="Imprimir" onclick="imprimirliberacao();"></td>
</tr>
</table>
</form>