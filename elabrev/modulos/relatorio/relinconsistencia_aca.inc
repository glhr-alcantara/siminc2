<?
#
# Sistema Simec
# Setor respons�vel: SPO-MEC
# Desenvolvedor: Equipe Consultores Simec
# Analista: Gilberto Arruda Cerqueira Xavier
# Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
# Programador: Thiago Mata (e-mail thiago.henrique.mata@gmail.com )
# M�dulo:relinconsistencias_aca.inc
# Finalidade: permitir a constru��o de relatorio de inconsistencias em acao
#
// as inconsistencias em a��o s�o por exemplo propostas de exclus�o ao memso tempo que h� porposta de fus�o ou altera��o de atributos e migra��es

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relat�rio de Inconsist�ncias em A��es', '' );
	
?>

<form action="" method="post" name="formulario">
<input type="hidden" name="act" value="0"> 

<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
	<td>
		<b>	Relat�rios de Inconsist�ncias em A��es</b>
	</td>
 </tr>
</table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
	<tr>
		<td>
			<b> A��o (clique na a��o para ver todas as propostas)</b>
		</td>
	</tr>
</thead>
<tbody>
	<?
	$sql = " SELECT prg.prgcod||'-'||p.acacod||'.'||p.acadsc as descricao, va.* FROM elabrev.v_total_propostas_acoes va inner join elabrev.ppaacao_proposta p on p.acaid=va.acaid and p.prsano= '". $_SESSION[ 'exercicio' ] . "' inner join elabrev.ppaprograma_proposta prg on prg.prgid=p.prgid WHERE va.TOTAL >1 order by descricao " ;

	$acoes=$db->carregar($sql);
	$i=0;
	
	if ( $acoes ) {
		foreach ($acoes as $acao)
		{
	 		// a query acima me tras as a��es com inconsist�ncias
			$boolPar = ( ( $i % 2 ) == 0 );
			$i++;
			if 	($boolPar )	$strBgColor = '';
			else $strBgColor = '#F7F7F7';
			?>
	    	<tr bgcolor="<?= $strBgColor ?>" onmouseover="this.bgColor='#ffffcc';" 
						onmouseout="this.bgColor='<?= $strBgColor ?>';">
			   <td onclick="abre_rel(<?=$acao['acaid']?>)">
				<?=$acao['descricao']?>
			</td>
		</tr>
		<?	
		
		}
	}
	?>	
</tbody>
</table>
</form>

<script type="text/javascript">

function abre_rel(cod)
{
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/acao/geraproposta&acao=A&acod='+cod;
      window.open(e, "Relatorio","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=600,height=450"); 
}
	

</script>