<?php

	/*
		Sistema Simec
		Setor respons�vel: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		M�dulo: limitefonte.inc
		Finalidade: distribuir o montante de um proposta de uma matriz para as unidades na tabela limitefonte
	*/

	// FUN��ES DA P�GINA
	
		/**
		 * Insere um registro na tabela elabrev.limitefonte e retorna
		 * seu identificador em caso de sucesso ou 0 em caso de falha.
		 *
		 * @param integer $mtmid
		 * @param string $unicod
		 * @param string $unitpocod
		 * @param string $foncod
		 * @param integer $lmfvalor
		 * @return integer
		 */
		function insere_limite( $mtmid, $unicod, $unitpocod, $foncod, $lmfvalor )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$fravalor = (integer) $fravalor;
			$sql = "insert into elabrev.limitefonte ( mtmid, unicod, unitpocod, foncod, lmfvalor ) values( " . $mtmid . ", '" . $unicod . "', '" . $unitpocod . "', '" . $foncod . "', " . $lmfvalor . " )";
			if ( !$db->executar( $sql ) )
			{
				return 0;
			}
			return $db->pegaUm( "select currval( 'elabrev.limitefonte_lmfid_seq'::regclass ) as mtmid" );
		}
		
		/**
		 * Captura identificadores das fontes de uma matriz de um
		 * montante ordenado pelo c�digo e descri��o da fonte.
		 *
		 * @param integer $mtmid
		 * @return string[]
		 */
		function pegar_fontes_por_montante( $mtmid )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$sql = <<<DML
SELECT fonterecurso.foncod,
       fonterecurso.fondsc,
       fonterecurso.fonabrev
  FROM elabrev.montantematriz
    INNER JOIN elabrev.fonterecursomatriz ON montantematriz.mtrid = fonterecursomatriz.mtrid
    INNER JOIN public.fonterecurso ON fonterecursomatriz.foncod = fonterecurso.foncod
  WHERE montantematriz.mtmid =  '{$mtmid}'
  ORDER BY fonterecurso.foncod,
           fonterecurso.fondsc
DML;
			$dados = $db->carregar( $sql );
			if ( !$dados )
			{
				return array();
			}
			return $dados;
		}
		
		/**
		 * Captura dados dos limites de uma unidade em rela��o �s
		 * fontes da matriz de um montante. Os limites s�o
		 * retornados ordenado pelo c�digo e descri��o da fonte.
		 * 
		 * @param integer $mtmid
		 * @param string $unicod
		 * @param string $unitpocod
		 * @return string[]
		 */
		function pegar_limites( $mtmid, $unicod, $unitpocod )
		{
			global $db;
			$sql = <<<DML
SELECT limitefonte.lmfvalor,
       fonterecurso.foncod
  FROM elabrev.montantematriz
    INNER JOIN elabrev.fonterecursomatriz ON montantematriz.mtrid = fonterecursomatriz.mtrid
    INNER JOIN public.fonterecurso ON fonterecursomatriz.foncod = fonterecurso.foncod
    LEFT JOIN elabrev.unidadematriz ON montantematriz.mtrid = unidadematriz.mtrid
    LEFT JOIN elabrev.limitefonte
      ON (fonterecurso.foncod = limitefonte.foncod AND limitefonte.unicod = unidadematriz.unicod
          AND limitefonte.unitpocod = unidadematriz.unitpocod AND montantematriz.mtmid = limitefonte.mtmid)
  WHERE montantematriz.mtmid = '{$mtmid}'
    AND unidadematriz.unicod = '{$unicod}'
    AND unidadematriz.unitpocod = '{$unitpocod}'
  ORDER BY fonterecurso.foncod,
           fonterecurso.fondsc
DML;
			$dados = $db->carregar( $sql );
			if ( !$dados )
			{
				return array();
			}
			return $dados;
		}
		
		/**
		 * Captura os dados de um montante
		 *
		 * @param integer $mtmid
		 * @return string[]
		 */
		function pegar_montante( $mtmid )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$sql = "select matriz.mtrdsc, matriz.mtrid, matriz.mtrmatriz, propostaorcamento.ppoid, propostaorcamento.ppodsc, propostaorcamento.ppoano, montantematriz.mtmvlrlimite, montantematriz.mtmid, montantematriz.mtmvlrlimite from elabrev.montantematriz inner join elabrev.matriz on montantematriz.mtrid = matriz.mtrid inner join elabrev.propostaorcamento on propostaorcamento.ppoid = montantematriz.ppoid where montantematriz.mtmid = " . $mtmid;
			$dados = $db->recuperar( $sql );
			if ( !$dados )
			{
				return array();
			}
			return $dados;
		}
		
		/**
		 * Captura dados das unidades de uma matriz ordenado
		 * pelo c�gido e nome da unidade.
		 *
		 * @param integer $mtmid
		 * @return string[]
		 */
		function pegar_unidades_por_montante( $mtmid )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$sql = "select unidade.unicod, unidade.unitpocod, unidade.unidsc, montantematriz.mtmid from elabrev.montantematriz inner join elabrev.unidadematriz on montantematriz.mtrid = unidadematriz.mtrid inner join unidade on unidadematriz.unicod = unidade.unicod and unidadematriz.unitpocod = unidade.unitpocod where montantematriz.mtmid = " . $mtmid . " order by unidade.unicod, unidade.unidsc";
			$dados = $db->carregar( $sql );
			if ( !$dados )
			{
				return array();
			}
			return $dados;
		}
		
		/**
		 * Remove todas os registros da tabela elabrev.limitefonte que
		 * pertence ao montante indicado pelo par�metro.
		 *
		 * @param integer $mtmid
		 * @return boolean
		 */
		function remove_limites_de_montante( $mtmid )
		{
			global $db;
			$mtmid = (integer) $mtmid;
			$sql_remover = "delete from elabrev.limitefonte where mtmid = " . $mtmid;
			return (boolean) $db->executar( $sql_remover );
		}
	
	// FIM FUN��ES DA P�GINA
	
	// PR�-PROCESSAMENTO
	
		// VARI�VEIS
		
			$mtmid = (integer) md5_decrypt( $_REQUEST['mtmid'], '' );
			$modulo = $_REQUEST['modulo'];
			$dados_montante = pegar_montante( $mtmid );
			$ppoid = $dados_montante['ppoid'];
		
		// FIM VARI�VEIS
		
		// MANIPULA SUBMISS�O DE FORMUL�RIO
		
			// verifica se formul�rio foi submetido
			if ( array_key_exists( 'act', $_REQUEST ) )
			{
				// verifica se o distribu�do n�o ultrapassa o valor do montante
				$soma_distribuido = 0;
				foreach ( $_REQUEST['limite'] as $limite )
				{
					foreach ( $limite[3] as $fonte_limite )
					{
						$soma_distribuido += str_replace( '.', '', $fonte_limite[1] );
					}
				}
				// caso valor distribu�do ultrapasse o limite nenhuma modifica��o no bando � realizada
				// caso o valor limite seja zero a verifica��o � 
				if ( $dados_montante['mtmvlrlimite'] <= 0 || $soma_distribuido <= $dados_montante['mtmvlrlimite'] )
				{
					// realiza requisi��es ao banco
					if ( $sucesso = remove_limites_de_montante( $mtmid ) )
					{
						// insere limites um-a-um
						// observar c�digo html para entender os �ndices num�ricos
						foreach ( $_REQUEST['limite'] as $limite )
						{
							$unicod = $limite[0];
							$unitpocod = $limite[1];
							foreach ( $limite[3] as $fonte_limite )
							{
								$lmfvalor = (float) str_replace( '.', '', $fonte_limite[1] );
								if ( $lmfvalor > 0 )
								{
									$foncod = $fonte_limite[0];
									if ( insere_limite( $mtmid, $unicod, $unitpocod, $foncod, $lmfvalor ) == 0 )
									{
										$sucesso = false;
										break;
									}
								}
							}
						}
					}
					if ( $sucesso == true )
					{
						$db->commit();
					}
				}
			}
			
		// FIM MANIPULA SUBMISS�O DE FORMUL�RIO
		
		$dados_fontes = pegar_fontes_por_montante( $mtmid );
		// verifica dados do montante e da matriz
		// somente as matrizes com o flag mtrmatriz falsos podem ser manipuladas nessa p�gina
		if ( $dados_montante['mtrmatriz'] != 'f' )
		{
			eof();
		}
		$dados_unidades = pegar_unidades_por_montante( $mtmid );
		foreach ( $dados_unidades as &$unidade )
		{
			$unidade['limites'] = pegar_limites( $mtmid, $unidade['unicod'], $unidade['unitpocod'] );
		}
		$quantidade_fontes = count( $dados_fontes ); // utilizado para controle de colunas das tabelas
		
		if( $_REQUEST['popup'] != 'true' ){
			include APPRAIZ . "includes/cabecalho.inc";
			$habilita = 'S';
		} else {
			$habilita = 'N';
			echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
					<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
					<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
		}
		
	// FIM PR�-PROCESSAMENTO

?>
<br/>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
<? 
if( $_REQUEST['popup'] != 'true' ){
monta_titulo( 'Dados da Proposta', '' ); ?>
<!-- FORMUL�RIO DE PROPOSTA OR�AMENT�RIA -->
<form action="" name="" method="post">
	<input type="hidden" name="modulo" value="principal/propostaorcamentaria/montante"/>
	<input type="hidden" name="acao" value="A"/>
	<input type="hidden" name="idproposta" value="<?= $ppoid ?>"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" >Descri��o:</td>
			<? $ppodsc = $dados_montante['ppodsc']; ?>
			<td><?= campo_texto( 'ppodsc', 'N', 'N', '', 130, 100, '', '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" >Exerc�cio:</td>
			<? $ppoano = $dados_montante['ppoano']; ?>
			<td><?= campo_texto( 'ppoano','N', 'N', '', 6, 4, '', '' ); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" >&nbsp;</td>
			<td style="margin:0;padding:0;">
				<input type="submit" name="alterar_proposta" value="Visualizar Proposta" />
			</td>
		</tr>
	</table>
</form>
<?} ?>
<!-- FIM FORMUL�RIO DE PROPOSTA OR�AMENT�RIA -->
<!-- INICIALIZA VARI�VEIS JAVASCRIPT -->
<script language="javascript">
	
	<!--
		/**
		 * Valores resultantes da soma de cada
		 * coluna ( soma por fonte ).
		 * 
		 * @var integer[]
		 */
		var coluna_distribuido = new Array();
		
		/**
		 * Indica se o usu�rio alterou em algum momento os valores de
		 * distribui��o. Caso o usu�rio altere o valore original e
		 * posteriormente retorne ao valor original esta vari�vel ser�
		 * verdadeira.
		 * 
		 * @var boolean
		 */
		var houve_modificacao = false;
		
		/**
		 * Valores resultantes da soma de cada
		 * linha ( soma por unidade ).
		 * 
		 * @var integer[]
		 */
		var linha_distribuido = new Array();
		
		/**
		 * Valor total distribu�do.
		 * 
		 * @var integer
		 */
		var total_distribuido = 0;
		
		/**
		 * Valor total limite.
		 * 
		 * @var integer
		 */
		var total_limite = <?= $dados_montante['mtmvlrlimite'] ?>;
		
		/**
		 * Valores de cada unidade para cada fonte.
		 * 
		 * @var integer[][]
		 */
		var valores = new Array();
		// inicializa valores das vari�veis para manipula��o javascript
		<? 
			foreach ( $dados_unidades as $posicao_unidade => $dados_unidade ) : ?>
			valores[<?= $posicao_unidade ?>] = new Array();
			linha_distribuido[<?= $posicao_unidade ?>] = 0;
			<? foreach ( $dados_unidade['limites'] as $posicao_limite => $dados_limite ) : ?>
				<? $lmfvalor = $dados_limite['lmfvalor'] ? $dados_limite['lmfvalor'] : 0 ; ?>
				<?php
					// define valores de soma de cada coluna para os campos do formul�rio
					// os valores definidos aqui s�o utilizados nos campos n�o edit�veis de cada coluna
					$nome_campo_coluna = 'coluna_distribuido[' . $posicao_limite . ']';
					${$nome_campo_coluna} += $lmfvalor;
				?>
				<? $total_distribuido += $lmfvalor; ?>
				if ( coluna_distribuido.length <= <?= $posicao_limite ?> )
				{
					coluna_distribuido[<?= $posicao_limite ?>] = 0;
				}
				linha_distribuido[<?= $posicao_unidade ?>] += <?= $lmfvalor ?>;
				coluna_distribuido[<?= $posicao_limite ?>] += <?= $lmfvalor ?>;
				valores[<?= $posicao_unidade ?>][<?= $posicao_limite ?>] = <?= $lmfvalor ?>;
			<? endforeach; ?>
		<? endforeach; ?>
		total_distribuido = <?= $total_distribuido ? $total_distribuido : '0' ; ?>;
		<? $diferenca = formata_numero( $dados_montante['mtmvlrlimite'] - $total_distribuido ); ?>
		// fim inicializa valores das vari�veis para manipula��o javascript
		
		/**
		 * Saldo inicial.
		 * 
		 * @var integer
		 */
		var saldo_inicial = total_limite - total_distribuido;
		
	-->
	
</script>
<!-- FIM INICIALIZA VARI�VEIS JAVASCRIPT -->
<? monta_titulo( 'Distribui��o do Montante', $dados_montante['mtrdsc'] ); ?>
<!-- FORMUL�RIO DE INSER��O / EDI��O -->
<form action="" method="post" name="formulario">
	<input type="hidden" name="modulo" value="<?= $modulo ?>">
	<input type="hidden" name="mtmid" id="mtmid" value="<?= $_REQUEST['mtmid'] ?>">
	<input type="hidden" name="act" value="1">
	
		<table class="tabela" bgcolor="#f5f5f5" width="1" cellspacing="1" cellpadding="2" align="center">
			<tr bgcolor="#cecece">
				<td align="left">&nbsp;</td>
				<? if ( $quantidade_fontes > 0 ) : ?>
					<td align="center" colspan="<?= $quantidade_fontes ?>"><b>Fontes</b></td>
				<? endif; ?>
				<td align="right">&nbsp;</td>
				<?if( $_REQUEST['popup'] != 'true' ){ ?>
					<td style="padding-left:11px;">&nbsp;</td>
				<?} ?>
			</tr>
			<tr bgcolor="#cecece">
				<td align="left" width="100%">
					<div><b>Unidades</b></div>
				</td>
				<? if ( $quantidade_fontes > 0 ) : ?>
					<? foreach ( $dados_fontes as $dado_fonte ) : ?>
						<td align="right" width="90"><?= $dado_fonte['foncod'] ?></td>
					<? endforeach; ?>
				<? endif; ?>
				<td align="right" width="90">Total</td>
				<?if( $_REQUEST['popup'] != 'true' ){ ?>
					<td>&nbsp;</td>
				<?} ?>
			</tr>
			<tr bgcolor="#dedede" <?= $dados_montante['mtmvlrlimite'] <= 0 ? 'style="display: none;"' : '' ?>>
				<td align="right">Limite Global (A):</td>
				<? if ( $quantidade_fontes > 0 ) : ?>
					<td align="right" colspan="<?= $quantidade_fontes ?>">&nbsp;</td>
				<? endif; ?>
				<td align="right">
					<? $limite = formata_numero( $dados_montante['mtmvlrlimite'] ); ?>
					<?= campo_texto( 'limite', 'N', 'N', '', 15, 14, '', 'N', 'right', '', 0, '' ) ?>
				</td>
				<?if( $_REQUEST['popup'] != 'true' ){ ?>
					<td width="17">&nbsp;</td>
				<?} ?>
			</tr>
			<tr valign="top">
				<td colspan="<?= $quantidade_fontes + 3 ?>" style="padding:0;margin:0;height:200px">
					<?if( $_REQUEST['popup'] != 'true' ){ ?>
						<div style="width: 100%; padding: 0; margin: 0; height: 200px; overflow: auto;">
					<?} ?>
						<table border="0" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
							<? if ( count( $dados_unidades ) ) : ?>
								<? $tabindex = 1; ?>
								<? foreach ( $dados_unidades as $posicao_unidade => $dados_unidade ) : ?>
									<tr bgcolor="#<?= $posicao_unidade % 2 == 0 ? 'f7f7f7' : 'ffffff' ?>">
										<td align="right" width="1" style="color:#0066cc;" title="C�digo da Unidade">
											<?= $dados_unidade['unicod'] ?>
										</td>
										<td align="left" title="Unidade Or�ament�ria">
											<?php
												// os dados de cada limitefonte esta no campo array 'limite', onde:
												// indice 0 = codigo da unidade
												// indice 1 = tipo da unidade
												// indice 2 = soma dos valores distribu�dos � unidade ( insignificante para o sistema, utilizado apenas para mostrar ao usu�rio )
												// indice 3 = array com os limites para cada fonte ( verifica sub-�ndices no pr�ximo foreach )
											?>
											<div>
												<?= $dados_unidade['unidsc'] ?>
												<input type="hidden" name="limite[<?= $posicao_unidade ?>][0]" value="<?= $dados_unidade['unicod'] ?>" />
												<input type="hidden" name="limite[<?= $posicao_unidade ?>][1]" value="<?= $dados_unidade['unitpocod'] ?>" />
											</div>
										</td>
										<? $soma_unidade = 0; // cont�m valor distribu�do para uma unidade ?>
										<? foreach ( $dados_unidade['limites'] as $posicao_limite => $dados_limite ) : ?>
											<?php
												// os dados do limite para cada fonte � um item o array 'limite', onde:
												// indice 0 = codigo da fonte ( campo hidden )
												// indice 1 = valor limite ( campo edit�vel )
												$nome_campo_valor = 'limite[' . $posicao_unidade . '][3][' . $posicao_limite . '][1]';
												${$nome_campo_valor} = formata_numero( $dados_limite['lmfvalor'] );
												$soma_unidade += $dados_limite['lmfvalor'];
												$tabindex++;
												$complemento = ' onkeyup="recalcula( this, ' . $posicao_unidade . ', ' . $posicao_limite . ' );" onblur="verifica_formato_valor( this );" tabindex="' . $tabindex . '"';
											?>
											<td align="center" width="90">
												<input type="hidden" name="limite[<?= $posicao_unidade ?>][3][<?= $posicao_limite ?>][0]" value="<?= $dados_limite['foncod'] ?>" />
												<?= campo_texto( $nome_campo_valor, 'N', $habilita, '', 15, 14, '', 'N', 'right', '', 0, $complemento ) ?>
											</td>
										<? endforeach; ?>
										<td align="right" width="90">
											<?php
												$campo_soma_unidade = 'limite[' . $posicao_unidade . '][2]';
												${$campo_soma_unidade} = formata_numero( $soma_unidade );
											?>
											<?= campo_texto( $campo_soma_unidade, 'N', 'N', '', 15, 14, '', 'N', 'right', '', 0, '' ) ?>
										</td>
									</tr>
								<? endforeach; ?>
							<? else : ?>
								<tr><td colspan="4">N�o h� unidades para esta coluna.</td></tr>
							<? endif; ?>
							</table>
					<?if( $_REQUEST['popup'] != 'true' ){ ?>
						</div>
					<?} ?>
				</td>
			</tr>
			<tr bgcolor="#cccccc">
				<td align="right">Limite Distribuido (B):</td>
				<? if ( $quantidade_fontes > 0 ) : ?>
					<? foreach ( $dados_fontes as $posicao_fonte => $dado_fonte ) : ?>
						<td align="right">
							<?
								// formata os valores da soma de cada coluna
								// os valores foram definidos no bloco de inicializa��o das vari�veis javascript
								$nome_campo_coluna = 'coluna_distribuido[' . $posicao_fonte . ']';
								${$nome_campo_coluna} = formata_numero( ${$nome_campo_coluna} );
							?>
							<?= campo_texto( 'coluna_distribuido[' . $posicao_fonte . ']', 'N', 'N', '', 15, 14, '', 'N', 'right', '', 0, '' ) ?>
						</td>
					<? endforeach; ?>
				<? endif; ?>
				<td align="right">
					<? $total_distribuido = formata_numero( $total_distribuido ); ?>
					<?= campo_texto( 'total_distribuido', 'N', 'N', '', 15, 14, '', 'N', 'right', '', 0, '' ) ?>
				</td>
				<?if( $_REQUEST['popup'] != 'true' ){ ?>
					<td>&nbsp;</td>
				<?} ?>
			</tr>
			<tr bgcolor="#cccccc" <?= $dados_montante['mtmvlrlimite'] <= 0 ? 'style="display: none;"' : '' ?>>
				<td align="right">Diferen�a (C) = (A-B):</td>
				<? if ( $quantidade_fontes > 0 ) : ?>
					<td align="right" colspan="<?= $quantidade_fontes ?>">&nbsp;</td>
				<? endif; ?>
				<td align="right">
					<?= campo_texto( 'diferenca', 'N', 'N', '', 15, 14, '', 'N', 'right', '', 0, '' ) ?>
				</td>
				<?if( $_REQUEST['popup'] != 'true' ){ ?>
					<td>&nbsp;</td>
				<?} ?>
			</tr>
			<tr bgcolor="#cccccc">
				<td colspan="<?= $quantidade_fontes + 3 ?>" style="text-align: right;" class="div_rolagem">
					<?if( $_REQUEST['popup'] != 'true' ){ ?>
						<input type="button" name="bimprimir" value="Imprimir" class="botao" onclick="javascript:imprimir();"/>
						<input type="button" name="bsubmeter" value="Salvar" class="botao" onclick="javascript:submeter_formulario();"/>
					<?} else { ?>
						<input type="button" name="bimprimir" value="Imprimir" class="botao" onclick="javascript:window.print();"/>
					<?} ?>
				</td>
			</tr>
		</table>
	
</form>
<!-- FIM FORMUL�RIO DE INSER��O / EDI��O -->
<script language="javascript">
	function imprimir(){
		var mtmid = document.getElementById('mtmid').value;
		window.open( 'elabrev.php?modulo=principal/propostaorcamentaria/limitefonte&acao=A&mtmid=' + mtmid +'&popup=true' , 'imprimirLimites', "height=700,width=900,scrollbars=yes,top=50,left=200" );
	}
	<!--
		
		/**
		 * Atualiza valores dos campos em rela��o aos valores digitados
		 * pelo usu�rio. Fun��o deve ser chamada quando algum campo que
		 * cont�m os valores � alterado.
		 * 
		 * @param string
		 * @return integer
		 */
		function recalcula( input, linha, coluna )
		{
			houve_modificacao = true;
			
			// formata valor digitado pelo usu�rio
			input.value = formata_inteiro( input.value );
			var valor = desformata_inteiro( input.value );
			
			// remove, da soma da coluna, o resultado ANTIGO do item
			coluna_distribuido[coluna] -= valores[linha][coluna];
			// remove, da soma da linha, o resultado ANTIGO do item
			linha_distribuido[linha] -= valores[linha][coluna];
			// remove, da soma do total, o resultado ANTIGO do item
			total_distribuido -= valores[linha][coluna];
			// atualiza o valor do item
			valores[linha][coluna] = valor;
			// adiciona novo valor ao item na soma da coluna
			coluna_distribuido[coluna] += valor;
			// adiciona novo valor ao item na soma da linha
			linha_distribuido[linha] += valor;
			// adiciona novo valor � soma do total
			total_distribuido += valor;
			
			// altera valor dos campos do formulario
			var elementos = document.forms['formulario'].elements;
			elementos['coluna_distribuido[' + coluna + ']'].value = formata_inteiro( coluna_distribuido[coluna] );
			elementos['limite[' + linha + '][2]'].value = formata_inteiro( linha_distribuido[linha] );
			elementos['total_distribuido'].value = formata_inteiro( total_distribuido );
			var diferenca = total_limite - total_distribuido;
			var campo_diferenca = elementos['diferenca'];
			campo_diferenca.value = formata_inteiro( diferenca );
			campo_diferenca.className = diferenca < 0 ? 'erroInput' : 'disabled' ;
			// fim altera valor dos campos do formulario
		}
		
		/**
		 * Tranforma um n�mero inteiro formatado ou n�o em um n�mero.
		 * 
		 * @param string
		 * @return integer
		 */
		function desformata_inteiro( valor )
		{
			valor += '';
			return parseInt( valor.replace( /\./g, '' ) );
		}
		
		/**
		 * Adiciona pontos a cada tr�s casas.
		 * Exemplo: entrada 12345 retorna 12.345
		 * 
		 * @param string
		 * @return string
		 */
		function formata_inteiro( valor )
		{
			valor += '';
			var caracteres_invalidos = new Array();
			for ( var i = 0; i < valor.length; i++ )
			{
				if ( isNaN( valor.charAt( i ) ) == true )
				{
					caracteres_invalidos[caracteres_invalidos.length] = valor.charAt( i );
				}
			}
			var auxiliar = caracteres_invalidos.length;
			for ( i = 0; i < auxiliar; i++ )
			{
				valor = valor.replace( caracteres_invalidos[i], '' );
			}
			while ( valor.charAt( 0 ) == '0' )
			{
				valor = valor.substr( 1 );
			}
			if ( valor.length == 0 )
			{
				valor = '0';
			}
			var resultado = '';
			auxiliar = valor.length;
			for ( var j = 0; auxiliar >= 0; j++ )
			{
				resultado = valor.charAt( auxiliar ) + resultado;
				if ( auxiliar != 0 && j > 0 && ( j % 3 ) == 0 )
				{
					resultado = '.' + resultado;
				}
				auxiliar--;
			}
			return resultado;
		}
		
		/**
		 * Submete o formul�rio caso o total distribu�do n�o ultrapasse o limite.
		 * 
		 * @return void
		 */
		function submeter_formulario()
		{
			// verifica se o valor distribu�do ultrapassou o limite
			if ( total_limite != 0 )
			{
				var saldo_atual = total_limite - total_distribuido;
				if ( saldo_inicial < 0 )
				{
					if ( saldo_atual < saldo_inicial )
					{
						alert( 'Aten��o. Os valores distribu�dos ultrapassam o limite.' );
						return;
					}
				}
				else if ( saldo_atual < 0 )
				{
					alert( 'Aten��o. Os valores distribu�dos ultrapassam o limite.' );
					return;
				}
			}
			// FIM verifica se o valor distribu�do ultrapassou o limite
			houve_modificacao = false; // evita que seja exibida um mensagem confirmando a submiss�o
			document.formulario.submit();
		}
		
		/**
		 * Realiza formata��es no campo valor que s� � poss�vel quando o
		 * foco � retirado do campo.
		 * 
		 * @param object
		 * @return void
		 */
		function verifica_formato_valor( input )
		{
			MouseBlur( input );
		}
		
		// define estilo do campo 'diferen�a' do formul�rio
		// verifica se valor distribu�do (vindo da base de dados) n�o ultrapassa limite
		document.forms['formulario'].elements['diferenca'].className = total_distribuido > total_limite ? 'erroInput' : 'disabled' ;
		
		/**
		 * M�todo chamada quanto o usu�rio quer sair da tela. Caso o
		 * formul�rio tenha sofrido alguma modifica��o um aviso �
		 * retornado.
		 * 
		 * @return string
		 */
		function sair_sem_salvar()
		{
			if ( houve_modificacao == true )
			{
				return 'Aten��o. Existem dados do formul�rio que n�o foram guardados.';
			}
		}
		
		// define m�todo para verifica��o de sa�da
		window.onbeforeunload = sair_sem_salvar;
		
	-->
	
</script>