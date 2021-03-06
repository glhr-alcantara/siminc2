<?php
ini_set( "memory_limit", "1024M" );
set_time_limit(0);

	/*
		Sistema Simec
		Setor respons�vel: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabr�cio Mendon�a (e-mail: fabriciomendonca@gmail.com)
		M�dulo: relatorioQuadroCreditos.inc
		Finalidade: relat�rio dos quadros or�amentarios, detalha os gastos da unidade por a��o
	*/

	// FUN��ES GERA TIPO
	
	/**
	 * Remove (ATRIBUI STATUS INATIVO) todas as entradas de
	 * tipo de detalhamento atribu�do as unidades de uma
	 * determinadas proposta.
	 *
	 * @param integer $ppoid
	 * @param string $unicod
	 * @param string $unitpocod
	 * @return boolean
	 */
	function remover_tipo_unidade( $unicod, $unitpocod )
	{
		global $db;
		//$ppoid = (integer) $ppoid;
		//$sql = "delete from elabrev.tpdunidade where ppoid = " . $ppoid;
		$sql =
			" update elabrev.tpdunidade set " .
				" tpustatus = false " .
			" where " .
				//" ppoid = " . $ppoid . " and " .
				" unicod = '". $unicod ."' and " .
				" unitpocod = '". $unitpocod . "'";
		return (boolean) $db->executar( $sql );
	}
	
	/**
	 * Remove (ATRIBUI STATUS INATIVO) todas as entradas de
	 * tipo de detalhamento atribu�do as unidades de uma
	 * determinadas proposta.
	 *
	 * @param string $unicod
	 * @param string $unitpocod
	 * @param string $tpdcod
	 * @return boolean
	 */
	function inserir_tipo_unidade( $unicod, $unitpocod, $tpdcod )
	{
		if ( !remover_tipo_unidade( $unicod, $unitpocod ) )
		{
			return false;
		}
		if ( $tpdcod == '0' )
		{
			return true;
		}
		global $db;
		$ppoid = (integer) $ppoid;
		$sql =
			" insert into elabrev.tpdunidade " .
				//" ( ppoid, tpdcod, unicod, unitpocod, tpustatus ) " .
				" ( tpdcod, unicod, unitpocod, tpustatus ) " .
			" values " .
				" ( " .
					//"0, " .
					"'" . $tpdcod . "', " .
					"'" . $unicod. "'," .
					"'" . $unitpocod. "'," .
					"true" .
				" ) ";
		return (boolean) $db->executar( $sql );
	}
	
	/**
	 * Verifica se usu�rio possui permiss�o para manipular a despesa.
	 *
	 * @param string $unicod
	 * @param string $unitpocod
	 * @return booelan
	 */
	function verifica_permissao_momento( $unicod, $unitpocod )
	{
		global $db;
		// captura o momento
		$sql =
			"select tpdcod " .
			" from elabrev.tpdunidade tu " .
				" inner join elabrev.tipodetalhamento td using ( tpdcod ) " .
			" where " .
				" tu.unicod = '" . $unicod . "' and " .
				" tu.unitpocod = '" . $unitpocod . "' and ".
				" tu.tpustatus = true ";
		$tpdcod = trim( $db->pegaUm( $sql ) );
		// captura o momento
		switch ( $tpdcod )
		{
			case '2': // SPO
				$cpf = $_SESSION['usucpf'];
				$sql =
					" select count(*) " .
					" from seguranca.perfilusuario " .
						" inner join seguranca.perfil using ( pflcod ) " .
					" where " .
						" usucpf = '{$cpf}' and " .
						" sisid = '2' and " .
						" pflcod not in ( " .
							" select pflcod from elabrev.tprperfil " .
						" )";
				return (boolean) $db->pegaUm( $sql );
			case '3': // SOF
				return false;
			default:
			case '1': // UO
				return $db->usuarioPossuiPermissaoUnidade( $unicod );
		}
	}
	
	/**
	 * Verifica se usu�rio autenticado possui perfil de coordenador de UO.
	 *
	 * @return boolean
	 */
	function coordenador_uo()
	{
		global $db;
		$cpf = $_SESSION['usucpf'];
		$sql = "select count(*) from seguranca.perfilusuario where usucpf = '" . $cpf . "' and pflcod in ( 53 ) ";
		return (boolean) $db->pegaUm( $sql );
	}
	
	// FUN��ES GERA TIPO

	
if ($_REQUEST['relat']=='planilha_lista')
{
	unset ($_REQUEST['relat']);
	if ($_REQUEST['planilha']=='4')
	{
    header('Content-type: application/xls');
    header('Content-Disposition: attachment; filename="planilha_simec.xls"');
    $proximaLinha = "\n";
    $proximaColuna = "\t"; 
	}
	else if ($_REQUEST['planilha']=='6')
	{
    header('Content-type: application/csv');
    header('Content-Disposition: attachment; filename="planilha_simec.csv"');
    $proximaLinha = "\n";
    $proximaColuna = ","; 
	}

/*

	    if ($_SESSION['LINHA'] >= 0) 
        {
        for ($i=0; $i<=$_SESSION['LINHA'];$i++)
        	{
            $res = $db->carrega_registro($RS,$i);
            // a linha abaixo transforma em vari�veis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
 	       $_SESSION['LIN'][$i][0] = $codigo;
		   $_SESSION['LIN'][$i][1] = $descricao;
		   $_SESSION['LIN'][$i][2] = $vllimite;
		   $_SESSION['LIN'][$i][3] = $vlDespesa;
     	    }
        }
*/
    for ($i=0;$i<count($_SESSION['Query']);$i++)
    {
	    $RS = $db->record_set($_SESSION['Query'][$i]);
  	    $_SESSION['conteudo']=  $db->carrega_tudo($RS); 

	    $_SESSION['LINHA'] = $db->conta_linhas($RS);
	    $_SESSION['title'] = $tit;
	    $_SESSION['TITULO']= 'Relat�rio Gerencial de A��o';
	    $_SESSION['TIT'][0][0] = 'C�digo ';
	    $_SESSION['TIT'][0][1] = 'T�tulo';
	    $_SESSION['data']= date('(h:m) d/m/Y');
	    $_SESSION['LIN'] = array();

	    $_SESSION['cab'] = Array();
		for ($a=0;$a<pg_num_fields($RS);$a++)
		{
			$_SESSION['cab'][$a] = pg_field_name($RS,$a); 
		}
	
	    for ($a=0;$a<count($_SESSION['cab']);$a++)
	    {
		    if ($a<count($_SESSION['cab'])-1) print trim(($_SESSION['cab'][$a])). ' ' . $proximaColuna;
		    else print($_SESSION['cab'][$a]);
	    }
    	
	    print $proximaLinha;

	    if ($_SESSION['LINHA'] >= 0) 
        {
        for ($a=0; $a<=$_SESSION['LINHA'];$a++)
        	{
            $res = $db->carrega_registro($RS,$a);
            // a linha abaixo transforma em vari�veis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
//	       $_SESSION['conteudo'][$i][0] = $codigo;
//		   $_SESSION['conteudo'][$i][1] = $descricao;
//		   $_SESSION['conteudo'][$i][2] = $vllimite;
//		   $_SESSION['conteudo'][$i][3] = $vlDespesa;
     	    }
        }

        if (count($_SESSION['conteudo'][0])>1)
	    {
	    for ($a=0;$a<count($_SESSION['conteudo']);$a++)
	    {
	        foreach($_SESSION['conteudo'][$a] as $k=>$v)
	        {
	        	if ($k<>'acao'){
	            ${$k}=$v;
	            if ($v == 't') $v='1';
	            else if ($v=='f') $v='0';
//	            if (! is_int($k) and $v <>'') 
	            //if ($v <>'') 
	            //{
//                   if (is_numeric($v)) $v=$v;
	               $v = str_replace(",","",$v);
	               print $v . $proximaColuna;
	            //}
	        	}
		     }
		     
		     print $proximaLinha;
		     
	     }	
	   }

    
    }
		 exit();

    
}	
	
//$unidade = $_REQUEST['unidade'];
$unidade = $_SESSION['unicod_visualizacao'];
$NomeUnidade = $db->pegaUm( "select unicod || '  ' || unidsc from unidade where unicod = '" . $unidade . "'" );
$ppoid = $db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoanoexercicio = '" . $_SESSION['exercicio'] . "'" );
if ( !$ppoid )
{
	?>
		<script language="javascript">
			alert( 'N�o foi encontrado a proposta ativa para o Exerc�cio de <?= $_SESSION['exercicio'] ?> !' );
			history.back(-1);
			self.close();
		</script>
	<?
	exit;
}

include APPRAIZ . "includes/cabecalho.inc";

// MANIPULA REQUISICAO GERA TIPO
	$unicod = $unidade;
	$unitpocod = 'U';

	$coord_uo = coordenador_uo();
	$permissao_momento = verifica_permissao_momento( $unicod, $unitpocod );
	if ( $_REQUEST['gera_tipo'] && $coord_uo && $permissao_momento )
	{
		if ( inserir_tipo_unidade( $unicod, $unitpocod, '2' ) )
		{
			$db->commit();
			?>
				<script type="text/javascript">
					alert( 'Envio para a SPO efetuada com sucesso!' );
				</script>
			<?
		}
	}
	
// FIM MANIPULA REQUISICAO GERA TIPO

if ( !$unidade and $_SESSION["unicod_visualizacao"] )
{
	$unidade = $_SESSION["unicod_visualizacao"];
}
else if ( !$unidade and !$_SESSION["unicod_visualizacao"] )
{
	exit(
		"<script type=\"text/javascript\">
			alert( 'Selecione a Unidade para Visualizar o Detalhamento das Despesas!' );
		</script>
		<script type=\"text/javascript\">
			document.location.href = \"elabrev.php?modulo=principal/propostaorcamentaria/despesa&acao=A\";
		</script>"
	);
}

?>
<br/>
<? $db->cria_aba( $abacod_tela, $url, '' ); ?>
<? monta_titulo( $titulo_modulo, 'Clique no c�digo da A��o ou na Natureza de Despesa para editar o detalhamento' ); ?>
<form method="POST" name="formulario" style="margin: 0;">
	<input type="hidden" name="ppoid" value="<?=$ppoid?>">
	<input type="hidden" name="unidade" value="<?=$unidade?>">
	<input type="hidden" name="acao" value="">

	<input type='hidden' name="relat" >
	<input type='hidden' name="planilha" >
	
	<script>
		var elementos = new Array();
	</script>
	
	<!-- CABECALHO -->
	<table class="tabela" cellSpacing="0" width="95%" bgcolor="#f5f5f5" cellPadding="2" border="0" align="center">
		<tr>
			<td>
				<table cellSpacing="0" width="100%" cellPadding="2" border="0" align="center">
					<tr>
						<td width="50" align="left" class="TituloTela" style="color:#000000;">
							�rg�o:
						</td>
						<td width="100%" align="left" class="TituloTela" style="color:#000000;">
							&nbsp;&nbsp;<?php echo CODIGO_ORGAO_SISTEMA. ' '. NOME_ORGAO_SISTEMA; ?>
						</td>
					</tr>
					<tr>
						<td align="left" class="TituloTela" style="color:#000000;">
							Unidade:
						</td>
						<td align="left" class="TituloTela" style="color:#000000;">
							&nbsp;&nbsp;<?= $NomeUnidade ?>
						</td>
					</tr>
					<? //if ( $permissao_momento && $coord_uo ) : ?>
					<!--	<tr>
							<td colspan="2" align="left" class="TituloTela" style="color:#000000;">
								<input type="button" name="salvar" value="Enviar para a SPO (Gera Tipo)" onclick="javascript:submeter_gera_tipo();"/>
							</td>
						</tr> -->
					<? //endif; ?>
				</table>
				<table cellSpacing="0" width="100%" bgcolor="#e9e9e9" cellPadding="2" border="0" align="center">
					<tr>
						<td align="left" valign="top">
							Quadro dos Cr�ditos Or�ament�rios
							<br />
							<a href="javascript:void(0);" id="abre_fecha" onclick="abrirFecharTodos();"><img border="0" src="/imagens/mais.gif" /> Abrir todos</a>
						</td>
						<td align="right"><b>R$ 1,00</b><br/>Recursos de todas as Fontes</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
	<!-- LISTAGEM DOS PROGRAMAS -->
	<?
	
	// define filtro fixo para todas as querys
	$filtro_fixo = " a.prgano = '" . $_SESSION['exercicio'] . "' and " .
			 	   " a.acastatus = 'A' and " .
			 	   " a.unicod = '" . $unidade . "' and " .
			 	   " a.acasnrap = false and " .
				   " ac.ppoid = " . $ppoid;
	// FIM define filtro fixo para todas as querys
	
	// captura os programas
	$Query = "select 
			 		ppa.prgcod, ppa.prgdsc, 
			 		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor, 
			 		ppa.prgid 
			 	from elabrev.despesaacao ac 
			 		inner join elabrev.ppaacao_orcamento a using ( acaid ) 
			 		inner join elabrev.ppaprograma_orcamento ppa using ( prgid ) 
			 	where " . $filtro_fixo . "
			 	group by 
			 		ppa.prgid, ppa.prgcod, ppa.prgdsc 
			 	order by 
			 		ppa.prgid, ppa.prgcod, ppa.prgdsc ";

	$RS1 = $db->record_set($Query);
	$nlinhas1 = $db->conta_linhas($RS1);
	$idtr = 0;
	// FIM captura os programas
	if ( $nlinhas1 < 0 )
	{
		?>
			<p align="center" style="color: #904040;">
				<br />
				N�o existe detalhamento or�ament�rio para esta unidade.
			</p>
		<?
	}
	else
	{
		?>
		<table class="tabela" border="1" width="95%" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="2" align="center">
		<?
		
	  	// percorre os programas
	  	$vlTotalPrograma = 0;
	  	for ( $i = 0 ; $i <= $nlinhas1; $i++ )
		{
			/*
				$resPrograma[0] => prgcod
				$resPrograma[1] => prgdsc
				$resPrograma[2] => valor
				$resPrograma[3] => prgid
				$resPrograma[4] => prgid
			*/
		  	$resPrograma = $db->carrega_registro( $RS1, $i );
		  	$vlTotalPrograma += $resPrograma[2];
		  	
				?>
			<tr bgcolor="#DCDCDC">
				<td width="15%" align="center" class="SemBorda"><b>Program�tica</b></td>
				<td width="55%" align="center" class="SemBorda"><b>Programa/A��o/Localiza��o/Produto/Natureza</b></td>
				<td width="3%" align="center" class="SemBorda"><b>Funcional</b></td>
				<td width="3%" align="center" class="SemBorda"><b>Esf</b></td>
				<td width="3%" align="center" class="SemBorda"><b>Nat</b></td>
				<td width="3%" align="center" class="SemBorda"><b>IU</b></td>
				<td width="3%" align="center" class="SemBorda"><b>Fte</b></td>
				<td width="15%" align="right" class="SemBorda"><b>Valor</b></td>
			</tr>
                        <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
                                <!-- PROGRAMA CODIGO -->
                                <td align="left" class="SemBorda">
                                        <b>&nbsp;<?= $resPrograma[0] ?></b>
                                </td>
                                <!-- PROGRAMA DESCRICAO -->
                                <td class="SemBorda" style="padding: 10px;" align="left">
                                        <b><?= $resPrograma[1] ?>&nbsp;</b>
                                </td>
                                <td align="left" class="SemBorda">&nbsp;&nbsp;</td>
                                <td align="left" class="SemBorda">&nbsp;&nbsp;</td>
                                <td align="left" class="SemBorda">&nbsp;&nbsp;</td>
                                <td align="left" class="SemBorda">&nbsp;&nbsp;</td>
                                <td align="left" class="SemBorda">&nbsp;&nbsp;</td>
                                <!-- PROGRAMA VALOR -->
                                <td align="right" class="SemBorda">
                                        <b>&nbsp;<?= formata_valor( $resPrograma[2], 0 ) ?></b>
                                </td>
                        </tr>
			   
				<?
				
				// captura as a��es
				$Query = "select 
						 		a.acacod, a.acadsc, a.funcod, a.sfucod, 
						 		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor, t.taccod, t.tacdsc  
						 	from elabrev.despesaacao ac 
						 		inner join elabrev.ppaacao_orcamento a on a.acaid = ac.acaid 
						 		inner join elabrev.ppafuncao f on f.funcod = a.funcod 
						 		inner join elabrev.ppasubfuncao s on s.sfucod = a.sfucod 
						 		left join tipoacao t on t.taccod = a.taccod 	
						 	where " . $filtro_fixo . " and 
						 		a.prgid = " . $resPrograma[3] . "
						 	group by 
						 		a.acacod, a.acadsc, a.funcod, a.sfucod, t.taccod, t.tacdsc 
						 	order by 
						 		t.tacdsc, a.acacod, a.acadsc, a.funcod, a.sfucod";
			    $RS2 = $db->record_set( $Query );
			    $nlinhas2 = (integer) $db->conta_linhas( $RS2 );

			    //dbg( $Query );
			    
			    // FIM captura as a��es
				// percorre as a��es
			  	for ( $j = 0; $j <= $nlinhas2; $j++ )
				{
					/*
						$resAcao[0] => acacod
						$resAcao[1] => acadsc
						$resAcao[2] => funcod
						$resAcao[3] => sfucod
						$resAcao[4] => valor
					*/
				  	$resAcao = $db->carrega_registro( $RS2, $j );
					?>
					
					<? if ($taccod != $resAcao[5] || $j == 0){ ?>
					<tr>
						<td colspan="8" align="center" bgcolor="#e7e7e7" class="SemBorda">
							<b><i><?= $resAcao['tacdsc'] ?></i></b>
						</td>
					</tr>
			    	<?$taccod = $resAcao[5];
				}?>
						<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
							<!-- PROGRAMA E ACAO CODIGO -->
							<td align="left" class="SemBorda">
								<?= $resPrograma[0] ?>.<b><?= $resAcao[0] ?></b>
							</td>
							<!-- ACAO DESCRICAO -->
							<td align="left" class="SemBordaCimaBaixo" style="padding: 0 0 0 20px;">
								<img src="/imagens/seta_filho.gif" align="top"/>
								<b><i><?= $resAcao[1] ?></i></b>
							</td>
							<!-- ACAO FUNCIONAL SUBFUNCIONAL -->
							<td width="6%" align="center" class="SemBordaCimaBaixoDireita">
								<b><i><?= $resAcao[2] ?>&nbsp;&nbsp;<?= $resAcao[3] ?></i></b>
							</td>
							<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
							<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
							<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
							<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
							<!-- ACAO VALOR -->
							<td align="right" class="SemBordaCimaBaixoDireita">
								<b><i>&nbsp;<?= formata_valor( $resAcao[4], 0 ) ?></i></b>
							</td>
						</tr>
					<?

					// captura os localizadores
					$Query = "select
							 		a.loccod, a.sacdsc, 
							 		coalesce( SUM( ac.dpavalor + ac.dpavalorexpansao) , 0 ) as valor, a.acaid, a.prgid 
							 	from elabrev.despesaacao ac 
							 		inner join elabrev.ppaacao_orcamento a using ( acaid ) 
							 	where " . $filtro_fixo . " and 
							 		a.prgid = " . $resPrograma[3] . " and 
							 		a.acacod = '" . $resAcao[0] . "' 
							 	group by a.prgid, a.acaid, a.loccod, a.sacdsc 
							 	order by a.loccod, a.sacdsc ";
					$RS3 = $db->record_set( $Query );
				 
					$nlinhas3 = (integer) $db->conta_linhas( $RS3 );
					// FIM captura os localizadores
					// percorre os localizadores
				  	for ( $k = 0; $k <= $nlinhas3; $k++ )
					{
						/*
							$resLocalizador[0] => loccod
							$resLocalizador[1] => sacdsc
							$resLocalizador[2] => valor
							$resLocalizador[3] => acaid
						*/
					  	$resLocalizador = $db->carrega_registro( $RS3, $k );
					  	?>
							<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
								<!-- PROGRAMA, ACAO E LOCALIZADOR CODIGO -->
								<td align="left" class="SemBorda">
									<a href="elabrev.php?modulo=principal/propostaorcamentaria/despesadetalhamento&acao=A&acaid=<?= $resLocalizador[3] ?>">
										<?= $resPrograma[0] ?>.<?= $resAcao[0] ?>.<b><?= $resLocalizador[0] ?></b>
									</a>
								</td>
								<!-- LOCALIZADOR DESCRICAO -->
								<td align="left" class="SemBordaCimaBaixo" style="padding: 0 0 0 40px;">
									<img src="/imagens/seta_filho.gif" align="top"/>
									<?= $resLocalizador[1] ?>&nbsp;
								</td>
								<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
								<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
								<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
								<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
								<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
								<td align="right" class="SemBordaCimaBaixoDireita">
									<b><?= formata_valor( $resLocalizador[2], 0 ) ?></b>
								</td>
							</tr>
						<?
						// captura produtos e unidades de medida
						$Query = "select 
										a.acadscunmsof as unmdsc,
										a.acadscprosof as prodsc,
										e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod, 
								 		coalesce( SUM( ac.dpavalor + ac.dpavalorexpansao) , 0 ) as valor, a.acaid 
								 	from elabrev.despesaacao ac 
								 		inner join elabrev.ppaacao_orcamento a using ( acaid ) 
								 		inner join esfera e using ( esfcod ) 
								 		inner join naturezadespesa n using ( ndpid ) 
								 	where " . $filtro_fixo . " and 
								 		a.prgid = " . $resPrograma[3] . " and 
								 		a.acacod = '" . $resAcao[0] . "' and 
								 		a.loccod = '" . $resLocalizador[0] . "' 
								 	group by 
								 		a.acadscunmsof, a.acadscprosof, e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod, a.acaid 
								 	order by 
								 		a.acadscunmsof, a.acadscprosof, e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod";
						$RS4 = $db->record_set( $Query );
						
				    	$nlinhas4 = (integer) $db->conta_linhas( $RS4 );
				    	/*
							$resProdutoUnidade[0] => unmdsc
							$resProdutoUnidade[1] => prodsc
							$resProdutoUnidade[2] => esfdsc
							$resProdutoUnidade[3] => ndpcod
							$resProdutoUnidade[4] => ndpdsc
							$resProdutoUnidade[5] => acaqtdefisico
							$resProdutoUnidade[6] => iducod
							$resProdutoUnidade[7] => foncod
							$resProdutoUnidade[8] => valor
							$resProdutoUnidade[9] => acaid
						*/
				    	$resProdutoUnidade = $db->carrega_registro( $RS4, 0 );
						// FIM captura produtos e unidades de medida
						//Mostra o produto
						
						//coloca id na tr
						$idtr = $idtr + 1; 
						?>
						<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
									<td align="left" class="SemBorda">&nbsp;&nbsp;</td>
									<td align="left" class="SemBordaCimaBaixo" style="padding: 0 0 0 60px;">
										<table width="100%" border="0" cellpadding="0" cellspacing="0">
											<tr>
												<!-- PRODUTO E UNIDADE DE MEDIDA DESCRICAO -->
												<td width="60%" align="left">
													<img src="/imagens/seta_filho.gif" align="top" />
													<?if($nlinhas4 >=0){?>
														<script>
															elementos[<?=$idtr?>] = <?=$nlinhas4?>;
														</script>
														<img id="mais_<?=$idtr?>" align="bottom" border="0" style="cursor:pointer" onclick="exibeTr('<?=$idtr?>','<?=$nlinhas4?>');" src="/imagens/mais.gif" /> 
														<img id="menos_<?=$idtr?>" align="bottom" border="0" style="cursor:pointer;display:none" onclick="escondeTr('<?=$idtr?>','<?=$nlinhas4?>');" src="/imagens/menos.gif" />
													<?}?>
													<?= $resProdutoUnidade[1] ?> (<?= $resProdutoUnidade[0] ?>)&nbsp;
												</td>
												<!-- ACAO QUANTIDADE FISICO -->
												<td width="40%" align="right">
													Qtd. F�sico: <?= $resProdutoUnidade[5] ?>&nbsp;
												</td>
											</tr>
										</table>
									</td>
									<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
									<!-- ESFERA -->
									<td align="center" class="SemBordaCimaBaixoDireita">&nbsp;
									</td>
									<!-- NATUREZA -->
									<td align="center" class="SemBordaCimaBaixoDireita">&nbsp;
									</td>
									<!-- USO CODIGO -->
									<td align="center" class="SemBordaCimaBaixoDireita">&nbsp;
									</td>
									<!-- FONTE CODIGO -->
									<td align="center" class="SemBordaCimaBaixoDireita">&nbsp;
									</td>
									<!-- LOCALIZADOR VALOR -->
									<td align="right" class="SemBordaCimaBaixoDireita">
										&nbsp;<?//= formata_valor( $resLocalizador[2], 0 ) ?>
									</td>
						</tr>
						
						<?
						// percorre os produtos/unidadesmedida
						for ( $l = 0; $l <= $nlinhas4; $l++ )
						{
							/*
								$resProdutoUnidade[0] => unmdsc
								$resProdutoUnidade[1] => prodsc
								$resProdutoUnidade[2] => esfdsc
								$resProdutoUnidade[3] => ndpcod
								$resProdutoUnidade[4] => ndpdsc
								$resProdutoUnidade[5] => acaqtdefisico
								$resProdutoUnidade[6] => iducod
								$resProdutoUnidade[7] => foncod
								$resProdutoUnidade[8] => valor
								$resProdutoUnidade[9] => acaid
							*/
						  	$resProdutoUnidade = $db->carrega_registro( $RS4, $l );
							?>
								<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';" style="display:none" id="tr_<?=$idtr?>_<?=$l?>">
									<td align="left" class="SemBorda">&nbsp;&nbsp;</td>
									<td align="left" class="SemBordaCimaBaixo" style="padding: 0 0 0 80px;">
										<table width="100%" border="0" cellpadding="0" cellspacing="0">
											<tr>
												<!-- PRODUTO E UNIDADE DE MEDIDA DESCRICAO -->
												<td width="60%" align="left">
													<img src="/imagens/seta_filho.gif" align="top" />
													<a href="elabrev.php?modulo=principal/propostaorcamentaria/despesadetalhamento&acao=A&acaid=<?= $resProdutoUnidade[9] ?>"><?=
														substr( $resProdutoUnidade[3], 0, 1 ) . '.' .
														substr( $resProdutoUnidade[3], 1, 1 ) . '.' .
														substr( $resProdutoUnidade[3], 2, 2 ) . '.' .
														substr( $resProdutoUnidade[3], 4, 2 ) . '.' .
														substr( $resProdutoUnidade[3], 6, 2 )
													?> - <?= $resProdutoUnidade[4] ?>
													</a>
												</td>
											</tr>
										</table>
									</td>
									<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
									<!-- ESFERA -->
									<td align="center" class="SemBordaCimaBaixoDireita">
										&nbsp;<?= $resProdutoUnidade[2]{0} ?>&nbsp;
									</td>
									<!-- NATUREZA -->
									<td align="center" class="SemBordaCimaBaixoDireita">
										&nbsp;<?= $resProdutoUnidade[3] ?>
									</td>
									<!-- USO CODIGO -->
									<td align="center" class="SemBordaCimaBaixoDireita">
										&nbsp;<?= $resProdutoUnidade[6] ?>
									</td>
									<!-- FONTE CODIGO -->
									<td align="center" class="SemBordaCimaBaixoDireita">
										&nbsp;<?= $resProdutoUnidade[7] ?>
									</td>
									<!-- LOCALIZADOR VALOR -->
									<td align="right" class="SemBordaCimaBaixoDireita">
										&nbsp;<?= formata_valor( $resProdutoUnidade[8], 0 ) ?>
									</td>
								</tr>
							<? 
						}
						// FIM percorre os produtos/unidadesmedida
					}
					// FIM percorre os localizadores
				}		    
				// FIM percorre as a��es
			}
			// FIM percorre os programas
			?>
			<tr>
				<td colspan="7" bgcolor="#e7e7e7" align="right" class="SemBorda"><b>Total Geral (Despesas Programadas):</b></td>
				<td align="right" bgcolor="#e7e7e7" class="SemBorda"><b><?= formata_valor( $vlTotalPrograma, 0 ) ?></b></td>
			</tr>
			<?
			?>
		</table>
	<? } ?>
</form>
<?
	$Query1 = 	"select distinct ppa.prgcod as CodigoPrograma, 
						ppa.prgdsc as Programa, 	
						coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as ValorPrograma, 		
						acao.tacdsc as TipoA��o, 		
						acao.acacod as CodigoA��o, 		
						acao.acadsc as A��o, 		
						acao.funcod || ' ' || acao.sfucod as Funcional, 		
						acao.valor as ValorA��o, 		
						loc.loccod || ' - ' || loc.sacdsc as Localizador, 		
						coalesce( a.acadscprosof , '-') || ' - Qtd. F�sico: ' || a.acaqtdefisico as Produto, 		
						subStr(prod.esfdsc,1,1) as Esfera, 		
						prod.iducod as IU, 		
						prod.foncod as Fonte, 		
						substr(prod.ndpcod,1,1)||'.'||substr(prod.ndpcod,2,1)||'.'||substr(prod.ndpcod,3,2)||'.'||substr(prod.ndpcod,5,2)||'.'||substr(prod.ndpcod,7,2) || ' - ' || prod.ndpdsc as Natureza, 
						prod.valor as ValorProduto ".
				"	from elabrev.despesaacao ac ".
				"	left join elabrev.ppaacao_orcamento a using ( acaid ) 	".
				"	left join elabrev.ppaprograma_orcamento ppa using ( prgid ) ".
				"	left join ".
				"	(select ".	
				"		a.prgid, a.acacod, a.acadsc, a.funcod, a.sfucod, ".
				"		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor, t.taccod, t.tacdsc  ".
				"	from elabrev.despesaacao ac ".
				"	left join elabrev.ppaacao_orcamento a on a.acaid = ac.acaid ".
				"	left join elabrev.ppafuncao f on f.funcod = a.funcod ".
				"	left join elabrev.ppasubfuncao s on s.sfucod = a.sfucod ".		
				"	left join tipoacao t on t.taccod = a.taccod ".
				"	where  a.prgano = '". $_SESSION['exercicio'] ."' and  a.acastatus = 'A' and  a.unicod = '". $unidade ."' and  a.acasnrap = false ".
				"	group by 		a.prgid, a.acaid, a.acacod, a.acadsc, a.funcod, a.sfucod, t.taccod, t.tacdsc ".
				"	order by 		t.tacdsc, a.prgid, a.acaid, a.acacod, a.acadsc, a.funcod, a.sfucod) acao ON ".
				"		acao.prgid = ppa.prgid and acao.acacod = a.acacod".
				"	left join ".
				"	(select ".
				"		a.prgid, a.acacod, a.loccod, a.sacdsc, coalesce( SUM( ac.dpavalor + ac.dpavalorexpansao) , 0 ) as valor, a.acaid ".
				"	from elabrev.despesaacao ac ".
				"	left join elabrev.ppaacao_orcamento a using ( acaid ) ".
				"	where  a.prgano = '". $_SESSION['exercicio'] ."' and  a.acastatus = 'A' and  a.unicod = '". $unidade ."' and  a.acasnrap = false ".
				"	group by a.prgid, a.acacod, a.loccod, a.sacdsc, a.acaid 	order by a.loccod, a.sacdsc) as loc ON ".
				"		loc.prgid = ppa.prgid and loc.acacod = acao.acacod ".
				"	left join ".
				"	(select ".
				"		a.prgid, a.acacod, a.loccod, u.unmdsc, p.prodsc, e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod, ".
				"		coalesce( SUM( ac.dpavalor + ac.dpavalorexpansao) , 0 ) as valor, a.acaid ".
				"	from elabrev.despesaacao ac ".
				"	left join elabrev.ppaacao_orcamento a using ( acaid ) ".
				"	left join unidademedida u using ( unmcod ) ".
				"	left join produto p using ( procod ) ".
				"	left join esfera e using ( esfcod ) ".
				"	left join naturezadespesa n using ( ndpid ) ".
				"	where  a.prgano = '". $_SESSION['exercicio'] ."' and  a.acastatus = 'A' and  a.unicod = '". $unidade ."' and  a.acasnrap = false  ".
				"	group by a.prgid, a.acacod, a.loccod, u.unmdsc, p.prodsc, e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod, a.acaid ".
				"	order by u.unmdsc, p.prodsc, e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod ) as prod ON ".
				"		prod.prgid = ppa.prgid and prod.acacod = loc.acacod and prod.loccod = loc.loccod ".
				"	where  ".
				"		a.prgano = '". $_SESSION['exercicio'] ."' and  ".
				"		a.acastatus = 'A' and  ".
				"		a.unicod = '". $unidade ."' and  ".
				"		a.acasnrap = false and	".
			    "       ac.ppoid = " . $ppoid .
				"	group by
						CodigoPrograma,	Programa, TipoA��o, CodigoA��o, A��o, Funcional, ValorA��o, Localizador, Produto, Esfera, IU, Fonte, Natureza, ValorProduto	".
				"	order by 
						ppa.prgcod, ppa.prgdsc, acao.acacod, acao.acadsc, prod.valor ";

	/*group by ppa.prgid, ppa.prgcod, ppa.prgdsc, acao.tacdsc, acao.acacod, acao.acadsc, 
	acao.funcod || ' ' || acao.sfucod, acao.valor, loc.loccod, loc.sacdsc, prod.prodsc, prod.esfdsc, 	
	prod.iducod, prod.foncod, prod.ndpcod, prod.valor, a.acadscprosof, a.acaqtdefisico, prod.ndpdsc */
	
	/*order by 
	ppa.prgid, ppa.prgcod, ppa.prgdsc, acao.acacod, acao.acadsc, 
	loc.loccod, loc.sacdsc, prod.prodsc, prod.ndpcod, prod.valor */
	
	$_SESSION['Query'] = Array($Query1);


    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($nlinhas1 >=0)
      print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(4)">';
      print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(6)">';      
    print '</td></tr></table>';

    
?>     
<script>
function imprime_rel(cod)
{
    <?
       $browser = get_browser();
       $saiu = false;
       if (is_array($browser)){
       foreach ($browser as $name => $value) {
           if ($value == 'IE') {
           $saiu = true;
          }
       }}
	   
       if ($saiu){?>
          if (cod == 1) {
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
          };
          if (cod== 2) {
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenaca&acao=R";
          }
          if (cod>2) {
          	if (cod ==3) document.formulario.relat.value='planilha';
          	if (cod ==4) document.formulario.relat.value='planilha_lista'; 
          	if (cod ==5) document.formulario.relat.value='planilha';
          	if (cod ==6) document.formulario.relat.value='planilha_lista';             	
          	document.formulario.planilha.value=cod;         	
                document.formulario.submit();
            }
       <?} else { ?>
          if (cod == 1) {
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
        }
          if (cod== 2) {
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenaca&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
            }
          if (cod>2) {
          	if (cod ==3) document.formulario.relat.value='planilha';
          	if (cod ==4) document.formulario.relat.value='planilha_lista'; 
          	if (cod ==5) document.formulario.relat.value='planilha';
          	if (cod ==6) document.formulario.relat.value='planilha_lista';          	
          	document.formulario.planilha.value=cod;         	
                document.formulario.submit();
            }  
           

      <? } ?>
}

function submeter_gera_tipo()
{
	window.location.href = 'elabrev.php?modulo=relatorio/orcamento/relatorioQuadroCreditos&acao=A&unicod=<?= $unicod ?>&gera_tipo=1';
}

function exibeTr(idx, total){

	for(i=0; i<=total; i++){
		document.getElementById('tr_'+idx+'_'+i).style.display = "";
	}
	
	document.getElementById('menos_'+idx).style.display = "";
	document.getElementById('mais_'+idx).style.display = "none";
}

	
function escondeTr(idx, total){

	for(i=0; i<=total; i++){
		document.getElementById('tr_'+idx+'_'+i).style.display = "none";
	}

	document.getElementById('menos_'+idx).style.display = "none";
	document.getElementById('mais_'+idx).style.display = "";
}	

var aberto = false;
function abrirFecharTodos()
{
	for(var i=1; i<elementos.length; i++)
	{
		if( !aberto )
		{
			exibeTr(i, elementos[i]);
		}
		else
		{
			escondeTr(i, elementos[i]);
		}
	}
	if(aberto)
	{
		aberto = false;
		document.getElementById("abre_fecha").innerHTML = '<img src="/imagens/mais.gif" border="0" /> Abrir todos';
	}
	else
	{
		aberto = true;
		document.getElementById("abre_fecha").innerHTML = '<img src="/imagens/menos.gif" border="0" /> Fechar todos';
	}
}

</script>
