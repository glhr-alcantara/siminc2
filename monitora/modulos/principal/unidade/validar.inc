<?
 /*
   sistema simec
   setor respons�vel: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   m�dulo:monitoraprg.inc
   finalidade: permitir o monitoramento de programa
*/

 //if (!$db->testa_superuser()) {print '<script>alert("P�gina em Constru��o!");history.back();</script>';exit();}
 //Aumenta a sess�o da p�gina
ini_set('session.gc_maxlifetime', 4800);


if( is_array( $_POST['acaidvalida'] ) && @count( $_POST['acaidvalida'] ) > 0 && $_POST['corcod_par'] )
{
	$concluido = 0; // -1 erro, 0 nao concluido, 1 sucesso
	//$sqlInsAvp = "Insert into monitora.avaliacaoparecer (refcod,usucpf,corcod,acaid,tpaid,avptexto,avpliberada,avpstatus,avporigem,avpdata) VALUES('%s', '%s', '%s', '%s', 2, '%s', '1', 'A', 4, now())";
	//$sqlUpdAvp = "Update monitora.avaliacaoparecer Set refcod='%s', usucpf='%s', corcod='%s', acaid='%s', tpaid=2, avptexto='%s', avpliberada='1', avpstatus='A', avporigem=4, avpdata=now() Where avpid='%s'";
	$sqlInsAvp = "Insert into monitora.avaliacaoparecer ( refcod, usucpf, avpsituacao, acaid, tpaid, avptexto, avpliberada, avpstatus, avporigem, avpdata, corcod ) VALUES( '%s', '%s', '%s', '%s', 2, '%s', '1', 'A', 4, now(), %s)";
	$sqlUpdAvp = "Update monitora.avaliacaoparecer Set refcod = '%s', usucpf = '%s', avpsituacao = '%s', acaid = '%s', tpaid = 2, avptexto = '%s', avpliberada = '1', avpstatus = 'A', avporigem = 4, avpdata = now(), corcod = %s Where avpid = '%s'";
	
	// o campo corcod_par na verdade significa situa��o de discord�ncia ou concord�ncia
	$avpsituacao = $_POST["corcod_par"];
	
	foreach ( $_POST['acaidvalida'] as $acaidvalida )
	{
		$acaidvalida = (integer) $acaidvalida;
		if( $acaidvalida )
		{
			// captura a cor
			// caso a cor seja nula indica que a cor n�o mudou
			// se for discordancia a cor � a do campo $_REQUEST['cor_discordancia']
			$corcod = 'null';
			if ( $avpsituacao == '3' && isset( $_POST['cor_discordancia'][$acaidvalida] ) )
			{
				$corcod = $_POST['cor_discordancia'][$acaidvalida];
			}
			
			$avpid = $db->pegaUm( "select avpid from monitora.avaliacaoparecer where tpaid = 2 and refcod = " . $_POST['refcod'] . " and acaid = " . $acaidvalida );
			if ( !$avpid )
			{
				//$dadosConsulta = array( $_POST["refcod"], $_SESSION["usucpf"], $_POST["corcod_par"], $acaidvalida, $_POST["avptexto"][$acaidvalida] );
				$dadosConsulta = array( $_POST["refcod"], $_SESSION["usucpf"], $avpsituacao, $acaidvalida, $_POST["avptexto"][$acaidvalida], $corcod );
				$sql = vsprintf( $sqlInsAvp, $dadosConsulta );
			}
			else
			{
				//$dadosConsulta = array( $_POST["refcod"], $_SESSION["usucpf"], $_POST["corcod_par"], $acaidvalida, $_POST["avptexto"][$acaidvalida], $avpid );
				$dadosConsulta = array( $_POST["refcod"], $_SESSION["usucpf"], $avpsituacao, $acaidvalida, $_POST["avptexto"][$acaidvalida], $corcod, $avpid );
				$sql = vsprintf( $sqlUpdAvp, $dadosConsulta );
			}
			$db->executar( $sql );
		}
	}
	$db->commit();
	//$db->sucesso($modulo);
	?>
		<script>
			alert( 'Opera��o Realizada com Sucesso!' );
			location.href= '/monitora/monitora.php?modulo=principal/unidade/unidadesobminharesp&acao=C';
			//history.back();
		</script>
	<?php
}

$modulo = $_REQUEST['modulo'];
/*
if ( $_REQUEST['prgid'] ) $_SESSION['prgid'] = $_REQUEST['prgid'];
else $_REQUEST['prgid'] = $_SESSION['prgid'];

if ( $_REQUEST['acaid'] ) $_SESSION['acaid'] = $_REQUEST['acaid'];
else $_REQUEST['acaid'] = $_SESSION['acaid'];
*/

/*
if ( $_POST['prgcod'] ) $sql= "select * from programa where prgano = '" . $_SESSION['exercicio'] . "' and prgcod='" . $_POST['prgcod'] . "'  order by prgcod";
elseif ( $_REQUEST['prgid'] ) $sql= "select * from programa where prgano = '" . $_SESSION['exercicio'] . "' and prgid=" . $_REQUEST['prgid'] . "  order by prgcod";

$RS = $db->record_set( $sql );
$nlinhas = $db->conta_linhas( $RS );

// as linhas abaixo n�o permitem navegar para al�m dos limites
//controla_navegacao($nlinhas);
if ( $nlinhas < 0 )
{
	?>
		<script>
			alert('Programa Inexistente!');
			history.back();
		</script>
	<?
	$db->close();
	exit();
}

// a linha abaixo transforma em vari�veis todos os campos do array
$res = $db->carrega_registro( $RS,0 );
if ( is_array( $res ) ) foreach ( $res as $k => $v ) ${$k} = $v;
$_SESSION['prgid'] = $prgid;
*/
// encontrar o orgao
$sql= "select orgdsc from orgao where orgcod='" . $orgcod . "'";
$res = $db->recuperar( $sql );
if ( is_array( $res ) ) foreach ( $res as $k => $v ) ${$k} = $v;

// encontrar a unidade
$sql= "select unidsc from unidade where unicod='" . $unicod . "'";
$res = $db->recuperar( $sql );
if ( is_array( $res ) ) foreach ( $res as $k => $v ) ${$k} = $v;

if ( $_REQUEST['refcod'] ) $refcod = $_REQUEST['refcod'];
if ( $_REQUEST['unicod'] ) $unicod = $_REQUEST['unicod'];
if ( $_REQUEST['corcod'] or $_REQUEST['corcod'] == 0 ) $corcod = $_REQUEST['corcod'];

if ( $refcod )
{
	$sqlVerificaPeriodoLiberado = "
		select count(*)
		from referencia
		where
			refdata_limite_parecer_aca is not null and
			refsnmonitoramento = 't' and
			refano_ref = '" . $_SESSION['exercicio'] . "' and
			refcod = '" . $refcod . "'
	";
	$periodoLiberado = $db->pegaUm( $sqlVerificaPeriodoLiberado );
	if ( !$periodoLiberado )
	{
		?>
		<script type="text/javascript">
			alert( 'Trimestre n�o liberado para valida��o.' );
		</script>
		<script type="text/javascript">
			history.back();
		</script>
		<?
		exit();
	}
}

include APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$ajustaTela = true; //Rola a tela para caber em 800x600
$db->cria_aba( $abacod_tela, $url, '' );
$titulo_modulo='Avalia��o da Unidade';
monta_titulo( $titulo_modulo, 'Valida��o do Gestor <span id="referencia"> -- </span>' );

//faz o controle do registro atual para a pagina��o
$numero = !$_REQUEST['numero'] ? 1 : intval( $_REQUEST['numero'] );

/*
//Teste se � Gerente (2) ou Gerente Executivo (3)
$gerente = false;
if ( $db->testa_responsabilidade( $prgid, 22, 'prgid' ) ) $gerente = true;
if ( $db->testa_responsabilidade( $prgid, 2, 'prgid' ) ) $gerente = true;
if ( $db->testa_responsabilidade( $prgid, 3, 'prgid' ) ) $gerente = true;
if ( $db->testa_superuser() ) $gerente = true;
*/

$gerente = true; // essa tela � acess�vel somente por gestor
/*
if ( $refcod )
{
	// verifica se data limite do parecer n�o expirou
	// caso sim o status de gerente � retirado do usu�rio
	$refdata_limite_parecer_aca = $db->pegaUm( "select refdata_limite_parecer_aca from referencia where refcod=" . $refcod );
	if ( $gerente and $refdata_limite_parecer_aca < date('Y-m-d') )
	{
		$gerente = false;
		// n�o est� autorizado na data
		// ent�o testa se tem alguma autoriza��o especial
		if ( $refcod )
		{
			$sql= "select ae.aelid from autorizacaoespecial ae where ae.prgid = " . $_SESSION['prgid'] . " and ae.aelstatus ='A' and ae.aeldata_inicio <= '" . date( 'Y-m-d' ) . "' and ae.aeldata_fim >= '" . date( 'Y-m-d' ) . "' and ae.togcod in (1,2) and ae.refcod = " . $refcod;
			$registro = $db->recuperar($sql);
			if ( is_array( $registro ) ) $gerente = true;
		}
	}
}
*/
// a variavel HABIL indica se esta dentro da data

if ( $gerente )
{
	$largura='width:99.9%';
	?>
		<center>
		<div id="valida" style="overflow:auto;width:95%;height:220px;border-bottom:2px solid #cccccc;">
	<?}?>
<form method="POST"  name="formulario" style="margin:0px;">
	<input type=hidden name="corcod_par" value="">
	<input type=hidden name="numero" value="1">
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;<?= $largura ?>;border-bottom:none;">
		<tr>
			<td align='right' class="subtitulodireita">Unidade Or�ament�ria:</td>
			<td>
				<?php
					
					$join = $db->usuarioJoinUnidadesPermitidas( "monitora" );
					if ( $join != "" )
					{
						$join .= " and unijoin.unicod = a.unicod ";
					}
					
					$sql2 = "
						select
							a.unicod as codigo,
							a.unicod || ' - ' || u.unidsc as descricao
						from acao a
							inner join unidade u on a.unicod = u.unicod
							" . $join . "
						group by a.unicod, u.unidsc
						order by u.unidsc
					";
					$db->monta_combo( "unicod", $sql2, 'S', "", $funcao_combo, '' );
				?>
			</td>
		</tr>
		<?php if ( $unicod ) : ?>
			<?php
				$sqlOrgao = "
					select o.orgdsc
					from public.orgao o
						inner join public.unidade u on u.orgcod = o.orgcod
					where
						u.unicod = '" . $unicod . "'
				";
				$orgdsc = $db->pegaUm( $sqlOrgao );
			?>
						
		<?php $db->mostra_resp( $unicod, 'unicod'); ?>
			
		<?php endif; ?>
		
		<?php if ( $unidsc ) : ?>
			<tr>
				<td align='right' class="subtitulodireita">Unidade Respons�vel:</td>
				<td><?= $unidsc ?></td>
			</tr>
		<? endif;
		
		if ( $refcod ) $funcao_combo = "document.formulario.submit";
		?>
		<tr>
			<td align='right' class="SubTituloDireita">Per�odo de Refer�ncia :</td>
			<td>
				<?php
					$sql2 = "select refcod as CODIGO,refdsc as DESCRICAO from referencia where refdata_limite_parecer_aca is not null and refsnmonitoramento='t' and refano_ref='" . $_SESSION['exercicio'] . "' order by refano_ref,refmes_ref";
					$db->monta_combo( "refcod", $sql2, 'S', "Escolha o per�odo de refer�ncia", $funcao_combo, '' );
				?>
			</td>
		</tr>		
		<tr>
			<td align='right' class="subtitulodireita">Situa��o:</td>
			<td>
				<?php
					$sql2 = "Select corcod as codigo,  corsignificadopar as descricao from cor order by corcod";
					$db->monta_combo( "corcod", $sql2, 'S', "Todas as Situa��es", $funcao_combo, '' );
				?>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;<?= $largura ?>;border-top:none;">
		<?php if ( $refcod && $unicod ) : ?>
			<?php
			//Par�metros de Pagina��o
			//Registros por p�gina
			$perpage = 50;
			//Qtde de Paginas que ser�o mostradas na barrinha
			$pages = 50;
			//Montando a cl�usula WHERE
			$wh = "";
			if ( $unicod ) $wh .= " and aca.unicod = '" . $unicod . "'";
			//if ( $corcod ) $wh .= " and avp.corcod = " . $corcod;
			//if ( $corcod == '0' ) $wh .= " and avp.corcod is null";
			if ( $corcod ) $wh .= " and avp.avpsituacao = " . $corcod;
			if ( $corcod == '0' ) $wh .= " and avp.avpsituacao is null";
		
			//Query que traz as a��es baseado no filtro escolhido
			$sql = "
				select
					distinct aca.acaid,
					aca.acacod,
					aca.prgcod,
					aca.unicod,
					aca.loccod,
					aca.acadsc,
					aca.procod,
					avp.avptexto,
					case
						when usu.usunome is null then 'N�o Atribuido'
						else usu.usunome || ' ('|| org.orgdsc ||') - Tel: ('||usufoneddd||') '||usufonenum end as usunome,
					case
						when usu.usucpf is null then ''
						else usu.usucpf end as cpfcoord,
					uni.unidsc,
					sacdsc,
					unm.unmdsc,
					pro.prodsc,
					case
						when cor.corcod is null then 0
						else cor.corcod end as corcod,
					case
						when cor.corsignificadopar is null then 'N�o Validado'
						else cor.corsignificadopar end as corsignificadopar,
				tac.tacdsc
				from acao aca
					inner join unidade uni on aca.unicod = uni.unicod
					inner join tipoacao tac on aca.taccod = tac.taccod
					left join monitora.usuarioresponsabilidade usr on
						aca.acaid = usr.acaid and
						usr.pflcod = 1 and
						rpustatus = 'A' and
						usr.prsano = '" . $_SESSION['exercicio'] . "'
					left join usuario usu on usr.usucpf = usu.usucpf
					left join orgao org on org.orgcod=usu.orgcod
					left join unidademedida unm on aca.unmcod = unm.unmcod
					left join produto pro on aca.procod = pro.procod
					left join avaliacaoparecer avp on aca.acaid=avp.acaid and tpaid=2 and refcod=" . $refcod . " 
					left join cor on avp.avpsituacao = cor.corcod
				where
					aca.acastatus = 'A' and
					aca.acasnrap = 'f' and
					aca.unicod = '" . $unicod . "' " . $wh . " and
					aca.prgano = '" . $_SESSION['exercicio'] . "'
				order by 3,2,4,5
			";
			//dump( $sql, true );
			$RS = $db->carregar( $sql );
			$arrAcas = array();
			if ( $RS != '' )
			{
				foreach ( $RS as $linha )
				{
					$arrAcas[] = $linha["acaid"];
				}
				$acaids = implode( ",", $arrAcas );
				$sql2 = "SELECT * FROM ex_fisico_financeiro WHERE acaid IN (" . $acaids . ") AND grefcod = " . $refcod;
				$RS3 = $db->carregaAgrupado( $sql2, "acaid" );
			}
			?>
			<?php if ( is_array( $RS ) ) : ?>
				</table>
				<?php
				$cont = 0;
				$nlinhas = count( $RS );
				//Total de Registros 
				$total_reg = $nlinhas + 1;
				$reg_fim = ( $numero + $perpage ) > $nlinhas ? $nlinhas : $numero + $perpage - 1;
				$i_div_discordancia = 0;
				for ( $i = ( $numero - 1 ); $i < $reg_fim; $i++ ) : ?>
					<?php foreach( $RS[$i] as $k => $v ) ${$k} = $v; ?>
					<table id="tabelaacao<?=$cont?>" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:333333;<?=$largura?>;border-top: 2px solid #404040;">
						<tr bgcolor="#ffffcc">
							<td align='right' class="subtitulodireita" width="10%">A��o:</td>
							<td width="40%">
								<?php if ( $gerente ) : ?>
									<input type="checkbox" name="acaid" id="<?=$acaid?>"
										<?= $usunome == 'N�o Atribuido' ? ' value="" disabled="disabled" ' : ' value="' . $acaid . '" ' ?>
										onclick="retorna( <?= $cont ?> );" >
								<?php endif; ?>
								<font color="#0F55A9"><?= $prgcod . '.' . $acacod . '.' . $unicod . '.' . $loccod ?></font>
								-
								<font color="#006600"><?= $acadsc ?></font>
								<input type="hidden" name="acadsc" value="<?= $prgcod . '.' . $acacod . '.' . $unicod . '.' . $loccod . ' - ' . $acadsc ?>">
							</td>
							<td align='right' class="subtitulodireita" width="10%">
								<b>Situa��o:</b>
							</td>
							<td style="color:#000000" width="40%">
								<img src="../imagens/valida<?= $corcod ?>.gif" align="absmiddle" width="15" height="15">
								<b><?= $corsignificadopar ?></b>
								<br/>
								<div id="div_cor_discordancia_<?= $i_div_discordancia++ ?>" style="display:none;">
									<input type="radio" title="Est�vel" name="cor_discordancia[<?= $acaid ?>]" checked="" value="1" id="cor_discordancia1<?= $acaid ?>"/>
									<label for="cor_discordancia1<?= $acaid ?>">
										<img title="Est�vel" src="../imagens/av_verde.gif"/>
									</label>
									<input type="radio" title="Merece Aten��o" name="cor_discordancia[<?= $acaid ?>]" value="2" id="cor_discordancia2<?= $acaid ?>"/>
									<label for="cor_discordancia2<?= $acaid ?>">
										<img title="Merece Aten��o" src="../imagens/av_amarelo.gif"/>
									</label>
									<input type="radio" title="Cr�tico" name="cor_discordancia[<?= $acaid ?>]" value="3" id="cor_discordancia3<?= $acaid ?>"/>
									<label for="cor_discordancia3<?= $acaid ?>">
										<img title="Cr�tico" src="../imagens/av_vermelho.gif"/>
									</label>
									<b>Discord�ncia</b>
								</div>
							</td>
						</tr>
						<tr>
							<td align='right' class="subtitulodireita">Unidade:</td>
							<td><?= $unicod . ' - ' . $unidsc ?></td>
							<td align='right' class="subtitulodireita">Localizador:</td>
							<td><?= $loccod . ' - ' . $sacdsc ?></td>
						</tr>
						<tr>
							<td align='right' class="subtitulodireita">Tipo:</td>
							<td><?= $tacdsc ?></td>
							<td align='right' class="subtitulodireita">Coordenador:</td>
							<td>
								<?php
								if ( $cpfcoord ) print '<img src="../imagens/email.gif" title="Envia e-mail ao Gestor" border="0" onclick="envia_email(\'' . $cpfcoord . '\');"> ';
								print $usunome
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="subtitulodireita">Produto:</td>
							<td><?= $prodsc ?></td>
							<td align='right' class="subtitulodireita">Unid. Medida:</td>
							<td><?= $unmdsc ?></td>
						</tr>
						<tr id="texto<?=$cont?>" <?= $gerente and $corcod != 3 ? 'style="visibility:hidden;display:none;"' : '' ; ?>>
							<td colspan="7" id="td<?= $cont ?>" align="left" style="padding-left:5px">
								Motivo da discord�ncia sobre a A��o ( <?= $acacod . '.' . $unicod . '.' . $loccod ?> ):
								<br/><textarea disabled id="avptexto<?= $acaid ?>" name="avptexto[<?= $acaid ?>]" rows="3" class="normal" onmouseover="MouseOver(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="width:99%;"><?=$avptexto?></textarea>
							</td>
						</tr>
						<tr>
							<td colspan="4" style="padding:0px;">
						  		<?php
							  		//Mostra o executado f�sico/financeiro
									$sql2 = "select * from ex_fisico_financeiro where acaid = " . $acaid . " and grefcod=" . $refcod;
									$RS2 = $RS3[$acaid];
									// print 'rs2='.print_r($RS2); //exit();
									$nlinhas2 = count( $RS2 ) - 1;
								?>
								<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:333333;width:100%;">
									<tr bgcolor="#E3E3E3">
										<td>Execu��o Fisico-Financeira</td>
										<?php for ( $j = 0; $j <= $nlinhas2; $j++ ) : ?>
											<td align="right" style="color:#0F55A9;"><?= $RS2[$j]['refdsc'] ?></td>
										<?php endfor; ?>
										<td align="right"><b>Executado</b></td>
										<td align="right">Previsto na LOA</td>
										<td align="right">% Executado</td>
									</tr>
									<?php // verifica se pode ser acompanhado (Se tem produto pode!) nao pode o procod 0,3 ?>
									<?php if ( !in( $procod, array( '0', '3' ) ) ) : ?>
										<tr>
											<td align="right">F�sico<br><font color="#aaaaaa">(em <?=$unmdsc?>)</font></td>
											<?php $totalfisrealizado = 0; ?>
											<?php for ( $j = 0; $j <= $nlinhas2; $j++ ) : ?>
												<td title="<?=$RS2[$j]['corsignificado']?>" align="right" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
													<?= number_format( $RS2[$j]['exprealizado'], 0, ',', '.' ); ?>
													<br/>
													<?php if ( $RS2[$j]['avpid'] ) : ?>
														<a href="javascript:visualiza('<?= md5_encrypt( $RS2[$j]['avpid'], '' ) ?>')">
													<? endif; ?>
													<img border="0" width="25" height="10" src="../imagens/<?= $RS2[$j]['corimgav'] ?>">
													<? if ( $RS2[$j]['avpid'] ) print '</a>'; ?>
												</td>
												<?php 
														$vlrealizado = $db->pegaUm( "SELECT case when aca.acasnmetanaocumulativa='f' then sum(exe.exprealizado) else max(exe.exprealizado) end as realizado
																					 FROM monitora.acao aca 
																					left join unidademedida  u on u.unmcod=aca.unmcod 
																					left join produto pro on aca.procod=pro.procod 
																					left  join monitora.dadofisico dad on aca.acaid=dad.acaid 
																					left join monitora.execucaopto exe on aca.acaid=exe.acaid
																					left  join monitora.referencia r on exe.refcod=r.refcod and r.refano_ref='".$_SESSION['exercicio']."' 
																					WHERE aca.acaid=".$RS3[$acaid][0]['acaid']." and aca.acasnrap='f' 
																					group by aca.acasnmetanaocumulativa");

														$totalfisrealizado = $vlrealizado;

												?>

											<?php endfor; ?>
											<td align="right" valign="top"><?=number_format($totalfisrealizado, 0, ',', '.');?></td>
											<td align="right" valign="top"><?=number_format($RS2[0]['fisqtdeprevistoano'], 0, ',', '.');?></td>
											<td align="right" valign="top">
												<?php
												if ( $RS2[0]['fisqtdeprevistoano'] == 0 ) print 0;
												else print number_format( $totalfisrealizado * 100 / $RS2[0]['fisqtdeprevistoano'], 2, ',', '.' );
												?>
											</td>
										</tr>
						  			<?php endif; ?>
						  			<tr bgcolor="White">
						  				<td align="right">Financeiro<br><font color="#aaaaaa">(em R$)</font></td>
						  				<?php $totalfinrealizado = 0; ?>
						  				<?php for ( $j = 0; $j <= $nlinhas2; $j++ ) : ?>
							  				<td align="right"><?=number_format($RS2[$j]['exfrealizado'], 2, ',', '.');?></td>
							  				<?php $totalfinrealizado = $totalfinrealizado+$RS2[$j]['exfrealizado']; ?>
						  				<?php endfor; ?>
						  				<td align="right"><?=number_format($totalfinrealizado, 2, ',', '.');?></td>
						  				<td align="right"><?=number_format($RS2[0]['finvlratualano'], 2, ',', '.');?></td>
						  				<td align="right">
						  					<?php
						  					if ( $RS2[0]['finvlratualano'] == 0 ) print 0;
						  					else print @number_format($totalfinrealizado*100/$RS2[0]['finvlratualano'], 2, ',', '.');
						  					?>
						  				</td>
						  			</tr>
						  			<tr bgcolor="#E3E3E3">
						  				<td colspan="7" align="right" style="color:#008080">* Clique no dado f�sico para ver o detalhamento da execu��o.</td>
						  			</tr>
								</table>
							</td>
						</tr>
					</table>
				<?php $cont++; ?>
				<?php endfor; ?>
				<?php if ( $gerente ) : ?>
					</div>
					<table id="tabelaacao<?= $cont ?>" border="0" bgcolor="#F5F5F5" width="95%" cellpadding="2" align="center" style="border-right: 1px solid #cccccc;border-left: 1px solid #cccccc;border-bottom: 2px solid #cccccc;width:95%;">
						<tr>
							<td colspan="4">
								<select multiple size="6" name="acaidvalida[]" id="acaidvalida" style="width:100%;" class="CampoEstilo" onchange="moveto(this);">
									<option value="">-- Selecione as A��es na Listagem Acima...</option>
								</select>
							</td>
						</tr>
						<tr>
							<?php if ( $i - $numero <= $perpage ) : ?>
								<td>
									<input type="button" name="Marcar Todas" value="Marcar Todas" onclick="marcatodas();">
								</td>
							<?php endif; ?>
							<td>
								<font color="#000099">
								<?= $nlinhas ?> A��es no Total</font><br/>
								<font color="#006600"><?= $i - $numero + 1 ?> A��es Listadas</font><br/>
								<font color="#990000"><span id="acamarcadas">0</span> A��es Marcadas</font>
							</td>
							<td align="right">
								<input type="button" name="Validar com Concord�ncia" value="Validar com Concord�ncia" onclick="selectAllOptions(campoSelect);envia(1);"> <input type="button" name="Validar com Disccord�ncia" value="Validar com Discord�ncia" onclick="selectAllOptions(campoSelect);envia(3);"> <input type="button" name="Enviar para Pend�ncia" value="Enviar para Pend�ncia" onclick="selectAllOptions(campoSelect);envia(2);">
							</td>
						</tr>
						<tr>
							<td colspan="3" align="right" bgcolor="#dcdcdc" style="padding:1px;">
								<?php include APPRAIZ . "includes/paginacao.inc"; ?>
							</td>
						</tr>
					</table>
					<table id="tabelaincluir" border="0" bgcolor="#F5F5F5" width="95%" cellpadding="2" align="center" style="border-right: 1px solid #cccccc;border-left: 1px solid #cccccc;border-bottom: 2px solid #cccccc;visibility:hidden;display:none;">
						<tr>
							<td align="center">
								<input type="button" name="Incluir" value="Validar A��es com Discord�ncia" onclick="discordar();">
							</td>
						</tr>
					</table>
					<input type="hidden" name="acaid" value="">
					<input type="hidden" name="acadsc" value="">
				</form>
				</center>
				<?php else : ?>
					<center>
						<div style='WIDTH:95%; border-bottom: 1px solid #cccccc;'>
							<?php include APPRAIZ."includes/paginacao.inc" ?>
						</div>
					</center>
				<?php endif; ?>
			<?php else : ?>
			<tr>
				<td class="SubTituloDireita" width="20%"></td>
				<td style="color:#990000">N�o foram encontrados registros para sua pesquisa.</td>
			</tr>
		<?php endif; ?>
	<?php endif; ?>
	<?php if ( !$refcod or !is_array( $RS ) ) : ?>
			    	<tr>
						<td class="SubTituloDireita"></td>
						<td bgcolor="#dcdcdc"><input type="button" name="Listar" value="Listar A��es" onclick="escolhe_referencia()"></td>
					</tr>
				</table>
			</form>
		</div>
	</center>
	<?php endif; ?>
	
	
	
<script type="text/javascript">

function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }


	var campoSelect = document.formulario.refcod.options;
	var obj = document.getElementById("referencia");
	obj.firstChild.nodeValue='('+campoSelect[campoSelect.selectedIndex].text+')';
	var campoSelect = document.getElementById("acaidvalida");
	var campodsc = document.formulario.acadsc;
	var campoid = document.formulario.acaid;
	var campototal = document.getElementById("acamarcadas");
	var qtdmarcadas = 0;

	function retorna(objeto)
	{
	
		tamanho = campoSelect.options.length;
		if (campoSelect.options[0].value=='') {tamanho--;}
		if (document.formulario.acaid[objeto].checked == true){
			campoSelect.options[tamanho] = new Option(campodsc[objeto].value, campoid[objeto].value, false, false);
			sortSelect(campoSelect);
			qtdmarcadas ++;
		}
		else {
			for(var i=0; i<=campoSelect.length-1; i++){
				if (campoid[objeto].value == campoSelect.options[i].value)
					{campoSelect.options[i] = null;qtdmarcadas--;}
				}
				if (!campoSelect.options[0]){campoSelect.options[0] = new Option('-- Selecione as A��es na Listagem Acima...', '', false, false); qtdmarcadas = 0;}
				sortSelect(campoSelect);
		}
		campototal.firstChild.nodeValue=qtdmarcadas;
	}
	
	function moveto(obj) {
			document.getElementById(obj.value).focus();
	}
	
	function marcatodas(){
		for(var i=0; i<=document.formulario.acaid.length-1; i++){
			if (
			 document.formulario.acaid[i].checked == false && document.formulario.acaid[i].value!="") 
			{document.formulario.acaid[i].checked = true;
			retorna(i)
			};
		}
	}
  
	function visualiza(cod)
	{
		e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/acao/mostraavpar&acao=A&cod="+cod;
		window.open(e, "viewavpar", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=640,height=480'");
	}
    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }

    function escolhe_referencia()
	{
		if (document.formulario.refcod.value=='') {alert('Selecione a Refer�ncia');document.formulario.refcod.focus();}
		else {document.formulario.submit();}
 	}
 	
 	
	function discordar()
	{
		branco = '';
		for ( var i = 0; i <= document.formulario.acaid.length - 1; i++ )
		{
			if ( document.formulario.acaid[i].checked == true )
			{
				var campotexto = document.getElementById( 'avptexto' + document.formulario.acaid[i].value );
				if ( campotexto.value == '' )
				{
					branco = document.formulario.acaid[i].value;
				}
			}
		}
		if ( branco )
		{
			alert( 'O Motivo da discord�ncia n�o pode estar em branco!' );
			document.getElementById( 'avptexto' + branco ).focus();
		}
		else
		{
			document.formulario.corcod_par.value = 3;
			document.formulario.submit();
		}
	};

 	function envia( act )
	{
		if ( act == 1 ) situaca = "CONCORDO";
		if ( act == 2 ) situaca = "PENDENTE";
		if ( act == 3 ) situaca = "DISCORD�NCIA";
		
		if ( campoSelect.options[0].value == '' )
		{
			alert('Selecione as A��es a serem submetidas!');
		}
		else
		{
			if ( act == 3 )
			{
				if ( confirm( 'Ao validar as a��es como ' + situaca + ', dever� ser informado o\nMotivo da discord�ncia sobre a(s) ' + qtdmarcadas + ' A��o(�es) selecionadas.\nDeseja Continuar?' ) )
				{
					var div = null;
					var k = 0;
					while ( div = document.getElementById( 'div_cor_discordancia_' + k++ ) )
					{
						div.style.display = 'inline';
					}
					for( var i = 0; i <= document.formulario.acaid.length - 1; i++ )
					{
						if ( document.formulario.acaid[i].checked == true )
						{
							// prepara formul�rio para inser��o de dados para justificar a discord�ncia
							document.formulario.acaid[i].style.visibility = "hidden";
							document.formulario.acaid[i].style.display = "none";
							document.getElementById( "texto" + i ).style.visibility = "visible";
							document.getElementById( "texto" + i ).style.display = "";
							document.getElementById( "texto" + i ).style.background = "#ffffcc";
							document.getElementById( "td" + i ).style.color = "#cc0000";
							document.getElementById( "avptexto" + document.formulario.acaid[i].value ).disabled = 0;
						}
						else 
						{
							document.getElementById( "tabelaacao" + i ).style.visibility = "hidden";
							document.getElementById( "tabelaacao" + i ).style.display = "none";
						}
					}
					document.getElementById( "valida" ).style.height = '';
					document.getElementById( "tabelaincluir" ).style.visibility = "visible";
					document.getElementById( "tabelaincluir" ).style.display = "";
				}
			}
			else
			{
				if ( confirm('Confirma a valida��o de ' + qtdmarcadas + ' A��o(�es) como: ' + situaca + '?' ) )
				{
					document.formulario.corcod_par.value=act;
					document.formulario.submit();
				}
			}
 		}
	}
 	
 	function procura()
	{
		if ( document.formulario.prgcod == '' )
		{
			alert( 'O C�digo do Programa n�o pode ser deixado em branco!' );
		}
		else
		{
			document.formulario.numero.value = '';
			document.formulario.submit();
		}
 	}
 	
 	function pagina( numero )
 	{
	 	document.formulario.numero.value = numero;
	 	document.formulario.submit();
 	}
 	
</script>
