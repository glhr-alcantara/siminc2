<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   M�dulo:cadindicador.inc
   Finalidade: permitir a listagem de indicadores de um programa
   */
$modulo=$_REQUEST['modulo'] ;//
			$prgid = (integer) $_REQUEST['prgid'];
function my_ucwords( $string )
{
	$string = strtolower( $string );	
	$minuscole = array(
		'a', '�', 'e', '�', 'o',
		'da', 'de', 'do',
		'com', 'para',
		'mas', 'por�m', 'ent�o',
		'at�',
		'l�',
		'em', 'no', 'na', 'num', 'nuns', 'numa', 'numas',
		'um', 'uns', 'uma', 'umas'
	);
	$result = '';
	foreach ( split( '[[:space:]]', $string ) as $word )
	{
		if ( in_array( $word, $minuscole ) )
		{
			$result .= ' ' . strtolower( $word );
		}
		else
		{
			$result .= ' ' . ucfirst( $word );
		}
	}
	return ucfirst( substr( $result, 1 ) );
}

if ($_SESSION['prgid']) $_REQUEST['prgid'] = $_SESSION['prgid'];

if ($_REQUEST['indnum']) $_SESSION['indnum'] = $_REQUEST['indnum'];
else $_REQUEST['indnum'] = $_SESSION['indnum'];


if ($_REQUEST['act']=='incluir')
{
	// inclusao
	$sql = "select prgcod, prgid from elabrev.ppaprograma_proposta where prgid=".$_SESSION['prgid'];
	$linha = $db->pegaLinha($sql);
	if(is_array($linha)) foreach($linha as $k=>$v) ${$k}=$v;	
	
	$sqlindnum = "select nextval('elabrev.limiteppo_lmpid_seq'::regclass)";
	$indnum=$db->pegaUm($sqlindnum);
	$unmcod= $_REQUEST['unmcod'];
	$bsgcod= $_REQUEST['bsgcod'];
	$percod = $_REQUEST['percod'];
    $inddsc = $_REQUEST['inddsc'];
	$inddscfonte = $_REQUEST['inddscfonte'];
	$inddscformula = $_REQUEST['inddscformula'];
	$indvlrano3 = $_REQUEST['indvlrano3'];
	$indvlrano4 = $_REQUEST['indvlrano4'];
	$indvlrano5 = $_REQUEST['indvlrano5'];
	$indvlrano6 = $_REQUEST['indvlrano6'];
	$indvlrreferencia = $_REQUEST['indvlrreferencia'];
	$indvlrfinalprg = $_REQUEST['indvlrfinalprg'];
	$indvlrfinalppa = $_REQUEST['indvlrfinalppa'];
	$inddataapuracao = $_REQUEST['inddataapuracao'];
	$prsano = $_SESSION['exercicio'];
	$prgano = $prsano;
	$indobs = $_REQUEST['indobs'];
	$usucpf = $_SESSION['usucpf'];
	$inddsc=my_ucwords($inddsc);
	

    if (! $indvlrano3) $indvlrano3=0;
	if (! $indvlrano4) $indvlrano4=0;	
    if (! $indvlrano5) $indvlrano5=0;
	if (! $indvlrano6) $indvlrano6=0;	
	if (! $indvlrfinalprg) $indvlrfinalprg=0;	
    if (! $indvlrfinalppa) $indvlrfinalppa=0;
	if (! $indvlrreferencia) $indvlrreferencia=0;	
	
	
	$sql = " insert into elabrev.ppaindicador (indnum,unmcod,bsgcod,percod,
	         prgano,prgcod,prgid,inddsc,inddscfonte,inddscformula,
	         indvlrano3,indvlrano4,indvlrano5,indvlrano6,
	         indvlrreferencia,indvlrfinalprg,indvlrfinalppa,inddataapuracao,prsano,
	         indobs,usucpf,indstatus) values ($indnum,$unmcod,$bsgcod,$percod,
	         '$prgano','$prgcod',$prgid,'$inddsc','$inddscfonte','$inddscformula',
	         $indvlrano3,$indvlrano4,$indvlrano5,$indvlrano6,
	         $indvlrreferencia,$indvlrfinalprg,$indvlrfinalppa,'$inddataapuracao','$prsano','$indobs','$usucpf','N')";
	//dbg($sql,1);
    $res = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo);
 
}
if ($_REQUEST['act']=='alterar')
{
	//alteracao
	$sql = "select prgcod, prgid from elabrev.ppaprograma_proposta where prgid=".$_SESSION['prgid'];
	$linha = $db->pegaLinha($sql);
	if(is_array($linha)) foreach($linha as $k=>$v) ${$k}=$v;	
	$indnum=$_REQUEST['indnum'];
	
	$unmcod= $_REQUEST['unmcod'];
	$bsgcod= $_REQUEST['bsgcod'];
	$percod = $_REQUEST['percod'];
    $inddsc = $_REQUEST['inddsc'];
	$inddscfonte = $_REQUEST['inddscfonte'];
	$inddscformula = $_REQUEST['inddscformula'];
	$indvlrano3 = $_REQUEST['indvlrano3'];
	$indvlrano4 = $_REQUEST['indvlrano4'];
	$indvlrano5 = $_REQUEST['indvlrano5'];
	$indvlrano6 = $_REQUEST['indvlrano6'];
	$indvlrreferencia = $_REQUEST['indvlrreferencia'];
	$indvlrfinalprg = $_REQUEST['indvlrfinalprg'];
	$indvlrfinalppa = $_REQUEST['indvlrfinalppa'];
	$inddataapuracao = $_REQUEST['inddataapuracao'];
	$prsano = $_SESSION['exercicio'];
	$prgano = $prsano;
	$indobs = $_REQUEST['indobs'];
	$usucpf = $_SESSION['usucpf'];
    if (! $indvlrano3) $indvlrano3=0;
	if (! $indvlrano4) $indvlrano4=0;	
    if (! $indvlrano5) $indvlrano5=0;
	if (! $indvlrano6) $indvlrano6=0;	
	if (! $indvlrfinalprg) $indvlrfinalprg=0;	
    if (! $indvlrfinalppa) $indvlrfinalppa=0;
	if (! $indvlrreferencia) $indvlrreferencia=0;	
	$sql = " update elabrev.ppaindicador set unmcod=$unmcod,bsgcod=$bsgcod,percod=$percod,
	         inddsc='$inddsc',inddscfonte='$inddscfonte',inddscformula='$inddscformula',
	         indvlrano3=$indvlrano3,indvlrano4=$indvlrano4,indvlrano5=$indvlrano5,indvlrano6=$indvlrano6,
	         indvlrreferencia=$indvlrreferencia,indvlrfinalprg=$indvlrfinalprg,indvlrfinalppa=$indvlrfinalppa,inddataapuracao='$inddataapuracao',indobs='$indobs' where indnum=$indnum";
	//dbg($sql,1);
    $res = $db->executar($sql);
    $db->commit();
    $db->sucesso($modulo);	
}
?>
<title>Indicadores de Programas do Simec</title>
<?
include APPRAIZ."includes/cabecalho.inc";

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Indicadores de Programa';
monta_titulo($titulo_modulo,'');
// carrega os ano base
$sql = "select a.* from anobase a order by anbordem";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas >= 0) {
	for ($i=0;$i<=$nlinhas;$i++)
	{
       $res =  @$db->carrega_registro($RS,$i);
       foreach($res as $k=>$v) {${$k}=$v;};
       ${'ano'.($i+1)}=$anbvalor;
	}
}
$momento = $db->verifica_momento();
$decisor = $db->ehdecisor($_SESSION['prgid'],'P',$momento); // verifica se � decisor
if ($decisor) $digit = $decisor;
else $digit = $db->ehdigitador($_SESSION['prgid'],'P',$momento); //verifica se � digitador
$sql = "select prgsntemporario, prgcod, prgdsc from elabrev.ppaprograma_proposta where prgid=".$_SESSION['prgid'];
$linha=$db->pegaLinha($sql);
foreach($linha as $k=>$v) {${$k}=$v;};
?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Programa:</td>
        <td><b><?=$prgcod?></b> - <?=$prgdsc?>
      </tr> 
</table>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="indnum" value=<?=$_REQUEST['indnum']?>>
<input type=hidden name="act" value=0>

<? if ($_REQUEST['indnum'] or $_REQUEST['act']=='I')
{
	//dbg($_REQUEST['act'],1);
	
	if ($_REQUEST['indnum'] and $_REQUEST['act']<>'alt' and $_REQUEST['act']<>'I')
	{
		//dbg($_REQUEST['act'],1);
		$sql = "select i.*,to_char(i.inddataapuracao, 'dd/mm/yyyy') as inddataapuracao,um.unmdsc, p.perdsc, b.bsgdsc from elabrev.ppaindicador i left join unidademedida um on um.unmcod=i.unmcod left join periodicidade p on p.percod=i.percod left join basegeografica b on b.bsgcod=i.bsgcod where indnum=".$_REQUEST['indnum'];
		$saida = $db->recuperar($sql,$res);
        if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}	
	?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr>
   <td align='right' class="SubTituloDireita">Denomina��o:</td>
   <td ><?=campo_textarea('inddsc','N','N','',80,3,'','','','');?></td>
 </tr>
 <tr>
   <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
   <td ><?=campo_texto('unmdsc','N','N','',50,50,'','','','dica',"unidademedida;unmcod;unm;combo;ppaindicador;Unidade de Medida ;$unmcod")?></td> 
 </tr>      
 <tr>
    <td align='right' class="SubTituloDireita">�ndice de refer�ncia<br>Data de apura��o:</td>
    <td ><?=campo_texto('indvlrreferencia','N','N','',50,50,'','','','dica',"ppaindicador;indvlrreferencia;ind;numfrac;ppaindicador;�ndice Valor de Refer�ncia ;$indvlrreferencia")?><br><b>Data: <?=campo_texto('inddataapuracao','N','N','',15,15,'','','','dica',"ppaindicador;inddataapuracao;ind;data;ppaindicador;Data de apura��o ;$inddataapuracao")?></td>       
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">�ndices esperados ao longo do PPA:</td>  
    <td ><?=$ano1?>:<?=campo_texto('indvlrano1','N','N','',5,5,'','','','dica',"")?>&nbsp;&nbsp;<?=$ano2?>:<?=campo_texto('indvlrano2','N','N','',5,5,'','','','dica',"")?>&nbsp;&nbsp;<?=$ano3?>:<?=campo_texto('indvlrano3','N','N','',5,5,'','','','dica',"ppaindicador;indvlrano3;ind;numfrac;ppaindicador;�ndice Ano 3 ;$indvlrano3")?>&nbsp;&nbsp;<br><?=$ano4?>:<?=campo_texto('indvlrano4','N','N','',5,5,'','','','dica',"ppaindicador;indvlrano4;ind;numfrac;ppaindicador;�ndice Ano 4 ;$indvlrano4")?>&nbsp;&nbsp;<?=$ano5?>:<?=campo_texto('indvlrano5','N','N','',5,5,'','','','dica',"ppaindicador;indvlrano5;ind;numfrac;ppaindicador;�ndice Ano 5 ;$indvlrano5")?>&nbsp;&nbsp;<?=$ano6?>:<?=campo_texto('indvlrano6','N','N','',5,5,'','','','dica',"ppaindicador;indvlrano6;ind;numfrac;ppaindicador;�ndice Ano 6 ;$indvlrano6")?></td>
  </tr>
  <? if ($prgsntemporario=='t') {?>
  <tr>
     <td align='right' class="SubTituloDireita">�ndice ao final do programa:</td>  
     <td ><?=campo_texto('indvlrfinalprg','N','N','',8,8,'','','','dica',"ppaindicador;indvlrfinalprg;ind;texto;ppaindicador;�ndice ao Final do Programa ;$indvlrfinalprg")?></td>          
  </tr> 
  <?}?>
  <tr>
    <td align='right' class="SubTituloDireita">Fonte:</td>  
    <td ><?=campo_textarea('inddscfonte','N','N','',80,3,'','dica','ppaindicador;inddscfonte;ind;text;ppaindicador;Fonte');?></td>
  </tr> 
  <tr>
     <td align='right' class="SubTituloDireita">Periodicidade:</td>  
     <td ><?=campo_texto('perdsc','N','N','',50,50,'','','','dica',"periodicidade;percod;per;combo;ppaindicador;Periodicidade ;$percod")?></td>
  </tr> 
  <tr>
    <td align='right' class="SubTituloDireita">Base geogr�fica:</td>
    <td><?=campo_texto('bsgdsc','N','N','',50,50,'','','','dica',"basegeografica;bsgcod;bsg;combo;ppaindicador;Base geogr�fica ;$bsgcod")?></td> 
  </tr>            
  <tr>
    <td align='right' class="SubTituloDireita">F�rmula:</td>  
    <td ><?=campo_textarea('inddscformula','N','N','',80,3,'','dica','ppaindicador;inddscformula;ind;text;ppaindicador;F�rmula');?></td>
  </tr> 
      <tr>
    <td align='right' class="SubTituloDireita">Observa��es:</td>  
    <td ><?=campo_textarea('indobs','N','N','',80,3,'','dica','ppaindicador;indobs;ind;text;ppaindicador;Observa��o');?></td>
  </tr>
 </table><?}
   else if ($_REQUEST['act']=='I' or $_REQUEST['act']=='alt')
  {
  	if ($_REQUEST['act']=='alt')
  	{
  		
  		$sql = "select i.*,to_char(i.inddataapuracao, 'dd/mm/yyyy') as inddataapuracao,um.unmdsc, p.perdsc, b.bsgdsc from elabrev.ppaindicador i left join unidademedida um on um.unmcod=i.unmcod left join periodicidade p on p.percod=i.percod left join basegeografica b on b.bsgcod=i.bsgcod where indnum=".$_REQUEST['indnum'];
		$saida = $db->recuperar($sql,$res);
        if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}	
  	}
  	?>
  	<script language="JavaScript" src="../includes/calendario.js"></script>
  	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
 <tr>
   <td align='right' class="SubTituloDireita">Denomina��o:</td>
   <td ><?=campo_texto('inddsc','S','S','Forma pela qual o indicador ser� apresentado � sociedade',80,200,'','','','Forma pela qual o indicador ser� apresentado � sociedade');?></td>
 </tr>
 <tr>
   <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
   <td ><?
	  $sql = "select unmcod as CODIGO,unmdsc as DESCRICAO from unidademedida where unmstatus='A' order by unmdsc ";
	  $db->monta_combo("unmcod",$sql,'S',"Escolha a Unidade de Medida",'','','Padr�o escolhido para mensura��o da rela��o adotada como indicador. a unidade de medida seria -porcentagem-, e para o indicador -taxa de mortalidade infantil- a unidade de medida seria -1/1000- (1 �bito para cada 1000 nascimentos)',400);	 ?>
   </td> 
 </tr>      
 <tr>
   <td align='right' class="SubTituloDireita">�ndice de refer�ncia<br>Data de apura��o:</td>
    <td ><?=campo_texto('indvlrreferencia','S','S','',10,10,'###.##','','','Expressa a situa��o mais recente do problema e sua respectiva data de apura��o. Consiste na aferi��o de um indicador em um dado momento, mensurado com a unidade de medida escolhida.')?><br><b>Data: <?=campo_data('inddataapuracao','S','S','abcd','S','');?></td>       
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">�ndices esperados ao longo do PPA:</td>  
    <td ><?=$ano3?>:<?=campo_texto('indvlrano3','S','S','',10,10,'###.##','','','Situa��o que se deseja atingir com a execu��o do programa, expresso pelo indicador, ao longo de cada ano do per�odo de vig�ncia do PPA.')?>&nbsp;&nbsp;<?=$ano4?>:<?=campo_texto('indvlrano4','S','S','',10,10,'###.##','','','Situa��o que se deseja atingir com a execu��o do programa, expresso pelo indicador, ao longo de cada ano do per�odo de vig�ncia do PPA.')?><br><?=$ano5?>:<?=campo_texto('indvlrano5','S','S','',10,10,'###.##','','','Situa��o que se deseja atingir com a execu��o do programa, expresso pelo indicador, ao longo de cada ano do per�odo de vig�ncia do PPA.')?>&nbsp;&nbsp;<?=$ano6?>:<?=campo_texto('indvlrano6','S','S','',10,10,'###.##','','','Situa��o que se deseja atingir com a execu��o do programa, expresso pelo indicador, ao longo de cada ano do per�odo de vig�ncia do PPA.')?></td>
  </tr>
  <? if ($prgsntemporario=='t') {?>
  <tr>
     <td align='right' class="SubTituloDireita">�ndice ao final do programa:</td>  
     <td ><?=campo_texto('indvlrfinalprg','S','S','',10,10,'###.##','','','Resultado, expresso pelo indicador, que se deseja atingir com a conclus�o da execu��o do programa.')?></td>          
  </tr> 
  <?}?>
  <tr>
    <td align='right' class="SubTituloDireita">Fonte:</td>  
    <td ><?=campo_textarea('inddscfonte','S','S','',80,3,'','','','�rg�o respons�vel pelo registro ou produ��o das informa��es necess�rias para a apura��o do indicador e divulga��o peri�dica dos �ndices. A maior parte das informa��es utilizadas na constru��o dos indicadores poder� ser produzida pelos pr�prios �rg�os executores dos programas ou outros integrantes da estrutura do Minist�rio respons�vel. Estes dever�o ma nter sistemas de coleta e tratamento de informa��es com esta finalidade. Em muitos casos as informa��es ser�o buscadas junto a outras fontes que podem ser institui��es oficiais ou mesmo privadas,quando de reconhecida credibilidade: IBGE, FIPE, FGV, Banco Central, DIEESE, ANBID, entre outras.');?></td>
  </tr> 
  <tr>
     <td align='right' class="SubTituloDireita">Periodicidade:</td>  
     <td ><?
     $sql = "select percod as CODIGO,perdsc as DESCRICAO from periodicidade where perstatus='A' order by perdsc ";
	  $db->monta_combo("percod",$sql,'S',"Escolha a Periodicidade",'','','Freq��ncia com a qual o indicador � apurado.',400);?></td>
  </tr> 
  <tr>
    <td align='right' class="SubTituloDireita">Base geogr�fica:</td>
    <td><?$sql = "select bsgcod as CODIGO,bsgdsc as DESCRICAO from basegeografica where bsgstatus='A' order by bsgdsc ";
	  $db->monta_combo("bsgcod",$sql,'S',"Escolha a Base geogr�fica",'','','Menor n�vel de agrega��o geogr�fica da apura��o do �ndice, podendo ser municipal, estadual, regional ou nacional.',400);?></td> 
  </tr>            
  <tr>
    <td align='right' class="SubTituloDireita">F�rmula:</td>  
    <td ><?=campo_textarea('inddscformula','S','S','',80,3,'','','','Demonstra, de forma sucinta e por meio de express�es matem�ticas, o algoritmo que permite calcular o valor do indicador. Por exemplo, para o indicador - Espa�o a�reo monitorado -, a f�rmula de c�lculo poderia ser - Rela��o percentual entre o espa�o a�reo monitorado e o espa�o a�reo sob jurisdi��o do Brasil-, assim como para o indicador - Incid�ncia do t�tano neonatal - o indicador poderia ser  -Rela��o percentual entre o n�mero de casos novos de t�tano neonatal e o total da popula��o menor de um ano de idade.');?></td>
  </tr> 
      <tr>
    <td align='right' class="SubTituloDireita">Observa��es:</td>  
    <td ><?=campo_textarea('indobs','N','S','',80,3,'','','','');?></td>
  </tr>
  <tr align="center">
   <td colspan="2" >
      <? if ($_REQUEST['act']=='I') { ?>
        <input type="button" name="btinclui" value="Incluir" onclick="gravar_proposta('I')"> <?} else {
        	?>
        	<input type="button" name="btalterar" value="Alterar" onclick="gravar_proposta('A')">
        	<?}?>
        
   </td>
   </tr>
 </table>
  <?}}
 
 ?>

 <table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="3" valign="top" align="center" class="title"><strong>Indicadores</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>A��es</strong></td>
      <td valign="top" class="title"><strong>Indicador </strong></td>      
    </tr>
</thead>
<tbody>
<?
$sql= "select distinct i.indnum as codigo, i.inddsc as descricao, i.usucpf, i.indstatus,case when i.usucpf is not null then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || i.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum ||'-'||un.unidsc else '' end as autor from elabrev.ppaindicador i  left join seguranca.usuario u on u.usucpf=i.usucpf left join unidade un on un.unicod=u.unicod where i.prgid=".$_SESSION['prgid']." and i.prsano='".$_SESSION['exercicio']."'  order by i.indnum, descricao";

$rs = $db->carregar( $sql );
if (  $rs && count($rs) > 0 )
	{
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
       	 print "<tr class='$marcado'>
  <td><img border='0' src='../imagens/alterar.gif' title='Alterar a proposta.' onclick=".'"ver_indicador('."'$codigo')".'"'."><img border='0' src='../imagens/exclusao.gif' title='Propor excluir o Indicador.' onclick=".'"exclui_indicador('."'".md5_encrypt($codigo,'')."')".'"'.">";
       	 if ($usucpf=$_SESSION['usucpf'] and $indstatus=='N')
       	 { 
       	 	print "<img border='0' src='../imagens/alteracao.gif' title='Editar a proposta.' onclick=".'"editar_indicador('."'$codigo')".'"'."><img border='0' src='../imagens/excluir.gif' title='Excluir a proposta.' onclick=".'"excluir_indicador('."'$codigo')".'"'.">"; 
       	 }
         print "</td><td>$autor.$descricao</td></tr>";
  		 $i++;
		}
	}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>N�o h� registros de Indicadores</td></tr>';
}
if ($momento and $digit)
{?>
   <tr align="center">
   <td colspan="2">
        <input type="button" name="btincluir" value="Incluir Indicador" onclick="incluir_indicador()">
   </td>
   </tr>
<?}

?>
</tbody>
</table>  
 
</form>
  </center>
  
  <script>
   function ver_indicador(cod)
   {
   	   document.formulario.indnum.value=cod;
	   document.formulario.submit();   	
   }
   function editar_indicador(cod)
   { 
   	   document.formulario.indnum.value=cod;
   	   document.formulario.act.value='alt';
	   document.formulario.submit();   	
   }   
   function incluir_indicador()
   {
   	   document.formulario.act.value='I';
	   document.formulario.submit();   	
   }   
   
var WindowObjectReference; /* Declaring a global variable
which will store a reference to the new window to be created */
   function edita_campo(tabela_alvo,campo,padrao,tipo_obj,tabela_origem,nome_campo,especial,especial2)
   {
   	 e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostarevisaoind&acao=A&alvo='+tabela_alvo+'&campo='+campo+'&padrao='+padrao+'&tipo_obj='+tipo_obj+'&origem='+tabela_origem+'&nomcampo='+nome_campo+'&especial='+especial+'&especial2='+especial2;

      window.open(e, "Associa��o_de_Respons�veis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=600,height=450");
     // &acao=A&alvo=orgao&campo=orgcod&padrao=org&tipo_obj=combo,acao&tabela_origem=undefined
   } 
   
    function exclui_indicador(cod)
   {
   	 e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/programa/propostaexcindicador&acao=A&indnum='+cod;

      window.open(e, "exclusaoIncicador","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=600,height=450");
     
   }
   
       function gravar_proposta(cod)
   {	
   	  //prepara_formulario();
   	  // testar todos os campos antes de incluir

   	  if (!validaBranco(document.formulario.inddsc, 'Denomina��o')) return;
   	  
   	  if (!validaBranco(document.formulario.unmcod, 'Unidade de Medida')) return;
   	  
   	  if (!validaBranco(document.formulario.indvlrreferencia, '�ndice de refer�ncia')) return;
   	  if (!validaBranco(document.formulario.inddataapuracao, 'Data de apura��o')) return;
   	  
   	  if (!validaBranco(document.formulario.inddscfonte, 'Fonte')) return;
   	  if (!validaBranco(document.formulario.percod, 'Periodicidade')) return;
   	  if (!validaBranco(document.formulario.bsgcod, 'Base geogr�fica')) return;
   	  if (!validaBranco(document.formulario.inddscformula, 'F�rmula')) return;   	  
 	  
   	  if (cod=='I') document.formulario.act.value='incluir';
   	  else document.formulario.act.value='alterar';
   	   	   document.formulario.submit();
   }
  </script>
