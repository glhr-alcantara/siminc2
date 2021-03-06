<?php

// alguma coisa aconteceu que a fun��o monta_link est� sumindo do arquivo funcoes.inc
if ( !function_exists( 'monta_link' ) )
{
	function monta_link( $modulo, $acao, $parametros = '' )
	{
		return '/' . $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . '.php?modulo=' . $modulo . '&acao=' . urlencode( $acao ) . '&' . $parametros;
	}
}

// ---------- SQLs PARA COMBOS POPUP ----------

$sql_acao =
	" select " .
		" acacod as codigo, " .
		" acacod || '.' || prgcod || ' ' || acadsc as descricao " .
	" from monitora.acao " .
	" where " .
		" prgano = '" . $_SESSION['exercicio'] . "' and " .
		" acastatus = 'A' " .
	" group by prgcod, acacod, acadsc";


$sql_unidade = "
SELECT unicod AS codigo,
       unicod || ' - ' || unidsc AS descricao
  FROM public.unidade
  WHERE unitpocod = 'U'
    AND uniano = '{$_SESSION['exercicio']}'
--    AND orgcod = '". CODIGO_ORGAO_SISTEMA. "'
";

$sql_gnd = "select gndcod as codigo, gndcod || ' - ' || gnddsc as descricao from public.gnd";
$sql_grupounidade = "select gunid as codigo, gundsc as descricao from public.grupounidade";
$sql_cred = "SELECT 
				ccrid as codigo, 
				ccrdescricao as descricao 
			 FROM 
				elabrev.colunascredito
			 WHERE
				ccrstatus = 'A'";

// define tipo de credito que est� sendo trabalhado
// manipula requisi��o do formul�rio

$tcrid                    = (integer) $_REQUEST['tcrid'];
$tcrcod                   = $_REQUEST['tcrcod'];
$tcrdsc                   = $_REQUEST['tcrdsc'];
$tcrdscfonte              = $_REQUEST['tcrdscfonte'];
$tcrdscautorizacao        = $_REQUEST['tcrdscautorizacao'];
$tcrtipoporcento          = $_REQUEST['tcrtipoporcento'];
$tcrporcentocancelamento  = $_REQUEST['tcrporcentocancelamento'];
$tcrporcentosuplementacao = $_REQUEST['tcrporcentosuplementacao'];
$tcrnaturezapropria       = (boolean) $_REQUEST['tcrnaturezapropria'];

$acacod = $_REQUEST['acacod'];
$unicod = $_REQUEST['unicod'];
$gndcod = $_REQUEST['gndcod'];
$gunid  = $_REQUEST['gunid'];

if ( isset( $_REQUEST['form'] ) )
{
	if ( $tcrid ) // editar
	{
		$sucesso = tc_editar( $tcrid, $tcrcod, $tcrdsc, $tcrdscfonte, $tcrdscautorizacao, $tcrtipoporcento, $tcrporcentocancelamento, $tcrporcentosuplementacao, $tcrnaturezapropria );
	}
	else // inserir
	{
		$tcrid = tc_inserir( $tcrcod, $tcrdsc, $tcrdscfonte, $tcrdscautorizacao, $tcrtipoporcento, $tcrporcentocancelamento, $tcrporcentosuplementacao, $tcrnaturezapropria );
		$sucesso = (boolean) $tcrid;
	}
	if ( !$sucesso )
	{
		$db->insucesso( 'Erro ao gravar dados no banco de dados', '', '' );
	}
	tc_inserir_acoes( $tcrid, $acacod );
	tc_inserir_unidades( $tcrid, $unicod );
	tc_inserir_gnds( $tcrid, $gndcod );
	tc_inserir_grupos_unidade( $tcrid, $gunid );
	$db->commit();
}

// carrega dados do banco

if ( $tcrid )
{
	$dados = tc_pegar( $tcrid );
	// captura dados gerais
	$tcrcod                   = $dados['tcrcod'];
	$tcrdsc                   = $dados['tcrdsc'];
	$tcrdscfonte              = $dados['tcrdscfonte'];
	$tcrdscautorizacao        = $dados['tcrdscautorizacao'];
	$tcrtipoporcento          = $dados['tcrtipoporcento'];
	$tcrporcentocancelamento  = $dados['tcrporcentocancelamento'];
	$tcrporcentosuplementacao = $dados['tcrporcentosuplementacao'];
	$tcrnaturezapropria       = $dados['tcrnaturezapropria'] == 't';
	// captura dados de relacionamentos externos
	$acacod = tc_pegar_acoes( $tcrid );
	$unicod = tc_pegar_unidades( $tcrid );
	$gndcod = tc_pegar_gnds( $tcrid );
	$gunid = tc_pegar_grupos_unidade( $tcrid );
}
else
{
	$tcrporcentocancelamento  = '0';
	$tcrporcentosuplementacao = '0';
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );

?>
<form action="?modulo=principal/remanejamento/tipocredito&acao=E" method="post" name="tipocredito">
	<input type="hidden" name="form" value="1">
	<input type="hidden" name="tcrid" value="<?= $tcrid ?>">
	<table class="tabela" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" >
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				C�digo:
			</td>
			<td width="80%">
				<?= campo_texto( 'tcrcod', 'S', 'S', 'C�digo', 5, 4, '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Permite Receita Pr�pria (Naturezas de Receita):
			</td>
			<td width="80%">
				<input type="checkbox" name="tcrnaturezapropria" value="1" <?php echo $tcrnaturezapropria ? 'checked="checked"' : ''; ?>/>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Descri��o:
			</td>
			<td width="80%">
				<?= campo_textarea( 'tcrdsc', 'N', 'S', 'C�digo', 70, 5, 2000 ); ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Fontes de Recurso:
			</td>
			<td width="80%">
				<?= campo_textarea( 'tcrdscfonte', 'N', 'S', 'C�digo', 70, 5, 2000 ); ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Autoriza��o:
			</td>
			<td width="80%">
				<?= campo_textarea( 'tcrdscautorizacao', 'N', 'S', 'C�digo', 50, 3, 1000 ); ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Tipo Dota��o:
			</td>
			<td width="80%">
				<select name="tcrtipoporcento">
					<option value="U" <?= $tcrtipoporcento == 'U' ? 'selected="selected"' : '' ;?>>Unidade</option>
					<option value="L" <?= $tcrtipoporcento == 'L' ? 'selected="selected"' : '' ;?>>Localizador</option>
				</select><img border='0' src='../imagens/obrig.gif' title='Indica campo obrigat�rio.'>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Cancelamento M�ximo:
			</td>
			<td width="80%">
				<?= campo_texto( 'tcrporcentocancelamento', 'S', 'S', 'C�digo', 3, 2, '##', 'N' ) ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Suplementa��o M�xima:
			</td>
			<td width="80%">
				<?= campo_texto( 'tcrporcentosuplementacao', 'S', 'S', 'C�digo', 3, 2, '##', 'N' ) ?>
			</td>
		</tr>
		<?php 
		
				// A��es
				$stSqlCarregados = "";
				mostrarComboPopup( 'A��es', 'acacod',  $sql_acao, '', 'Selecione a(s) A��o(�es)' );
		
				// Tipos de Unidade
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipos de Unidade', 'gunid',  $sql_grupounidade, '', 'Selecione o(s) Tipo(s) de Unidade' );
		
				// Unidades Or�ament�rias
				$stSqlCarregados = "";
				mostrarComboPopup( 'Unidades Or�ament�rias', 'unicod',  $sql_unidade, '', 'Selecione a(s) Unidade(s) Or�ament�ria(s)' );
		
				// Grupo de Natureza e Despesa
				$stSqlCarregados = "";
				mostrarComboPopup( 'Grupo de Natureza e Despesa', 'gndcod',  $sql_gnd, '', 'Selecione o(s) Grupo(s) de Natureza e Despesa' );
		
				// Tipos de Cr�dito
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipos de Cr�dito', 'ccrid',  $sql_cred, '', 'Selecione o(s) Tipo(s) de Cr�dito' );
		?>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				&nbsp;
			</td>
			<td width="80%">
				<input type="button" onclick="submeter();" name="gravar" value="Gravar" />
				&nbsp;&nbsp;
				<input type="button" onclick="voltarListagem();" name="voltar" value="Voltar" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">

	function submeter()
	{
		var form = document.tipocredito;
		if ( !form.tcrcod.value )
		{
			alert( 'Preencha o campo c�digo' );
			return;
		}
		if ( form.tcrporcentocancelamento.value == '' )
		{
			alert( 'Preencha o campo cancelamento m�ximo' );
			return;
		}
		if ( form.tcrporcentosuplementacao.value == '' )
		{
			alert( 'Preencha o campo suplementa��o m�xima' );
			return;
		}
		selectAllOptions( document.getElementById( 'acacod' ) );
		selectAllOptions( document.getElementById( 'unicod' ) );
		selectAllOptions( document.getElementById( 'gunid' ) );
		selectAllOptions( document.getElementById( 'gndcod' ) );
		form.submit();
	}
	
	function voltarListagem()
	{
		window.location = '<?= monta_link( 'principal/remanejamento/tipocredito', 'L' ) ?>';
	}

	function onOffCampo( campo ) {
		var div_on  = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input   = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

</script>
