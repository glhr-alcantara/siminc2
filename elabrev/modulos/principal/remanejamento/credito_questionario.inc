<?php

require_once 'credito_funcoes.inc';
$edicao_ativa = cre_edicao_ativa();

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, 'Caracteriza��o e justificativa dos remanejamentos' );

if(!$_SESSION['creditos_adicionais_tcrid'] || !$_SESSION['creditos_adicionais_unicod']){
	echo "<script>
			alert('Sess�o expirou!');
			location.href='elabrev.php?modulo=principal/remanejamento/credito&acao=A';
		 </script>";
	die();
}

$tcrid = $_SESSION['creditos_adicionais_tcrid'];
$unicod = $_SESSION['creditos_adicionais_unicod'];

$jscresumo = '';
$jsccaracterizacao = '';
$jscresultado = '';
$jscconsequencia = '';
$jscreflexo = '';
$jscrepercussao = '';
$jscaplicacao = '';
$jscmemoria = '';

if ( $edicao_ativa && isset( $_REQUEST['form'] ) ) 
{
	// grava os dados do formul�rio no banco
	$jscresumo = $_REQUEST['jscresumo'];
	$jsccaracterizacao = $_REQUEST['jsccaracterizacao'];
	$jscresultado = $_REQUEST['jscresultado'];
	$jscconsequencia = $_REQUEST['jscconsequencia'];
	$jscreflexo = $_REQUEST['jscreflexo'];
	$jscrepercussao = $_REQUEST['jscrepercussao'];
	$jscaplicacao = $_REQUEST['jscaplicacao'];
	$jscmemoria = $_REQUEST['jscmemoria'];
	
	// verifica se existe justificativa
	$sql =
		" select count(*) as existe " .
		" from elabrev.justificativacredito " .
		" where " .
			" tcrid = " . $tcrid . "  and " .
			" unicod = '" . $unicod . "' and " .
			" unitpocod = 'U' and " .
			" mcrid = " . $_SESSION['credito_mcrid'];
	$existe = (boolean) $db->pegaUm( $sql );
	
	if ( $existe )
	{
		$sql =
			" update elabrev.justificativacredito " .
			" set " .
				" jscresumo = '" . ( $jscresumo ) . "', " .
				" jsccaracterizacao = '" . ( $jsccaracterizacao ) . "', " .
				" jscresultado = '" . ( $jscresultado ) . "', " .
				" jscconsequencia = '" . ( $jscconsequencia ) . "', " .
				" jscreflexo = '" . ( $jscreflexo ) . "', " .
				" jscrepercussao = '" . ( $jscrepercussao ) . "', " .
				" jscaplicacao = '" . ( $jscaplicacao ) . "', " .
				" jscmemoria = '" . ( $jscmemoria ) . "' " .
			" where " .
				" tcrid = " . $tcrid . " and " .
				" unicod = '" . $unicod . "' and " .
				" unitpocod = 'U' and " .
				" mcrid = " . $_SESSION['credito_mcrid'];
	}
	else
	{
		$sql =
			" insert into elabrev.justificativacredito " .
			" ( " .
				" tcrid, unicod, unitpocod, " .
				" jscresumo, " .
				" jsccaracterizacao, " .
				" jscresultado, " .
				" jscconsequencia, " .
				" jscreflexo, " .
				" jscrepercussao, " .
				" jscaplicacao, " .
				" jscmemoria, " .
				" mcrid " .
			" ) " .
			" values " .
			" ( " .
				$tcrid . ", '" . $unicod . "', 'U', " .
				" '" . ( $jscresumo ) . "', " .
				" '" . ( $jsccaracterizacao ) . "', " .
				" '" . ( $jscresultado ) . "', " .
				" '" . ( $jscconsequencia ) . "', " .
				" '" . ( $jscreflexo ) . "', " .
				" '" . ( $jscrepercussao ) . "', " .
				" '" . ( $jscaplicacao ) . "', " .
				" '" . ( $jscmemoria ) . "', " .
				" " . ( $_SESSION['credito_mcrid'] ) . " " .
			" ) ";
	}
	$db->executar( $sql );
	$db->commit();
}
else
{
	// carregar dados do banco
	$sql =
		" select * " .
		" from elabrev.justificativacredito " .
		" where " .
			" tcrid = " . $tcrid . " and " . 
			" unicod = '" . $unicod . "' and " .
			" mcrid = " . $_SESSION['credito_mcrid'];
	$dados = $db->recuperar( $sql );
	if ( $dados )
	{
		$jscresumo = $dados['jscresumo'];
		$jsccaracterizacao = $dados['jsccaracterizacao'];
		$jscresultado = $dados['jscresultado'];
		$jscconsequencia = $dados['jscconsequencia'];
		$jscreflexo = $dados['jscreflexo'];
		$jscrepercussao = $dados['jscrepercussao'];
		$jscaplicacao = $dados['jscaplicacao'];
		$jscmemoria = $dados['jscmemoria'];
	}
}

$tcrdsc = '';
$unidsc = '';

$sql = "select tcrdsc, tcrcod from elabrev.tipocredito where tcrid = " . $tcrid;
$dados = $db->recuperar( $sql );
$tcrcod = $dados['tcrcod'];
$tcrdsc = $dados['tcrdsc'];

$sql = "select unidsc from public.unidade where unicod = '" . $unicod . "'";
$unidsc = $db->pegaUm( $sql );

?>
<script type="text/javascript">
	
	function voltarEscolher()
	{
		document.escolher.escolher.value = '1';
		document.escolher.submit();
	}
	
</script>
<form action="?modulo=principal/remanejamento/credito&acao=A" method="post" name="escolher">
	<input type="hidden" name="escolher" value="1"/>
	<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-bottom: 0px;">
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Tipo de Cr�dito:
			</td>
			<td width="80%">
				<input type="hidden" name="tcrid" value="<?php echo $tcrid; ?>"/>
				<b><?= $tcrcod ?></b> - <?= $tcrdsc ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				Unidade:
			</td>
			<td width="80%">
				<input type="hidden" name="unicod" value="<?php echo $unicod; ?>"/>
				<b><?= $unicod ?></b> - <?= $unidsc ?>
			</td>
		</tr>
		<tr>
			<td width="20%" align="right" class="SubTituloDireita">
				&nbsp;
			</td>
			<td width="80%">
				<input type="button" onclick="voltarEscolher();" name="voltar" value="Voltar para tela inicial de Cr�ditos Adicionais" />
			</td>
		</tr>
	</table>
</form>
<form name="questionario" action="" method="post">
	<input type="hidden" name="form" value="1"/>
	<table class="tabela" width="90%" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" style="border-top: 0px;">
<!--
		<tr>
			<td width="20%" align="right" class="SubTituloDireita" >
				<b>Resumo</b>
			</td>
			<td width="80%">
				<?php
					//print campo_textarea( 'jscresumo', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000 );
				?>
			</td>
		</tr>
-->
		<tr>
			<td width="20%" align="right" class="SubTituloDireita" >
				<!--<b>Caracteriza��o</b> do problema e suas causas -->
				Indique a necessidade de altera��o or�ament�ria. - Aonde ser�o aplicados os recursos e quais as raz�es que deram origem a insufici�ncia dos recursos iniciais ?
			</td>
			<td width="80%">
				<?php
					print campo_textarea( 'jsccaracterizacao', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000, '', 0,
						'Descrever a situa��o atual, ou situa��o-problema, com as raz�es que deram origem a insufici�ncia de dota��o or�ament�ria detectada, incluindo a varia��o dos par�metros atuais em rela��o �queles originalmente utilizados, se for o caso.',
						true
					);
				?>
			</td>
		</tr>
		<!--<tr>
			<td align="right" class="SubTituloDireita" >
				<b>Resultados</b> esperados com a altera��o solicitada
			</td>
			<td>
				<?php
				/*	print campo_textarea( 'jscresultado', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000, '', 0,
						'Descrever os resultados esperados com a aplica��o dos recursos solicitados e os indicadores que demonstrarem seus efeitos na altera��o do quadro descrito na situa��o-problema, evidenciando o incremento qualitativo ou quantitativo dos n�veis de servi�os ou a��es.',
						false
					);
					*/
				?>
			</td>
		</tr> -->
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Reflexos</b> nos cancelamentos sobre a programa��o prevista e o impacto no plano pluruanual PPA 2003-2007 -->
				Descreva o impacto do cancelamento de dota��o. - Haver� impacto nas contas da unidade com o cancelamento (se for o caso) de dota��o? - A unidade poder� arcar com seus custos ?
			</td>
			<td>
				<?php
					print campo_textarea( 'jscreflexo', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000, '', 0,
						'Demonstrar quais os efeitos dos cancelamentos das dota��es propostas na execu��o da programa��o e o impacto no PPA 2008-2010, inclusive que n�o haver� pedido de recursos adicionais nestas dota��es.',
						false
					);
				?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Consequ�ncias</b> do n�o atendimento do pleito -->
				Quais as consequ�ncias do n�o atendimento do pleito ? - Caso n�o atendido o pedido de cr�dito, quais problemas acarretar�o para a unidade ?
			</td>
			<td>
				<?php
					print campo_textarea( 'jscconsequencia', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000, '', 0,
						'Apresentar as repercus�es negativas no desenvolvimento das a��es do �rg�o/unidade caso a altera��o solicitada n�o seja atendida ou a seja parcialmente.',
						false
					);
				?>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Repercus�o</b> no n�vel de gastos fixos decorrente da altera��o solicitada -->
				Quais os reflexos do atendimento da demanda sobre o n�vel dos gastos de custeio do �rg�o e/ou da unidade or�ament�ria? - Haver� aumento sobre os gastos com custeio da unidade?
				- Caso sim, quais e quanto? - A unidade est� preparada para esse aumento (caso haja)?
			</td>
			<td>
				<?php
					print campo_textarea( 'jscrepercussao', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000, '', 0,
						'Demonstrar o efeito do atendimento da solicita��o em rela��o ao n�vel de gasto fixo, indicando f�sica e financeiramente o acr�scimo.',
						false
					);
				?>
			</td>
		</tr>
		<!--<tr>
			<td align="right" class="SubTituloDireita" >
				"Como" e "em que" ser�o <b>aplicados</b> os recursos solicitados
			</td>
			<td>
				<?php
				/*	print campo_textarea( 'jscaplicacao', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000, '', 0,
						'Descrever pormenorizadamente *como* e *em que* ser�o aplicados os recursos. No caso de despesa de capital, especificar detalhadamente as aquisi��es, indicando os custos unit�rios ou totais. No caso de terceirizado, indicar a natureza do servi�o e o respectivo custo.',
						false
					);
					*/
				?>
			</td>
		</tr> -->
		<tr>
			<td align="right" class="SubTituloDireita" >
				<!--<b>Mem�rias</b> de c�lculo n�o inclu�das nos itens precedentes-->
				Mem�ria de c�lculo e outras informa��es consideradas relevantes.
			</td>
			<td>
				<?php
					print campo_textarea( 'jscmemoria', 'N', ( $edicao_ativa ? 'S' : 'N' ), 'Resumo do Cr�dito', 80, 4, 7000, '', 0,
						'Apresentar as mem�rias de c�lculo n�o inclu�das nos itens anteriores necess�rias � demonstra��o dos valores propostos, seja na suplementa��o seja no cancelamento.',
						false
					);
				?>
			</td>
		</tr>
		<?php if ( $edicao_ativa ) : ?>
			<tr>
				<td align="right" class="SubTituloDireita">&nbsp;</td>
				<td>
					<input type="button" value="Gravar" name="gravar" onclick="enviarQuestionario();"/>
				</td>
			</tr>
		<?php endif; ?>
	</table>
</form>
<script type="text/javascript">

	function enviarQuestionario()
	{
		var form = document.questionario;
		/*
		if ( !form.jscresumo.value )
		{
			alert( 'Preencha o resumo.' );
			form.jscresumo.focus();
			return;
		}
		if ( !form.jsccaracterizacao.value )
		{
			alert( 'Preencha a caracteriza��o.' );
			form.jsccaracterizacao.focus();
			return;
		}
		if ( !form.jscresultado.value )
		{
			alert( 'Preencha o resultado.' );
			form.jscresultado.focus();
			return;
		}
		if ( !form.jscconsequencia.value )
		{
			alert( 'Preencha a consequ�ncia.' );
			form.jscconsequencia.focus();
			return;
		}
		if ( !form.jscreflexo.value )
		{
			alert( 'Preencha o reflexo.' );
			form.jscreflexo.focus();
			return;
		}
		if ( !form.jscrepercussao.value )
		{
			alert( 'Preencha a repercuss�o.' );
			form.jscrepercussao.focus();
			return;
		}
		if ( !form.jscaplicacao.value )
		{
			alert( 'Preencha a aplica��o.' );
			form.jscaplicacao.focus();
			return;
		}
		if ( !form.jscmemoria.value )
		{
			alert( 'Preencha a mem�ria.' );
			form.jscmemoria.focus();
			return;
		}
		*/
		form.submit();
	}

</script>




