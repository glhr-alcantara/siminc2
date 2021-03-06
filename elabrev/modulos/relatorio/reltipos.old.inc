<?
#   Sistema Simec
#   Setor respons�vel: SPO-MEC
#   Desenvolvedor: Equipe Consultores Simec
#   Analista: Cristiano Cabral, Gilberto Arruda Cerqueira Xavier
#   Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
#   M�dulo:despesa.inc

//Recupera tudo que foi passado por REQUEST e instancia as vari�veis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;

if ( !$listaprg ) $listaprg = 'A';
if ( !$ordemlista && !$ordemlistadir )
{
		$ordemlista = "2"; $ordemlistadir = "ASC";
}

//Cabe�alho do sistema
include  APPRAIZ."includes/cabecalho.inc";

if (!$ordemgrupo) $ordemgrupo='A';
if ($ordemgrupo=='L')
{
	$agrupadopor = 'Localizador'; 
}
elseif ($ordemgrupo=='U')
{
	$agrupadopor = 'Unidade'; 
}
else
{
	$agrupadopor = 'A��o';
}

//Ordem da listagem
if (!$ordemlista) $ordemlista = 2;
//Direcionamento da ordena��o ASC / DESC
if ($ordemlistadir <> 'DESC')
{
	$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';
}
else
{
	$ordemlistadir2 = 'ASC';
}
?>
<br>
<?
switch ( str_to_upper( $listaprg ) )
{
	case 'A':
	{ 
		$tipoprgaca == "Programas";
		$sql =	'' .
		' SELECT ' .
			'prop.prgid' .
			' , ' .
			'prop.prgcod' . ' AS ' . 'codigo' . 
			' , ' .
			'prop.prgdsc' . ' AS ' . 'descricao' . 
			' , ' . 
			' COUNT ' . ' ( ' . 'a.acaid' . ' ) ' . ' AS ' . 'numacoes' .
		' FROM ' .
			'elabrev.ppaprograma_proposta prop' .
		' LEFT JOIN ' .
			'elabrev.v_propostas_acoes a' . 
		' ON ' . 
			'a.prgid' . ' = ' . 'prop.prgid' .
		' WHERE ' .
			'prop.prsano' . ' = ' . '\'' . $_SESSION['exercicio'] . '\'' .
		' GROUP BY ' .
			'prop.prgid' .
			' , ' .
			'prop.prgcod' . 
			' , ' .
			'prop.prgdsc' .
		' ORDER BY ' .
			$ordemlista .
			' ' .
			$ordemlistadir .
		'';
				
		$subtit1 = 'de Programas'; 
		$subtit2 = 'do Programa';
		break;
	}
	case 'P': 
	{	
		$wh1 = '';
		$wh = '';

		if ( $inclusao )	$wh .= ' or prop.tipo = \'I\'';
		if ( $alteracao )	$wh .= ' or prop.tipo = \'A\'';
		if ( $exclusao )	$wh .= ' or prop.tipo = \'E\'';
		if ( $fusao )		$wh .= ' or prop.tipo = \'F\'';
		if ( $wh != '' )	$wh1 = ' where 1 = 2 ' . $wh; else $wh1 = ' where 1 = 1';

		$tipoprgaca = "Programas";
		
		$sql = '' .
		' SELECT ' .
			'prop.prgid' .
			' , ' .
			'prop.prgcod' . ' AS ' . 'codigo' .
			' , ' .
			'prop.prgdsc' . ' AS ' . 'descricao' . 
			' , ' . 
			' CASE ' . 'tipo' . 
				' WHEN ' 	. '\'' . 'I' . '\'' . 
					' THEN '	. '\'' . 'Inclus�o' . '\'' . 
				' WHEN ' 	. '\'' . 'A' . '\'' .  
					' THEN ' 	. '\'' . 'Altera��o' . '\'' . 
				' WHEN ' 	. '\'' . 'F' . '\'' . 
					' THEN '	. '\'' . 'Fus�o' . '\'' . 
				' WHEN ' 	. '\'' . 'M' . '\'' . 
					' THEN ' 	. '\'' . 'Migra��o' . '\'' . 
				' WHEN ' 	. '\'' . 'E' . '\'' . 
					' THEN ' 	. '\'' . 'Exclus�o' . '\'' . 
			' END ' . ' AS ' . 'tipo' .
		' FROM ' .
			'elabrev.v_propostas_programas prop' . 
			$wh1 .
		' GROUP BY ' .
			'prop.prgid' . 
			' , ' . 
			'prop.prgcod' . 
			' , ' . 
			'prop.prgdsc' . 
			' , ' . 
			'prop.tipo' .
		' ORDER BY ' .
			$ordemlista . 
			' ' . 
			$ordemlistadir .
		'';
		
		$subtit1 = 'de Programas'; 
		$subtit2 = 'do Programa';
		break;
	}
	case 'U': 
	{	
		$tipoprgaca = "Unidades";
		$sql =	'' .
		' SELECT ' .
			' COUNT ' . ' ( ' . 'prop.acaid' . ' ) ' . ' AS ' . 'numacoes' .
			' , ' .
			'unicod' . ' AS ' .  'Codigo' . 
			' , ' .
			'unidsc' . ' AS ' . 'Descricao' .
		' FROM ' .
			'elabrev.v_propostas_acoes prop' .
		' GROUP BY ' . 'unicod' . ' , ' . 'unidsc' .
		' ORDER BY ' .
			$ordemlista . 
			' '. 
			$ordemlistadir .
		'';
		
		$subtit1 = 'de Unidades'; 
		$subtit2 = 'da Unidade';
		break;
	}
	case 'I': 
	{	
		$tipoprgaca = "Indicadores";
		$sql =	'' .
		' SELECT ' .
 			'p.prgid' . 
 			' , ' . 
 			'p.prgdsc' . ' AS ' . 'descricao' . 
 			' , ' . 
 			'p.prgcod' . ' AS ' . 'codigo' . 
 			' , ' . 
 			' COUNT ' . ' ( ' . 'i.prgid' . ' ) ' . ' AS ' . 'numacoes' .
		' FROM ' . 
			'ppaprograma_proposta p' . 
		' INNER JOIN ' . 
			'elabrev.v_propostas_indicadores i' . 
		' ON ' . 
			'i.prgid'.  ' = ' . 'p.prgid' . 
		' GROUP BY ' . 
			'p.prgid' . ' , ' . 'p.prgdsc' . 
			' , ' . 
			'p.prgcod' . 
		' ORDER BY ' . $ordemlista . ' ' . $ordemlistadir;
		
		$subtit1 = 'de Indicadores'; 
		$subtit2 = 'do Indicador';
		break;
	}
}

$_SESSION[ "debugger" ] = $sql;
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo_modulo,'Clique no c�digo para ver detalhes - Total '.$subtit1.' ('.($nlinhas+1).')');
?>
	
	<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
	    <form name="formulario" method="post">
			<input type="Hidden" name="ordemlista" value="<?=$ordemlista?>" />
			<input type="Hidden" name="ordemlistadir" value="<?=$ordemlistadir?>" />
			<tr>
				<td class="SubTituloDireita">Tipo de proposta:</td>
				<td style="color:#008000;">
					<input type="checkbox" name="inclusao" value="I"  <? if ($inclusao=='I') print 'checked';?> />
					Inclus�o
					<input type="checkbox" name="alteracao" value="A" <? if ($alteracao=='A') print 'checked';?> />
					Altera��o
					<input type="checkbox" name="migracao" value="M"  <? if ($exclusao=='E') print 'checked';?> />
					Migra��o
					<input type="checkbox" name="exclusao" value="E"  <? if ($exclusao=='E') print 'checked';?> />
					Exclus�o
					<input type="checkbox" name="fusao" value="F"  <? if ($fusao=='F') print 'checked';?>>
					Fus�o
				</td>
				<td class="SubTituloDireita">
					Agrupado por:
				</td>
				<td style="color:#008000;">
					<input type="radio" name="listaprg" value="A"  <?if ($listaprg=='A') print 'checked';?> />
					A��es 
					<input type="radio" name="listaprg" value="P"  <?if ($listaprg=='P') print 'checked';?> /> 
					Programas
					<input type="radio" name="listaprg" value="U"  <?if ($listaprg=='U') print 'checked';?> />
					Unidades
					<input type="radio" name="listaprg" value="I"  <?if ($listaprg=='I') print 'checked';?> />
					Indicadores
				</td>
				<td class="SubTituloDireita" style="text-align:center;">
					<input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="filtra();"/>
				</td>
			</tr>
		</form>
	</table>
	<? if ($nlinhas >= 0) 
	{
	?>
		<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
			<thead>
				<tr>
					<td style="width:65px;" nowrap class="title">
						<?if ($ordemlista == '2')
						{
							$ordemlistadirnova = $ordemlistadir2;
						?>
							<img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" 
							align="middle" />
						<?
						} 
						else
						{
								$ordemlistadirnova = 'ASC';
						}
						?>
						<label onclick="ordena('2','<?=$ordemlistadirnova?>');" title="Ordenar por C�digo" >
							<strong>
								C�digo
							</strong>
						</label>
					</td>
					<td width="100%" class="title">
						<? if ($ordemlista == '3')
						{
							$ordemlistadirnova = $ordemlistadir2;
						?>
							<img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" 
							align="middle" />
						<?
						}
						else
						{
							$ordemlistadirnova = 'ASC';
						}
						?>
						<label onclick="ordena('3','<?=$ordemlistadirnova?>');" title="Ordenar por T�tulo <?=$subtit2?>">
							<strong>
								T�tulo <?= $subtit2 ?>
							</strong>
						</label>
					</td>
					<td style="width:100px;" nowrap class="title" align="right">
						<? if ($ordemlista == '5')
						{
							$ordemlistadirnova = $ordemlistadir2;
						?>
							<img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" 
							align="middle">
						<?
						}
						else
						{
							$ordemlistadirnova = 'ASC';
						}
					?>
						<label onclick="ordena('5','<?=$ordemlistadirnova?>');"  title="Ordenar por Total">
							<? if ( $listaprg == 'P' )
							{
							?>
								<strong>
									Tipo
								</strong>
							<? 
							}
							else
							{
							?>
								<strong>
									Quantidade
								</strong>
							<?
							} 
							?>
						</label>
					</td>
				</tr>
			</thead>
			<tbody>
				<?
				$totalacoes = 0;
				$valor_total = 0;
				$despesa_total = 0;
				$saldo_total = 0;
				for ( $i = 0; $i <= $nlinhas; $i++ )
				{
					$res = $db->carrega_registro($RS,$i);
					// a linha abaixo transforma em vari�veis todos os campos do array
					if( is_array( $res ) )
					{
						foreach($res as $k=>$v) ${$k}=$v;
					}
					$cor = '';
					if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				?>
					<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
						<? 
						if ( $listaprg == 'A' ) 
						{ 
						?>
							<td>
								<?= $codigo; ?>
							</td>
							<td onclick="abreconteudo('geral/listaacaoproposta.php?prgid=<?=$prgid?>&tipo=<?=$listaprg?>&ordem=<?=$ordemgrupo?>&inclusao=<?=$inclusao?>&exclusao=<?=$exclusao?>&alteracao=<?=$alteracao?>&fusao=<?=$fusao?>','<?=$i?>-<?=$codigo?>')" <?if ($listaprg=='A') print 'style="color:#003c7b"';?>> 
								<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>"> <?=$descricao?>
							</td>
							<td align=right>
								(<?=$numacoes?>)
							</td>
						<?
						} 
						elseif ( $listaprg == 'P' ) 
						{ 
						?>
							<td>
								<?= $codigo; ?>
							</td>
							<td <?if ($listaprg=='P') print 'style="color:#003c7b"';?>>
								<a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/relprogramanovo&acao=A&prgid=<?=$prgid?>" target="_blank">
									<?=$descricao?>
								</a>
							</td>
							<td align=right>
								<?=$tipo?>
							</td>
						<? 
						}
						elseif ( $listaprg == 'U' ) 
						{ 
						?>
							<td>
								<?= $codigo; ?>
							</td>
							<td onclick="abreconteudo(<? if ( $listaprg== 'U' ) { ?>'geral/listaacaoproposta.php?unicod=<?=$codigo?>&tipo=<?=$listaprg?>&ordem=<?=$ordemgrupo?>&inclusao=<?=$inclusao?>&exclusao=<?=$exclusao?>&alteracao=<?=$alteracao?>&fusao=<?=$fusao?>'<?} else {?>'geral/listaacaoproposta.php?codigo=<?if ($ordemgrupo=='U') print $codigo; else print $codigo;?>&ordem=<?=$ordemgrupo?>&inclusao=<?=$inclusao?>&exclusao=<?=$exclusao?>&alteracao=<?=$alteracao?>&fusao=<?=$fusao?>'<?}?>,'<?=$i?>-<?=$codigo?>')" <?if ($listaprg=='U') print 'style="color:#003c7b"';?>> <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>">
								<?=$descricao?>
							</td>
							<td align=right>
									(<?=$numacoes?>)
								</td>
						<? 
						}
						elseif ( $listaprg == 'I' ) 
						{
						?>
							<td>
								<?= $codigo; ?>
							</td>
							<td onclick="abreconteudo(<? if ( $listaprg== 'I' ) { ?>'geral/listaacaoproposta.php?prgid=<?=$prgid?>&tipo=<?=$listaprg?>&ordem=<?=$ordemgrupo?>&&inclusao=<?=$inclusao?>&exclusao=<?=$exclusao?>&alteracao=<?=$alteracao?>&fusao=<?=$fusao?>'<?} else {?>'geral/listaacaoproposta.php?codigo=<?if ($ordemgrupo=='I') print $codigo; else print $codigo;?>&ordem=<?=$ordemgrupo?>&inclusao=<?=$inclusao?>&exclusao=<?=$exclusao?>&alteracao=<?=$alteracao?>&fusao=<?=$fusao?>'<?}?>,'<?=$i?>-<?=$codigo?>')" <?if ($listaprg=='I') print 'style="color:#003c7b"';?>> <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>">
								<?=$descricao?>
							</td>
							<td align=right>
								(<?=$numacoes?>)
							</td>
						<? 
						} 
						?>
					</tr>
					 <tr bgcolor="<?=$marcado?>">
						 <td></td>
						 <td id="td<?=$i?>-<?=$codigo?>"></td><td></td>
					 </tr>
				<? 
				} 
				?>
				<? $marcado = fmod( $i, 2 ) == 0 ? '' : 'marcado'; ?>
				<tr class="<?= $marcado ?>">
					<td></td>
					<td valign="top" class="title" colspan="<?if ($listaprg=='A') print '3'; ?>"> 
						<table width="100%">
						  	<tr>
				  				<td style="border:none;">
				  					<strong>
				  						Total <?=$subtit1?>: <?=$i?>
						  			</strong>
						  		</td>
						  		<td align="right" style="border:none;">
						  			<strong>
						  				Total de Localizadores de Gasto:
				  					</strong>
				  				</td>
							</tr>
						</table>
					</td>
					<td align="right" >
						<strong>
							<?= $totalacoes ?>
						</strong>
					</td>
				</tr>
			</tbody>
		</table>
	<? 
	} 
	else 
	{ 
	?>
		<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" 
		bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
			<tr>
				<td align="center">
					N�o foram encontrados registros
				</td>
			</tr>
		</table>
	<? 
	}
	?>
	<!-- 
				</td>
			</tr>
		</table>
	</center>
	 -->
</div>
<script language="JavaScript">
	function filtra()
	{
		document.formulario.submit();
	}
	
	function ordena( ordem , direcao)
	{
		document.formulario.ordemlista.value=ordem;
		document.formulario.ordemlistadir.value=direcao;
		document.formulario.submit();
	}
</script>

