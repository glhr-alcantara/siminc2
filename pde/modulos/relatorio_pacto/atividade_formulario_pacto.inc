<?php

if ( isset( $_REQUEST['verid'] ) )
{
	$sql = sprintf(
		"select
			v.verid,
			v.vernome,
			v.vertipomime,
			a.atiid
		from pde.versaoanexoatividade v
			inner join pde.anexoatividade a on
				a.aneid = v.aneid
		where v.verid = %d",
		$_REQUEST['verid']
	);
	$versao = $db->pegaLinha( $sql );
	$caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] . '/' . $versao['verid'];
	if ( !is_readable( $caminho ) ) {
		redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
	}
	header( 'Content-type: '. $versao['vertipomime'] );
	header( 'Content-Disposition: attachment; filename=' . $versao['vernome'] );
	readfile( $caminho );
	exit();
}

// VERIFICA SE PROJETO EST� SELECIONADO
projeto_verifica_selecionado();

if ( isset( $_REQUEST['submetido'] ) )
{
	include 'atividade_resultado_pacto.inc';
	exit();
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>"; 
montar_titulo_projeto();
//monta_titulo( 'Plano de Desenvolvimento da Educa��o', 'Relat�rio' );

include APPRAIZ . 'includes/Agrupador.php';

$sql_combo_nivel_inicial = "
	select
		da.profundidade,
		da.atiid,
		da.numero,
		a.atidescricao
	from pde.f_dadostodasatividades() da
		inner join pde.atividade a on
			a.atiid = da.atiid
	where
		da.profundidade < 4 and
		da.projeto = " . PROJETO . " 
	order by
		da.ordem
";
$niveis_iniciais = $db->carregar( $sql_combo_nivel_inicial );
$niveis_iniciais = $niveis_iniciais ? $niveis_iniciais : array();

?>
<script type="text/javascript" src="/includes/calendario.js"></script>
<script type="text/javascript">
	
	var resultado_popup = null;
	
	function submeterFormulario()
	{
		var form = document.formulario;
		form.target = '_blank';
		selectAllOptions( form.colunas );
		if (form.estrategica)
			selectAllOptions( form.estrategica );
				
		if(validar_from()){
		
			form.submit();
		}
		
		
		
	}
	
	function validar_from(){ 
	
		if((document.formulario.data_inicio_0.value.length < 10) && ( document.formulario.data_inicio_0.value.length > 0 )){
			alert('Primeira data inicial preenchida incorretamente.');
			return false;
		}else if((document.formulario.data_inicio_1.value.length < 10) && ( document.formulario.data_inicio_1.value.length  > 0 )){
			alert('Segunda data inicial preenchida incorretamente.');
			return false;
		} else if((document.formulario.data_termino_0.value.length < 10) && ( document.formulario.data_termino_0.value.length  > 0 )){
			alert('Primeira data de t�rmino preenchida incorretamente.');
			return false;
		}else if((document.formulario.data_termino_1.value.length < 10) && ( document.formulario.data_termino_1.value.length  > 0 )){
			alert('Segunda data de t�rmino preenchida incorretamente.');
			return false;
		}else return true;
	}

</script>
<form action="" name="formulario" method="post">
	<input type="hidden" name="submetido" value="1"/>
	<table align="center" border="0" cellspacing="1" cellpadding="3" class="tabela" bgcolor="#f5f5f5">
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Colunas Vis�veis</b>
			</td>
			<td>
				<?php
					$html_agrupador =
					<<<EOF
						<table>
							<tr valign="middle">
								<td>
									<select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="5" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
								</td>
								<td>
									<img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
									<!--
									<img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
									<img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
									-->
									<img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
								</td>
								<td>
									<select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="5" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
								</td>
								<td>
									<img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
									<img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
								</td>
							</tr>
						</table>
						<script type="text/javascript" language="javascript">
							limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
							limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
							{POVOAR_ORIGEM}
							{POVOAR_DESTINO}
							sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
						</script>
EOF;
					$campoColuna = new Agrupador( 'formulario', $html_agrupador );
					$campos = array(
						array(
							'codigo'    => 'descricao',
							'descricao' => 'Nome / Descri��o'
						),
						array(
							'codigo'    => 'data_inicio',
							'descricao' => 'Data In�cio'
						),
						array(
							'codigo'    => 'data_fim',
							'descricao' => 'Data Fim'
						),
						array(
							'codigo'    => 'observacoes',
							'descricao' => 'Observa��es'
						),
						array(
							'codigo'    => 'produtos',
							'descricao' => 'Produtos Gerados - Entrada/Sa�da'
						)
						
					);
					$campoColuna->setOrigem( 'colunas_invisiveis', null, $campos );
					$campoColuna->setDestino( 'colunas' );
					$campoColuna->exibir();
					
				?>
			</td>
		</tr>
<?php if (usuario_possui_perfil( PERFIL_ASSESSOR )): ?>		
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Estrutura</b>
			</td>
			<td>
				<?				
				function agrupador(){
						return array(
				array('codigo' 	  => 'E',
					  'descricao' => 'Etapa'),	
				array('codigo' 	  => 'P',
					  'descricao' => 'Processo'),
				array('codigo' 	  => 'S',
					  'descricao' => 'Subprocesso'),				
				array('codigo' 	  => 'A',
					  'descricao' => 'Atividade')				

				);
}	
				?>
				<?
					$matriz = agrupador();
					$campoAgrupador = new Agrupador( 'formulario' );
					$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
					$campoAgrupador->setDestino( 'estrategica', null);
					$campoAgrupador->exibir();
				
				
						  $sql = "SELECT
									'E' as codigo,
									'Etapa' as descricao
								  UNION ALL
								  SELECT
									'P' as codigo,
									'Processo' as descricao
								UNION ALL
								  SELECT
									'S' as codigo,
									'SubProcesso' as descricao
									UNION ALL
								  SELECT
									'A' as codigo,
									'Atividade' as descricao";
					$matriz = (array) $db->carregar($sql);
				?>
			</td>
		</tr>
<?php endif; ?>				
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>N�vel Inicial</b>
			</td>
			<td>
				<select id="atiidraiz" name="atiidraiz" class="CampoEstilo" style="width: 250px;">
					<option value="">
						<?php
							$sql = "select atidescricao from pde.atividade where atiid = " . PROJETO;
							echo $db->pegaUm( $sql );
						?>
					</option>
					<?php foreach ( $niveis_iniciais as $item ) : ?>
						<option
							value="<?=  $item['atiid'] ?>"
							<?= $item['atiid'] == $atiidraiz ? 'selected="selected"' : '' ?>
						>
							<?= str_repeat( '&nbsp;', $item['profundidade'] * 5 ) ?>
							<?=  $item['numero'] ?>
							<?=  $item['atidescricao'] ?>
						</option>
					<?php endforeach; ?>
				</select>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>N�veis Vis�veis</b>
			</td>
			<td>
				<select name="niveisvisiveis" class="CampoEstilo">
					<option value="1">1</option>
					<option value="2">2</option>
					<option value="3" selected="selected">3</option>
					<option value="4">4</option>
					<option value="-1">Todos</option>
				</select>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Execu��o</b>
			</td>
			<td>
				<select name="execucao" class="campoEstilo">
					<option value="-1"></option>
					<option value="0">0 %</option>
					<option value="10">10 %</option>
					<option value="20">20 %</option>
					<option value="30">30 %</option>
					<option value="40">40 %</option>
					<option value="50">50 %</option>
					<option value="60">60 %</option>
					<option value="70">70 %</option>
					<option value="80">80 %</option>
					<option value="90">90 %</option>
					<option value="100">100 %</option>
				</select>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Situa��o</b>
			</td>
			<td>
				<?php
					$sql = "select esaid as codigo, esadescricao as descricao from pde.estadoatividade order by esadescricao";
					$db->monta_combo( 'situacao', $sql, 'S', 'Escolha uma situa��o', '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Restri��es</b>
			</td>
			<td>
				<input
					type="radio"
					name="restricao"
					id="restricao_superada"
					value="1"
					<?= $_REQUEST['restricao'] == '1' ? 'checked="checked"' : '' ?>
				/>
				<label for="restricao_superada">Superadas</label>
				
				&nbsp;&nbsp;&nbsp;&nbsp;
				
				<input
					type="radio"
					name="restricao"
					id="restricao_naosuperada"
					value="0"
					<?= $_REQUEST['restricao'] == '0' ? 'checked="checked"' : '' ?>
				/>
				<label for="restricao_naosuperada">N�o superadas</label>
				
				&nbsp;&nbsp;&nbsp;&nbsp;
				
				<input
					type="radio"
					name="restricao"
					id="restricao_todas"
					value=""
					<?= $_REQUEST['tipo'] == '' ? 'checked="checked"' : '' ?>
				/>
				<label for="restricao_todas">Todas</label>
			</td>
		</tr>
<!-- 		
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Atividades Estrat�gicas</b>
			</td>
			<td>
				<input
					type="radio"
					name="estrategica"
					id="estrategica_s"
					value="0"
					<?= $_REQUEST['estrategica'] == '0' ? 'checked="checked"' : '' ?>
				/>
				<label for="restricao_superada">Estrat�gicas</label>
				
				&nbsp;&nbsp;&nbsp;&nbsp;
				
				<input
					type="radio"
					name="estrategica"
					id="estrategica_n"
					value="1"
					<?= $_REQUEST['estrategica'] == '1' ? 'checked="checked"' : '' ?>
				/>
				<label for="restricao_naosuperada">N�o Estrat�gicas</label>
				
				&nbsp;&nbsp;&nbsp;&nbsp;
				
				<input
					type="radio"
					name="estrategica"
					id="estrategica_todas"
					value=""
					<?//= $_REQUEST['estrategica'] == '2' ? 'checked="checked"' : '' ?>
					<?= $_REQUEST['tipo'] == '' ? 'checked="checked"' : '' ?>
				/>
				<label for="restricao_todas">Todas</label>
			</td>
		</tr>
 -->		
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>In�cio</b>
			</td>
			<td>
				de&nbsp;&nbsp;
				<?=
					campo_data(
						'data_inicio_0',
						'N',
						'S',
						'Data de in�cio',
						''
					);
				?>
				&nbsp;&nbsp;at�&nbsp;&nbsp;
				<?=
					campo_data(
						'data_inicio_1',
						'N',
						'S',
						'Data de in�cio',
						''
					);
				?>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>T�rmino</b>
			</td>
			<td>
				de&nbsp;&nbsp;
				<?=
					campo_data(
						'data_termino_0',
						'N',
						'S',
						'Data de in�cio',
						''
					);
				?>
				&nbsp;&nbsp;at�&nbsp;&nbsp;
				<?=
					campo_data(
						'data_termino_1',
						'N',
						'S',
						'Data de in�cio',
						''
					);
				?>
			</td>
		</tr>
		<!--<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Per�odo de Or�amento</b>
			</td>
			<td>
				de&nbsp;&nbsp;
				<select name="orcamento_inicio" class="campoEstilo">
					<option value=""></option>
					<?php foreach ( range( 2004, date( 'Y' ) + 4 ) as $ano ) : ?>
						<option value="<?= $ano ?>">
							<?= $ano ?>
						</option>
					<?php endforeach; ?>
				</select>
				a
				<select name="orcamento_fim" class="campoEstilo">
					<option value=""></option>
					<?php foreach ( range( 2004, date( 'Y' ) + 4 ) as $ano ) : ?>
						<option value="<?= $ano ?>">
							<?= $ano ?>
						</option>
					<?php endforeach; ?>
				</select>
				<input type="checkbox" id="orcamento_agrupar" name="orcamento_agrupar" value="1"/>
				<label for="orcamento_agrupar">agrupar</label>
			</td>
		</tr>
		--><tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Minhas Atividades</b>
			</td>
			<td>
				<input type="checkbox" id="responsabilidade" name="responsabilidade" value="1"/>
				<label for="responsabilidade">listar apenas atividades sob minha responsabilidade</label>
			</td>
		</tr>
<!--		<tr>-->
<!--			<td align="right" width="20%" class="SubTituloDireita">-->
<!--				<b>Possui Plano Interno:</b>-->
<!--			</td>-->
<!--			<td>-->
<!--				<input type="checkbox" id="planointerno" name="planointerno" value="true"/>-->
<!--				<label for="responsabilidade">listar apenas atividades que possuam Plano Interno</label>-->
<!--			</td>-->
<!--		</tr>		-->
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				<b>Respons�veis</b>
			</td>
			<td>
				<?php
					$sql_combo = "
						SELECT DISTINCT
							ent.entid as codigo,
							ent.entnome as descricao
						FROM
							entidade.entidade ent
						INNER JOIN
							pde.responsavelatividade rpa ON rpa.entid = ent.entid
										    AND rpa.rpastatus = 'A'
						WHERE
							ent.entstatus = 'A'";	
					
					$db->monta_combo(	
						'responsavel',	// nome do campo
						$sql_combo,		// sql...
						'S',			// habilitado
						'Respons�vel',	// label
						'', ''
					);
				?>
			</td>
		</tr>
		<tr>
			<td align="right" width="20%" class="SubTituloDireita">
				&nbsp;
			</td>
			<td bgcolor="#dcdcdc">
				<input type="button" name="submeter" value="Visualizar" onclick="submeterFormulario();"/>
			</td>
		</tr>
	</table>
</form>