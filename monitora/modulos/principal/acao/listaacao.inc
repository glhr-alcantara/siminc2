<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   M�dulo:listaacao.inc
   Finalidade: permitir o controle da lista de a��es
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_SESSION['prgid']) $_REQUEST['prgid'] = $_SESSION['prgid'];
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
	if (! $_SESSION['prgid'] or $_SESSION['prgid'] == '') {
	   ?>
	      <script>
	         alert ('Ocorreu um erro inesperado. Tente novamente!');
	         history.back();
	      </script>
	   <?
	     exit();
	   }
$sql = "select p.prgid, p.prgcod, p.prgdsc, count(*) as totalacoes from programa p inner join acao a on p.prgid=a.prgid where p.prgid='".$_SESSION['prgid']."' group by p.prgid, p.prgcod, p.prgdsc";

$RS = $db->recuperar($sql);
if(is_array($RS)) foreach($RS as $k=>$v) ${$k}=$v;


$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Consultar A��es do Programa';
monta_titulo($titulo_modulo, 'Para ver detalhes das A��es, clique no <img src="../imagens/mais.gif" border="0">');
?>
<form method="POST"  name="formulario">
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"  width="55"><strong>C�digo</strong></td>
      <td valign="top" class="title"><strong>T�tulo do Programa</strong></td>
      <td valign="top" class="title" width="45"><strong>Total de A��es</strong></td>
    </tr>
</thead>
<tbody>
	<tr>
      <td><strong><?=$prgcod?></strong></td>
	  <td><img src="../imagens/mais.gif" name="+" border="0" id="img<?=$prgcod?>"> <strong><?=$prgdsc?></strong></td>
	  <td align="right"><?=$totalacoes?></td>
    </tr>
    <tr>
      <td></td><td valign="top" id="td<?=$prgcod?>"></td><td></td>
    </tr>
</tbody>
</table>
<script language="JavaScript">
//Este script popula a p�gina com as a��es do programa
abreconteudo('geral/listaacao.php?prgid=<?=$prgid?>&ordem=A','<?=$prgcod?>');
</script>
