<?php

/**
 * Renan de Lima (renandelima@gmail.com)
 * Julho 2007
 */


// ----------- FUN��ES


/**
 * @param integer
 * @return array
 */
function pegarMomentoCredito( $mcrid )
{
	global $db;
	$mcrid = (integer) $mcrid;
	$sql = "select * from elabrev.momentocredito where mcrid = " . $mcrid;
	$dados = $db->recuperar( $sql );
	return $dados ? $dados : array(); 
}

/**
 * Retorna o id do momento.
 * 
 * @param array
 * @return integer
 */
function insereMomentoCredito( $momentoCredito )
{
	global $db;
	$sql = "select nextval( 'elabrev.momentocredito_mcrid_seq' )";
	$mcrid = $db->pegaUm( $sql );
	$sql = sprintf(
		"insert into elabrev.momentocredito 
		(
			 mcrid, 
			 mcrdsc, 
			 mcrdatainiciouo, 
			 mcrdatainiciocgo, 
			 mcrdatafimcgo, 
			 mcrdatafimuo, 
			 mcrstatus,
			 mcrano
		) values ( %d, '%s', '%s', '%s', '%s', '%s', 'A' , '%s') ",
			$mcrid,
			$momentoCredito['mcrdsc'],
			$momentoCredito['mcrdatainiciouo'],
			$momentoCredito['mcrdatainiciocgo'],
			$momentoCredito['mcrdatafimcgo'],
			$momentoCredito['mcrdatafimuo'],
			$_SESSION['exercicio']
	);
	$db->executar( $sql );
	
	//insere unidades
	if($_POST['unicod'][0]){
		foreach($_POST['unicod'] as $d){
			$sql = "INSERT INTO elabrev.momentocreditounid(unicod, mcrid) VALUES ($d, $mcrid)";	
			$db->executar($sql);
		}
	}
	
	return $mcrid ? $mcrid : null;
}

/**
 * Se n�o retornar texto, a altera��o foi realizada com
 * sucesso. Caso contr�rio o texto detalha o erro que
 * aconteceu.
 * 
 * @param array
 * @return string
 */
function alterarMomentoCredito( $momentoNovo )
{
	global $db;
	// verifica se o identificador foi passado
	if ( !array_key_exists( 'mcrid', $momentoNovo ) )
	{
		return "Identificador do momento de cr�dito n�o especificado.";
	}
	// verifica se momento de cr�dito existe
	$momentoAntigo = pegarMomentoCredito( $momentoNovo['mcrid'] );
	if ( !$momentoAntigo )
	{
		return "Momento de cr�dito n�o existe.";
	}
	// deixa somente as colunas que existem na tabela
	$momentoNovo = array_intersect_key( $momentoNovo, $momentoAntigo );
	// verifica se algum dado foi enviado par altera��o
	if ( count( $momentoNovo ) < 2 )
	{
		return "";
	}
	// verifica se o status est� sendo alterado de ativo para inativo
	if (
		array_key_exists( 'mcrstatus', $momentoNovo ) &&
		$momentoAntigo['mcrstatus'] == 'A' &&
		$momentoNovo['mcrstatus'] == 'I'
	)
	{
		// verifica se h� despesas no momento atual
		$sql = sprintf(
			"select count(*) from elabrev.despesaacao where mcrid = %d ",
			$momentoNovo['mcrid']
		);
		if ( $db->pegaUm( $sql ) )
		{
			return "Status n�o pode ser alterado para inativo, pois existem despesas cadastrados no momento de cr�dito.";
		}
	}
	
	
	//insere unidades
	$sql = "DELETE from elabrev.momentocreditounid where mcrid = ".$momentoNovo['mcrid'];
	$db->executar($sql);
	if($_POST['unicod'][0] && $momentoNovo['mcrstatus'] == 'A'){
		foreach($_POST['unicod'] as $d){
			$sql = "INSERT INTO elabrev.momentocreditounid(unicod, mcrid) VALUES ($d, ".$momentoNovo['mcrid'].")";	
			$db->executar($sql);
		}
	}
	
	$set = array();
	foreach ( $momentoNovo as $campo => $valor )
	{
		if ( $campo != 'mcrid' )
		{
			array_push( $set, " " . $campo . " = '" . $valor . "' " );
		}
	}
	$sql = sprintf(
		"update elabrev.momentocredito set %s where mcrid = %d",
		implode( " , ", $set ),
		$momentoNovo['mcrid']
	);
	return $db->executar( $sql ) ? "" : "Ocorreu um erro no banco de dados."; 
}

// ----------- PROCESSA REQUISI��O -----------

$mensagem = "";
$commit   = true;
if ( array_key_exists( 'gravar', $_REQUEST ) )
{
	// captura dados
	$momento = array();
	$momento['mcrdsc']           = $_REQUEST['mcrdsc'];
	$momento['mcrdatainiciouo']  = formata_data_sql( $_REQUEST['mcrdatainiciouo'] );
	$momento['mcrdatainiciocgo'] = formata_data_sql( $_REQUEST['mcrdatainiciocgo'] );
	$momento['mcrdatafimuo']     = formata_data_sql( $_REQUEST['mcrdatafimuo'] );
	$momento['mcrdatafimcgo']    = formata_data_sql( $_REQUEST['mcrdatafimcgo'] );
	$momento['mcrstatus']        = 'A';
	$erro = array();
	if ( !$momento['mcrdsc'] )
	{
		array_push( $erro, "Descri��o � onrigat�rio." );
	}
	if ( !$momento['mcrdatainiciouo'] )
	{
		array_push( $erro, "Data de in�cio UO � onrigat�rio." );
	}
	if ( !$momento['mcrdatainiciocgo'] )
	{
		array_push( $erro, "Data de in�cio CGO � onrigat�rio." );
	}
	if ( !$momento['mcrdatafimcgo'] )
	{
		array_push( $erro, "Data de t�rmino CGO � onrigat�rio." );
	}
	if ( !$momento['mcrdatafimuo'] )
	{
		array_push( $erro, "Data de t�rmino UO � onrigat�rio." );
	}
	// faz grava��o, caso n�o existe erros
	if ( !count( $erro ) )
	{
		if ( array_key_exists( 'mcrid', $_REQUEST ) && $_REQUEST['mcrid'] )
		{
			$momento['mcrid'] = $_REQUEST['mcrid'];
			$erroAlteracao = alterarMomentoCredito( $momento );
			if ( $erroAlteracao )
			{
				array_push( $erro, $erroAlteracao );
			}
		}
		else
		{
			insereMomentoCredito( $momento );
		}
	}
	if ( count( $erro ) )
	{
		$mensagem = "Dados n�o gravados!\\n\\n" . implode( "\\n", $erro );
		$commit   = false;
	}
	else
	{
		$mensagem = "Dados gravados com sucesso!";
	}
	unset( $_REQUEST['excluir'] );
	unset( $_REQUEST['alterar'] );
}

if ( array_key_exists( 'excluir', $_REQUEST ) )
{
	$momento = array();
	$momento['mcrid']     = (integer) $_REQUEST['mcrid'];
	$momento['mcrstatus'] = 'I';
	$erro = alterarMomentoCredito( $momento );
	if ( $erro != '' )
	{
		$mensagem = "N�o foi poss�vel excluir!\\n\\n" . $erro;
		$commit   = false;
	}
	unset( $_REQUEST['excluir'] );
	unset( $_REQUEST['alterar'] );
}



// ----------- POVOA FORMUL�RIO -----------


$mcrid            = '';
$mcrdsc           = '';
$mcrdatainiciouo  = '';
$mcrdatainiciocgo = '';
$mcrdatafimcgo    = '';
$mcrdatafimuo     = '';
if ( array_key_exists( 'alterar', $_REQUEST ) )
{
	$momento = pegarMomentoCredito( $_REQUEST['mcrid'] );
	$mcrid            = $momento['mcrid'];
	$mcrdsc           = $momento['mcrdsc'];
	$mcrdatainiciouo  = $momento['mcrdatainiciouo'];
	$mcrdatainiciocgo = $momento['mcrdatainiciocgo'];
	$mcrdatafimcgo    = $momento['mcrdatafimcgo'];
	$mcrdatafimuo     = $momento['mcrdatafimuo'];
}

$commit ? $db->commit() : $db->rollback();

//dbg( $commit );
//dbg( $mensagem );

include  APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );

?>
<?php if ( $mensagem != '' ) : ?>
	<script type="text/javascript">
		alert( "<?php echo $mensagem; ?>" );
	</script>
<?php endif; ?>
<script src="../includes/calendario.js" type="text/javascript"></script>


<!-- FORMUL�RIO -->

<script type="text/javascript">

	function enviarFormulario()
	{
		var formulario = document.formulario;
		if ( !validaBranco( formulario.mcrdsc, 'Descri��o' ) ) return;
		if ( !validaBranco( formulario.mcrdatainiciouo, 'Dada in�cio UO' ) ) return;
		if ( !validaBranco( formulario.mcrdatafimuo, 'Dada fim UO' ) ) return;
		if ( !validaBranco( formulario.mcrdatainiciocgo, 'Dada in�cio CGO' ) ) return;
		if ( !validaBranco( formulario.mcrdatafimcgo, 'Dada fim CGO' ) ) return;

		selectAllOptions( formulario.unicod );
		
		document.formulario.submit();
	}
	
	function cancelar()
	{
		document.location.href = '?modulo=principal/propostaorcamentaria/configuracao/momentocredito&acao=A';
	}

</script>
<form method="post" name="formulario">
	<input type="hidden" name="gravar" value="1"/>
	<input type="hidden" name="mcrid" value="<?= $mcrid ?>"/>
	<table align="center" class="tabela" width="90%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td align="right" class="SubTituloDireita" style="vertical-align:top; width:25%;">
				Descri��o:
			</td>
			<td>
				<?= campo_texto( 'mcrdsc', 'S', 'S', 'Descri��o', 80, 100, '', 'N' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidades:</td>	
			<td valign="top">
				<?php
					$sql_combo = "select a.unicod as codigo, a.unicod ||' - '|| u.unidsc as descricao
								 from monitora.acao a 
								 inner join unidade u on a.unicod = u.unicod 
								 where prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A' 
								 group by a.unicod, u.unidsc, a.prgano 
								 order by 2";
	
					$wherePesqUnid = array(
								array(
										"codigo" 	=> "u.unidsc",
										"descricao" => "Unidade:",
										"numeric"	=> false
							   		  )
							   );

				   if($mcrid){
						$sql = "select a.unicod as codigo, a.unicod ||' - '|| u.unidsc as descricao
								 from monitora.acao a 
								 inner join unidade u on a.unicod = u.unicod 
								 inner join elabrev.momentocreditounid mu on mu.unicod = u.unicod
								 where prgano = '".$_SESSION['exercicio']."'  and a.acastatus='A'
								 and mu.mcrid = $mcrid  
								 group by a.unicod, u.unidsc, a.prgano 
								 order by 2";
						$unicod = $db->carregar($sql);
				   }							   
							   
					combo_popup( 'unicod', $sql_combo, 'Selecione a(s) Unidade(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, null, $wherePesqUnid, null, true, false, null, true);
					echo obrigatorio();				
		    	?>		
			</td>
		</tr>		
		<tr>
			<td
				align="right"
				class="SubTituloDireita"
				style="vertical-align: middle; width:25%;"
			>
				Per�odo UO:
			</td>
			<td>
				<?= campo_data( 'mcrdatainiciouo', 'S', 'S', 'Dada in�cio UO', 'S' ); ?>
				<br/>
				<?= campo_data( 'mcrdatafimuo', 'S', 'S', 'Dada fim UO', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td
				align="right"
				class="SubTituloDireita"
				style="vertical-align: middle; width:25%;"
			>
				Per�odo CGO:
			</td>
			<td>
				<?= campo_data( 'mcrdatainiciocgo', 'S', 'S', 'Dada in�cio CGO', 'S' ); ?>
				<br/>
				<?= campo_data( 'mcrdatafimcgo', 'S', 'S', 'Dada fim CGO', 'S' ); ?>
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td>&nbsp;</td>
			<td>
				<input
					type="button"
					name="botao"
					value="<?= $mcrid ? 'Alterar' : 'Inserir'; ?>"
					onclick="enviarFormulario();"
				/>
				&nbsp;&nbsp;&nbsp;
				<input
					type="button"
					name="botao"
					value="Cancelar"
					onclick="cancelar();"
				/>
			</td>
		</tr>
	</table>
</form>

<!-- FIM : FORMUL�RIO -->

<!-- LISTAGEM -->


<script type="text/javascript">
	
	function alterarMomentoCredito( mcrid )
	{
		document.location.href = '?modulo=principal/propostaorcamentaria/configuracao/momentocredito&acao=A' +
			'&alterar=1&' +
			'mcrid=' + mcrid;
	}
	
	function excluirMomentoCredito( mcrid )
	{
		document.location.href = '?modulo=principal/propostaorcamentaria/configuracao/momentocredito&acao=A' +
			'&excluir=1&' +
			'mcrid=' + mcrid;
	}
	
</script>
<?php
	$sql =
		"select
			'<img
                                style=\"cursor:pointer;\"
				border=\"0\"
				src=\"../imagens/alterar.gif\"
				title=\"Alterar Momento de Cr�dito\"
				onclick=\"alterarMomentoCredito( ' || mcrid || ' )\">
			&nbsp;&nbsp;&nbsp;
			<img
                                style=\"cursor:pointer;\"
				border=\"0\"
				src=\"../imagens/excluir.gif\"
				title=\"Excluir Momento de Cr�dito\"
				onclick=\"excluirMomentoCredito( ' || mcrid || ' )\">' as acao,
			mcrdsc, 
			to_char(mcrdatainiciouo,'dd/mm/YYYY') as mcrdatainiciouo,
			to_char(mcrdatafimuo,'dd/mm/YYYY') as mcrdatafimuo,
  			to_char(mcrdatainiciocgo,'dd/mm/YYYY') as mcrdatainiciocgo,
  			to_char(mcrdatafimcgo,'dd/mm/YYYY') as mcrdatafimcgo
  			
		from elabrev.momentocredito
		where
			mcrstatus = 'A' and mcrano = '".$_SESSION['exercicio']."'
		order by
			mcrdsc";
		
	$cabecalho = array( 'A��es', 'Descri��o','Dt. In�cio UO','Dt. Fim UO','Dt. In�cio CGO','Dt. Fim CGO' );
	$db->monta_lista( $sql, $cabecalho, 200, 20, '', '', '' );
?>
<!-- FIM : LISTAGEM -->

