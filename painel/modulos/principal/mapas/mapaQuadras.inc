<?php
if($_REQUEST['requisicao'])
{
	ob_clean();
	$_REQUEST['requisicao']();
	die;
}

// monta cabe�alho 
if(!$sem_cabecalho) {
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
}


function gerarXml() {
	global $db;
	header ("Content-Type:text/xml");
	$sql = montaSQL();
	$dados = $db->carregar($sql);
	
	if($dados):
		
		$conteudo .= "<markers>"; // inicia o XML
		
		foreach($dados as $d):
										
			$conteudo .= "<marker "; //inicia um ponto no mapa
			$conteudo .= "info=\"". $d['info'] ."\" ";
			$conteudo .= "lat=\"". $d['lat'] ."\" ";
			$conteudo .= "lng=\"". $d['lng'] ."\" ";
			$conteudo .= "icon=\"". $d['icon'] ."\" ";
			$conteudo .= "/>";
		
		endforeach;
		
		$conteudo .= "</markers>";
		print $conteudo;
		
	endif;
}


function montaBalao() {
	global $db;
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	$mundados = $db->pegaLinha("SELECT mundescricao||' - '||estuf as cabecalhomun, munpopulacao FROM territorios.municipio WHERE muncod='".$_REQUEST['muncod']."'");
	$dadoproinfancia = $db->pegaLinha("SELECT * FROM painel.dadoproinfancia WHERE trim(muncod)='".$_REQUEST['muncod']."'");
	
	echo "<table align=center border=0 class=listagem cellpadding=3 cellspacing=1 width=100%>";
	echo "<tr><td class=SubTituloCentro colspan=2>".$mundados['cabecalhomun']." (Grupo ".$dadoproinfancia['dpitipogrupo'].")</td></tr>";
	echo "<tr>";

	echo "<td valign=top><table align=center border=0 class=tabela cellpadding=3 cellspacing=1>";
	echo "<tr><td class=SubTituloCentro colspan=2>Unidades de quadras</td></tr>";
	echo "<tr><td class=SubTituloDireita>Quantidade de escolas municipais sem quadra</td><td>".$dadoproinfancia['dpiescolamunsemquadra']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Quantidade de escolas municipais com quadra coberta</td><td>".$dadoproinfancia['dpiescolamuncomcobertura']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Quantidade de escolas municipais com quadra descoberta</td><td>".$dadoproinfancia['dpiescolamunsemcobertura']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Quantidade de escolas estaduais sem quadra</td><td>".$dadoproinfancia['dpiescolaestsemquadra']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Quantidade de escolas estaduais com quadra coberta</td><td>".$dadoproinfancia['dpiescolaestcomcobertura']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Quantidade de escolas estaduais com quadra descoberta</td><td>".$dadoproinfancia['dpiescolaestsemcobertura']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Proposta de constru��o de quadras cobertas municipais</td><td>".$dadoproinfancia['dpipropostaquadracobertamun']."</td></tr>";	
	echo "<tr><td class=SubTituloDireita>Proposta de cobertura de quadras municipais</td><td>".$dadoproinfancia['dpipropostacoberturamun']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Proposta de constru��o de quadras cobertas estaduais</td><td>".$dadoproinfancia['dpipropostaquadracobertaest']."</td></tr>";	
	echo "<tr><td class=SubTituloDireita>Proposta de cobertura de quadras estaduais</td><td>".$dadoproinfancia['dpipropostacoberturaest']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Proposta de constru��o de quadras cobertas total</td><td>".($dadoproinfancia['dpipropostaquadracobertamun']+$dadoproinfancia['dpipropostaquadracobertaest'])."</td></tr>";	
	echo "<tr><td class=SubTituloDireita>Proposta de cobertura de quadras total</td><td>".($dadoproinfancia['dpipropostacoberturamun']+$dadoproinfancia['dpipropostacoberturaest'])."</td></tr>";
	
	echo "</table></td></tr>";
	
	echo "<tr><td class=SubTituloEsquerda colspan=2>Quadras</td></tr>";
	
	$sql = "SELECT upper(predescricao) as predescricao, preid FROM obras.preobra p 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			WHERE p.muncod='".$_REQUEST['muncod']."' AND d.esdid='228' AND p.ptoid=5 AND p.tooid=1";
	
	$obras = $db->carregar($sql);
	
	if($obras[0]) {
		foreach($obras as $obra) {
			echo "<tr><td colspan=2>".$obra['predescricao']."</td></tr>";
			$sql = "SELECT '<img title='||p.pofdescricao||' border=1px; id='||p.arqid||' src=../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid='||p.arqid||'&_sisarquivo=obras hspace=10 vspace=3 style=position:relative;z-index:5;float:left;width:70px; height:70px; onmouseover=\"return escape( \''||p.pofdescricao||'\' );\" onclick=\"javascript:window.open(\'../slideshow/slideshow/ajustarimgparam3.php?pagina=0&_sisarquivo=obras&preid={$obra['preid']}\',\'imagem\',\'width=850,height=600,resizable=yes\')\">' as fot,
						   to_char(a.arqdata, 'DD/MM/YYYY') as data  
					FROM obras.preobrafotos p
					INNER JOIN public.arquivo a ON a.arqid = p.arqid 
					WHERE p.preid='".$obra['preid']."' ORDER BY a.arqdata DESC LIMIT 5";
			$fotos = $db->carregar($sql);
			if($fotos[0]) {
				echo "<tr><td colspan=2>";
				echo "<table>";
				foreach($fotos as $foto) {
					echo "<td>".$foto['fot']."<br>".$foto['data']."</td>";
				}
				echo "</table>";
				echo "</td></tr>";
			}
		}
	} else {
		echo "<tr><td colspan=2>N�o existem obras</td></tr>";
	}

	echo "</table>";
}

function montaSQL() {
	
	$filtro[] = "m.munlatlong IS NOT NULL";
	
	if( $_REQUEST['estuf'][0] ){
		$filtro[] = "m.estuf in ('".implode("','",$_REQUEST['estuf'])."') ";
	}
	if( $_REQUEST['muncod'][0] ){
		$filtro[] = "m.muncod in ('".implode("','",$_REQUEST['muncod'])."') ";
	}
	
	if( $_REQUEST['territorioscidadania'] ){
		$filtro[] = "dpiterritoriocidadania=TRUE";
	}
	if( $_REQUEST['grupo'] ){
		$filtro[] = "dpitipogrupo IN('".implode("','",$_REQUEST['grupo'])."')";
	}
	
	if($_REQUEST['esfera'] == "todos") {
		$qtdsemquadras = '(dpiescolaestsemquadra+dpiescolamunsemquadra)';
		$qtdquadrascobertura = '(dpiescolaestcomcobertura+dpiescolamuncomcobertura)';
		$qtdquadrasdescobertas = '(dpiescolamunsemcobertura+dpiescolaestsemcobertura)';
		$qtdquadrascoberta = '(dpipropostaquadracobertaest+dpipropostaquadracobertamun)';
		$qtdquadracobertura = '(dpipropostacoberturaest+dpipropostacoberturamun)';
		$dpiquadrapac2 = '(dpiquadrapac2)';
	} elseif($_REQUEST['esfera'] == "mun") {
		$qtdsemquadras = '(dpiescolamunsemquadra)';
		$qtdquadrascobertura = '(dpiescolamuncomcobertura)';
		$qtdquadrasdescobertas = '(dpiescolamunsemcobertura)';
		$qtdquadrascoberta = '(dpipropostaquadracobertamun)';
		$qtdquadracobertura = '(dpipropostacoberturamun)';
		$dpiquadrapac2 = '(dpiquadrapac2)';
	} elseif($_REQUEST['esfera'] == "est") {
		$qtdsemquadras = '(dpiescolaestsemquadra)';
		$qtdquadrascobertura = '(dpiescolaestcomcobertura)';
		$qtdquadrasdescobertas = '(dpiescolaestsemcobertura)';
		$qtdquadrascoberta = '(dpipropostaquadracobertaest)';
		$qtdquadracobertura = '(dpipropostacoberturaest)';
		$dpiquadrapac2 = '0';
	}
	
	if($_REQUEST['icone']){
		if(in_array('dpiescolasemquadra',$_REQUEST['icone'])) {
			$filtroesp[] = $qtdsemquadras.">0";
		}
		if(in_array('dpiescolacomcobertura',$_REQUEST['icone'])) {
			$filtroesp[] = $qtdquadrascobertura.">0";
		}
		if(in_array('dpiescolasemcobertura',$_REQUEST['icone'])) {
			$filtroesp[] = $qtdquadrasdescobertas.">0";
		}
		
		if(in_array('dpipropostaquadracoberta',$_REQUEST['icone'])) {
			$filtroesp[] = $qtdquadrascoberta.">0";
		}
		if(in_array('dpipropostacobertura',$_REQUEST['icone'])) {
			$filtroesp[] = $qtdquadracobertura.">0";
		}
		if(in_array('dpiquadrapac2',$_REQUEST['icone'])) {
			$filtroesp[] = $dpiquadrapac2.">0";
		}
	} 
	
	if($filtro || $filtroesp) {
		$cl = "WHERE ";
		if($filtro) {
			$cls[] = implode($filtro," AND ");
		}
		if($filtroesp) {
			$cls[] = "(".implode($filtroesp," ".$_REQUEST['contexto']." ").")";
		}
		$cl .= implode(" AND ", $cls);
	}
	
	$sql = "select  m.muncod,
				    r.regdescricao as regiao,
				    e.estuf as uf,
				    'Grupo '||dpitipogrupo as grupo,
   				    '<span style=cursor:pointer onclick=localizaMapa2(\''||m.muncod||'\',\''||ST_X(m.munlatlong)||'\',\''||ST_Y(m.munlatlong)||'\')>'||m.mundescricao||'</span>' as municipio,
   				    dpipopulacao0_5 as popul,
   				    CASE WHEN dpideficit0_5 > 0 THEN dpideficit0_5 ELSE 0 END as defic,
   				    dpimcmv         as mcmv,
   				    {$qtdquadrascobertura} as qtdquadrascobertura,
   				    {$qtdsemquadras} as qtdsemquadras,
   				    {$qtdquadrasdescobertas} as qtdquadrasdescobertas,
   				    {$qtdquadrascoberta} as qtdquadrascoberta,
   				    {$qtdquadracobertura} as qtdquadracobertura,
   				    {$dpiquadrapac2} as qtdquadrasaprpac2,
   				    ST_X(m.munlatlong) as lng,
   				    ST_Y(m.munlatlong) as lat,
   				    '&lt;iframe class=iframeBalao src=?modulo=principal/mapas/mapaQuadrasEEacao=AEErequisicao=montaBalaoEEmuncod'||m.muncod||' &gt;&lt;/iframe&gt;' as info,
   				    '/imagens/tachinha_b.png' as icon
			FROM painel.dadoproinfancia d 
			INNER JOIN territoriosgeo.municipio m on m.muncod = d.muncod 
			INNER JOIN territoriosgeo.estado e on e.estuf = m.estuf 
			INNER JOIN territoriosgeo.regiao r on r.regcod = e.regcod 
			{$cl} ORDER BY ".implode(",",$_REQUEST['agrupador']);
			
	return $sql;
}


function agp_relatorio(){
	
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array("qtdquadrascobertura",
										  "qtdsemquadras",
										  "qtdquadrasdescobertas",
										  "qtdquadrascoberta",
										  "qtdquadracobertura",
										  "qtdquadrasaprpac2"),
				"agrupadorDetalhamento" => array(
													array(
															"campo" => "regiao",
															"label" => "Regi�o"
														  ),
													array(
															"campo" => "uf",
															"label" => "UF"
														  ),
													array(
															"campo" => "municipio",
															"label" => "Munic�pio"
														  ),
													array(
															"campo" => "grupo",
															"label" => "Grupo"
														  )				  
														  
												)	  
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "regiao":
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											  		"label" => "Regi�o")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Munic�pio")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "UF")										
									   				);
			break;
			case "grupo":
				array_push($agp['agrupador'], array(
													"campo" => "grupo",
											  		"label" => "Grupo")										
									   				);
			break;
			
		}	
	}
	
	return $agp;
	
}

function carregarParametroMapa()
{
	global $db;
	switch($_GET['parametro'])
	{
		case "populacao":
			$sql = "select 
						munpopulacao as parametro,
						muncod,
						'{$_GET['parametro']}' as tipoparametro
					from 
						territorios.municipio";
		break;
		default:
			$sql = "select muncod,".str_replace("_mais_","+",$_GET['parametro'])." as parametro,'".$_GET['parametro']."' as tipoparametro from painel.dadoproinfancia pro ";
		break;
	}
	$dados = $db->carregar($sql); 
	echo JSON_encode($dados);
}


function gerarRelatorio() {
	global $db;
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	$sql = montaSQL();

	$agrupador = agp_relatorio();
	$dados = $db->carregar( $sql );

	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "qtdsemquadras",
					   		   "label" 	  => "Escolas sem quadras",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "qtdquadrascobertura",
					   		   "label" 	  => "Escolas com quadra coberta",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "qtdquadrasdescobertas",
					   		   "label" 	  => "Escolas com quadra descoberta",
					   		   "type"	  => "numeric") );
	
	array_push( $coluna, array("campo" 	  => "qtdquadrasaprpac2",
					   		   "label" 	  => "Quadras aprovadas",
					   		   "type"	  => "numeric") );
	
	
	array_push( $coluna, array("campo" 	  => "qtdquadrascoberta",
					   		   "label" 	  => "Proposta de constru��o de quadras cobertas",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "qtdquadracobertura",
					   		   "label" 	  => "Proposta de cobertura de quadras",
					   		   "type"	  => "numeric") );
	
	$coltop = array( 0 => array( "label" => "","rowspan" => 2 ),
					1 => array( "label" => "","rowspan" => 2 ),
					2 => array( "label" => "","rowspan" => 2 ),
					3 => array( "label" => "PAC 2" , "colspan" => 3 )
							
				);
	
	$rel = new montaRelatorio();
	$rel->setColuna($coluna);
	$rel->setColunaTop($coltop);
	$rel->setTolizadorLinha(true);
	$rel->setMonstrarTolizadorNivel(true);
	$rel->setTotalizador(true);
	$rel->setAgrupador($agrupador, $dados);
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
}



function carregaParametro()
{
	global $db;
	
	switch($_GET['parametro']){
		case "populacao":
			$arrSteps[] = array( "inicio" => 0 , "fim" => 10000 );
			$arrSteps[] = array( "inicio" => 10001 , "fim" => 50000 );
			$arrSteps[] = array( "inicio" => 50001 , "fim" => 100000 );
			$arrSteps[] = array( "inicio" => 100001 , "fim" => 200000 );
			$arrSteps[] = array( "inicio" => 200001 , "fim" => 11500000 );
		break;
		default:
			$sql = "select 
						parametro,
						count(1) as qtde
					from (
							select 
								mun.muncod,
								".str_replace("_mais_","+",$_GET['parametro'])." as parametro
							from 
								painel.dadoproinfancia pro
							inner join
								territoriosgeo.municipio mun ON mun.muncod = pro.muncod
							where
								".str_replace("_mais_","+",$_GET['parametro'])." >= 0
						  ) as foo 
					group by 
						parametro
					order by 
						1";
			
			$arrDados = $db->carregar($sql);
			
			$sqlChave = "select sum(qtde) - {valorChave} from (
							select 
									parametro,
									count(1) as qtde
								from (
										select 
											mun.muncod,
											".str_replace("_mais_","+",$_GET['parametro'])." as parametro
										from 
											painel.dadoproinfancia pro
										inner join
											territoriosgeo.municipio mun ON mun.muncod = pro.muncod
										where
											".str_replace("_mais_","+",$_GET['parametro'])." >= 0
									  ) as foo 
								group by 
									parametro
								order by 
									1 ) as foo2";
			
			if($arrDados){
				$numLegenda = 5;
				$valorChave = 5565;
				//$valorChave = 1113;
				(float)$num = 0; //incia o n�mero de soma acumulada igual a zero
				$arrSteps = array();

				foreach($arrDados as $linha => $dado){
					if(count($arrSteps) < ($valorChave/$numLegenda)){//apenas 5 legendas
						if($dado['qtde'] >= $valorChave){//se a qtde for maior que a chave
							$arrSteps[] = array("inicio" => $dado['parametro'], "fim" => $dado['parametro']);
							(float)$valorChave = $db->pegaUm(str_replace("{valorChave}",$dado['qtde'],$sqlChave)); //toral sem o n�mero
						}else{ //se n�o for maior que a chave
							$arrLeg[] = $dado['parametro']; //grava o parametro
							$num+=(float)$dado['qtde']; //soma o numero de qtde
							if($num >= ($valorChave/($numLegenda-count($arrSteps))) ){ //se o numero form maior que a chave
								$arrSteps[] = array("inicio" => $arrLeg[0], "fim" => $arrLeg[count($arrLeg)-1]); //atribui o inicio e o fim da legenda
								unset($arrLeg); //reinicia o array de legendas
							}	
						}
					}
				}
			}
		break;
	}
	
	$arrCores["dpiescolaestcomcobertura"] = "#0000FF";
	$arrCores["dpiescolaestsemcobertura"] = "#0000FF";
	$arrCores["dpiescolaestsemquadra"] = "#FF0000";
	$arrCores["dpiescolamuncomcobertura"] = "#0000FF";
	$arrCores["dpiescolamunsemquadra"] = "#FF0000";
	$arrCores["dpiescolamunsemcobertura"] = "#FF0000";
	$arrCores["dpipropostaquadracoberta"] = "#0000FF";
	$arrCores["dpipropostacobertura"] = "#0000FF";
	$arrCores['dpipropostaquadracobertamun'] = "#0000FF";
	$arrCores['dpipropostacoberturamun'] = "#0000FF";
	$arrCores['dpipropostaquadracobertaest'] = "#0000FF";
	$arrCores['dpipropostacoberturaest'] = "#0000FF";
	//escola total sem quadra
	$arrCores['dpiescolamunsemquadra_mais_dpiescolaestsemquadra'] = "#FF0000";
	$arrCores['dpiescolaestsemcobertura_mais_dpiescolamunsemcobertura'] = "#FF0000";
	
	$arrCores['(dpipropostaquadracobertaest_mais_dpipropostaquadracobertamun)'] = "#0000FF";
	$arrCores['dpipropostacoberturaest_mais_dpipropostacoberturamun'] = "#0000FF";
	
	$i = 1;
	if($arrSteps){
		$arrJS[] = "arrIncio['{$_GET['parametro']}'] = false; arrIncio['{$_GET['parametro']}'] = new Array(); ";
		$arrJS[] = "arrFim['{$_GET['parametro']}'] = false; arrFim['{$_GET['parametro']}'] = new Array(); ";
		foreach($arrSteps as $i => $range){
			if($range['inicio'] >= 0 && $range['fim'] >= 0){
				if($range['inicio'] == $range['fim']){
					echo "<div style=\"width:200px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;\" ></div> <div style=\"float:left;\" >".(strstr($_GET['parametro'],"percent") ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'))."</div> </div> ";
				}else{
					echo "<div style=\"width:200px;height:20px;\" > <div class=\"\" id=\"div_range_".($i+1)."\" style=\"border:solid 1px black;float:left;width:15px;height:15px;margin-right:3px;\" ></div> <div style=\"float:left;\" >".(strstr($_GET['parametro'],"percent") ?number_format($range['inicio'],2,',','.')."%" : number_format($range['inicio'],0,'','.'))." - ".(strstr($_GET['parametro'],"percent") ?number_format($range['fim'],2,',','.')."%" : number_format($range['fim'],0,'','.'))."</div> </div> ";	
				}
				$arrJS[] = "var cor = retornarCor('#dddddd', '".$arrCores[$_GET['parametro']]."', 5 , ".($i+1)." ); document.getElementById('div_range_".($i+1)."').style.background=cor; document.getElementById('div_range_".($i+1)."').setAttribute(\"class\",cor);";
				$arrJS[] = "arrIncio['{$_GET['parametro']}'][$i] = ".(strstr($_GET['parametro'],"percent") ? $range['inicio'] : round($range['inicio'],0) ).";";
				$arrJS[] = "arrFim['{$_GET['parametro']}'][$i] = ".(strstr($_GET['parametro'],"percent") ? $range['fim'] : round($range['fim'],0) )."; ";
			}
		}
	}
?>
	<input type="button" onclick="carregarParametroMapa('<?php echo $_GET['parametro'] ?>');carregarParametroGrafico('<?php echo $_GET['parametro'] ?>')" name="btn_parametro" value="Carregar" />
	<script type="text/javascript">
	<?php if($arrJS): ?>
		<?php foreach($arrJS as $js): ?>
			<?php echo $js; ?>
		<?php endforeach; ?>
	<?php endif; ?>
	</script> <?php
}



function mapaFiltros() {
	global $db;
?>
<script>
var arrIncio = new Array();
var arrFim = new Array();

jQuery(function() {
	 jQuery('[name="combo_parametro"]').change(function() {
	 	var parametro = jQuery(this).val();
	 	if(parametro)
	 	{
	 		exibeParametroMapa(parametro);
	 		jQuery("#grafico_parametro").html("<div id=\"grafico_mapa\" ></div>");
	 	}else{
	 		jQuery("#span_parametro").html("");
	 		jQuery("#div_parametro").html("");
	 		jQuery("#grafico_parametro").html("<div id=\"grafico_mapa\" ></div>");
	 	}
	 });
});

function exibeParametroMapa(parametro)
{
	jQuery("#span_parametro").html(jQuery('[name="combo_parametro"] option:selected').text());
 	var url = "?modulo=principal/mapas/mapaQuadras&acao=A&requisicao=carregaParametro&parametro=" + parametro;
 	jQuery.ajax({
			type: "POST",
			url: url,
			async: false,
			success: function(response){
				jQuery("#div_parametro").html(response);
			}
		});
}

html_municipio = "<div style=\"padding:5px\" ><iframe src=\"?modulo=principal/mapas/mapaQuadras&acao=A&requisicao=montaBalao&muncod={muncod}\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"500px\" ></iframe></div>";

function mostrar_painel(painel) {
	if(document.getElementById(painel).style.display == "none") {
		document.getElementById("img_"+painel).src="../imagens/menos.gif";
		document.getElementById(painel).style.display = "";
	} else {
		document.getElementById("img_"+painel).src="../imagens/mais.gif";
		document.getElementById(painel).style.display = "none";
	}
}

function restaurarItens() {
	removeAllOptions(document.getElementById('agrupador'));
    addOption(document.getElementById('agrupador'),"Regi�o","regiao",false);
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Munic�pio","municipio",false);
}

function carregarParametroMapa(parametro)
{
	
	divCarregando();
	
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	
	var arrCores = new Array();
	
	arrCores['dpiescolaestcomcobertura'] = "#0000FF";
	arrCores['dpiescolaestsemcobertura'] = "#0000FF";
	arrCores['dpiescolaestsemquadra'] = "#FF0000";
	arrCores['dpiescolamuncomcobertura'] = "#FF0000";
	arrCores['dpiescolamunsemquadra'] = "#FF0000";
	arrCores['dpiescolamunsemcobertura'] = "#FF0000";
		
	arrCores['dpipropostaquadracobertamun'] = "#0000FF";
	arrCores['dpipropostacoberturamun'] = "#0000FF";
	arrCores['dpipropostaquadracobertaest'] = "#0000FF";
	arrCores['dpipropostacoberturaest'] = "#0000FF";
	arrCores['dpiescolamunsemquadra_mais_dpiescolaestsemquadra'] = "#FF0000";
	arrCores['dpiescolaestsemcobertura_mais_dpiescolamunsemcobertura'] = "#FF0000";
	arrCores['(dpipropostaquadracobertaest_mais_dpipropostaquadracobertamun)'] = "#0000FF";
	arrCores['dpipropostacoberturaest_mais_dpipropostacoberturamun'] = "#0000FF";
			
	var url = "?modulo=principal/mapas/mapaQuadras&acao=A&requisicao=carregarParametroMapa&parametro=" + parametro + "&estuf=" + arrUF;	
	jQuery.ajax({
		type: "POST",
		url: url,
		async: false,
		dataType:'JSON',
		success: function(response){
			response = jQuery.parseJSON(response);
			jQuery.each(response,function(index,item){
			   	if(nomePoli[item.muncod]){
			   		var posicao = retornaPosicaoMunicipio(item.parametro,item.tipoparametro);
			   		var cor = '';
					if(posicao)
					{
						cor = '' + retornarCor('#dddddd', arrCores[item.tipoparametro] , 5 , posicao );
					}
			   		f_mudacor(item.muncod,cor);
			   	}
			});
			divCarregado();
		}
	});
}


function restaurarItens() {
	removeAllOptions(document.getElementById('agrupador'));
    addOption(document.getElementById('agrupador'),"Regi�o","regiao",false);
    addOption(document.getElementById('agrupador'),"UF","uf",false);
    addOption(document.getElementById('agrupador'),"Munic�pio","municipio",false);
    }


function carregarMapasPontos(tipo,carrega) {
	
	if(pontoMarcadores['/imagens/tachinha_b.png']) {
		deleteOverlays(pontoMarcadores['/imagens/tachinha_b.png']);
	}
	
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );
	
	var filtros='';
	filtros += '&contexto='+jQuery('#contexto').val();
	filtros += '&'+jQuery('#esfera').serialize();
	if(jQuery("[name^='grupo[']:checked").length>0) {
		filtros += '&'+jQuery("[name^='grupo[']").serialize();
	}
	if(jQuery("[name^='territorioscidadania']:checked").length>0) {
		filtros += '&territorioscidadania=true';
	}
	filtros += '&'+jQuery('#estuf').serialize();
	filtros += '&'+jQuery('#muncod').serialize();
	filtros += '&'+jQuery('#agrupador').serialize();
	if(jQuery("[name^='icone']:checked").length>0) {
		filtros += '&'+jQuery("[name^='icone']").serialize();
	}
	if(!carrega){
		divCarregando();
	}
	preencherRelatorio("?modulo=principal/mapas/mapaQuadras&acao=A&requisicao=gerarRelatorio"+filtros);
	marcarPontos("?modulo=principal/mapas/mapaQuadras&acao=A&requisicao=gerarXml"+filtros);
	divCarregado();

}

function carregarMapasPontosGoogle(tipo) {
	
	if(pontoMarcadores['/imagens/tachinha_b.png']) {
		deleteOverlays(pontoMarcadores['/imagens/tachinha_b.png']);
	}
	
	selectAllOptions( document.getElementById( 'estuf' ) );
	selectAllOptions( document.getElementById( 'muncod' ) );		
	selectAllOptions( document.getElementById( 'agrupador' ) );
	
	var filtros='';
	filtros += '&contexto='+jQuery('#contexto').val();
	filtros += '&'+jQuery('#esfera').serialize();
	if(jQuery("[name^='grupo[']:checked").length>0) {
		filtros += '&'+jQuery("[name^='grupo[']").serialize();
	}
	if(jQuery("[name^='territorioscidadania']:checked").length>0) {
		filtros += '&territorioscidadania=true';
	}
	filtros += '&'+jQuery('#estuf').serialize();
	filtros += '&'+jQuery('#muncod').serialize();
	filtros += '&'+jQuery('#agrupador').serialize();
	if(jQuery("[name^='icone']:checked").length>0) {
		filtros += '&'+jQuery("[name^='icone']").serialize();
	}
	
	divCarregando();
	marcarPontos("?modulo=principal/mapas/mapaQuadras&acao=A&requisicao=gerarXml"+filtros);
	divCarregado();

}

</script>

<?php if($_SESSION['sem_cabecalho']){ ?>

	<input type="checkbox" style="display:none" name="icone[]" id="chk_1" checked="checked" value="dpiescolasemquadra"  />
	
	<input type="checkbox" style="display:none" name="icone[]" id="chk_2" checked="checked" value="dpiescolacomcobertura"  />
	
	<input type="checkbox" style="display:none" name="icone[]" id="chk_3" checked="checked" value="dpiescolasemcobertura"  />
	
	<input type="checkbox" style="display:none" name="semcreche" checked="checked" id="chk_4" value="dpipropostaquadracoberta"  />
	
	<input type="checkbox" style="display:none" name="icone[]" checked="checked" id="chk_5" value="dpipropostacobertura"  />
	
	<input type="checkbox" style="display:none" name="icone[]" checked="checked" id="chk_6" value="dpiquadrapac2"  />
	<div style="padding:3px;" >
	<b>Esfera:</b> <?
	$_opcontexto = array(0 => array('codigo'=>'todos','descricao'=>'Todos'),
						 1 => array('codigo'=>'mun','descricao'=>'Municipal'),
						 2 => array('codigo'=>'est','descricao'=>'Estadual')
						 );
	$db->monta_combo('esfera', $_opcontexto, 'S', '', '', '', '', '', 'N', 'esfera');
	?>
	</div>
	<input type="hidden" name="contexto" id="contexto" value="OR" />

<?php }else{ ?>

		<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td><input type="checkbox" name="icone[]"    id="chk_1" value="dpiescolasemquadra"  /></td>
				<td><font size=1>1 - Escolas sem quadra</font></td>
			</tr>
			
			<tr>
				<td><input type="checkbox" name="icone[]"    id="chk_2" value="dpiescolacomcobertura"  /></td>
				<td><font size=1>2 - Escolas com quadra coberta (ou em cobertura)</font></td>
			</tr>
			<tr>
				<td><input type="checkbox" name="icone[]"    id="chk_3" value="dpiescolasemcobertura"  /></td>
				<td><font size=1>3 - Escolas com quadra descoberta</font></td>
			</tr>
			<tr>
				<td><input type="checkbox" name="icone[]"    id="chk_4" value="dpipropostaquadracoberta"  /></td>
				<td><font size=1>4 - Proposta de constru��o de quadras cobertas PAC2</font></td>
			</tr>
			<tr>
				<td><input type="checkbox" name="icone[]"    id="chk_5" value="dpipropostacobertura"  /></td>
				<td><font size=1>5 - Proposta de cobertura de quadras PAC2</font></td>
			</tr>
			<tr>
				<td><input type="checkbox" name="icone[]"    id="chk_6" value="dpiquadrapac2"  /></td>
				<td><font size=1>6 - Quadras aprovadas PAC2</font></td>
			</tr>
			<tr>
				<td colspan="3">
				<b>Esfera:</b> <?
				$_opcontexto = array(0 => array('codigo'=>'todos','descricao'=>'Todos'),
									 1 => array('codigo'=>'mun','descricao'=>'Municipal'),
									 2 => array('codigo'=>'est','descricao'=>'Estadual')
									 );
				$db->monta_combo('esfera', $_opcontexto, 'S', '', '', '', '', '', 'N', 'esfera');
				?>
				</td>
			</tr>
			<tr>
				<td colspan="3">
				<b>Contexto:</b> <?
				$_opcontexto = array(0 => array('codigo'=>'OR','descricao'=>'OU'),
									 1 => array('codigo'=>'AND','descricao'=>'E'));
				$db->monta_combo('contexto', $_opcontexto, 'S', '', '', '', '', '', 'N', 'contexto');
				?>
				</td>
			</tr>
		</table>
		<br>
		<br>
<?php } ?>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_uf" onclick="mostrar_painel('uf');" border=0> UF
	</td>
</tr>
<tr>
	<td>
		<div id="uf" style="display:none">
			<?php
			$sql = "	SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM 
							territorios.estado
						ORDER BY
							estdescricao ";

			combo_popup( 'estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 240, '', '' );
			?>
		</div>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_municipio" onclick="mostrar_painel('municipio');" border=0> Munic�pio
	</td>
</tr>
<tr>
	<td>
		<div id="municipio" style="display:none">
			<?php
			$sql = " 	SELECT	
							muncod AS codigo,
							mundescricao AS descricao
						FROM 
							territorios.municipio
						ORDER BY
							mundescricao";

			combo_popup( 'muncod', $sql, 'Selecione os Munic�pios', '400x400', 0, array(), '', 'S', false, false, 5, 240);							?>
		</div>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda">
		<img style="cursor: pointer" src="../imagens/mais.gif" id="img_grupomunicipio" onclick="mostrar_painel('grupomunicipio');" border=0> Grupo Munic�pio
	</td>
</tr>
<tr>
	<td>
		<div id="grupomunicipio" style="display:none">
		<input type="checkbox" name="grupo[]"    			  id="grp_1" value="1"  /> <font size=1>Grupo 1</font><br/>
		<input type="checkbox" name="grupo[]"    			  id="grp_2" value="2"  /> <font size=1>Grupo 2</font><br/>
		<input type="checkbox" name="grupo[]"    			  id="grp_3" value="3"  /> <font size=1>Grupo 3</font><br/>
		<input type="checkbox" name="territorioscidadania"    id="grp_4" value="territorioscidadania"  /> <font size=1>Territ�rios da Cidadania</font><br/>
		</div>
	</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" ><img style="cursor:pointer" id="img_agrup" onclick="mostrar_painel('agrup');" src="/imagens/mais.gif"> Agrupadores</td>
</tr>
<tr>
	<td>
	<div style="display:none" id="agrup">
	<table width="100%">
	<tr>
			<td>
				<select id="agrupador" name="agrupador[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" class="combo campoEstilo">
				<option value="grupo">Grupo</option>
				<option value="regiao">Regi�o</option>
				<option value="uf">UF</option>
				<option value="municipio">Munic�pio</option>
				</select>
			</td>
			<td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( 'agrupador' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( 'agrupador' ) );"/><br/>
			</td>
	</tr>
	<tr>
		<td colspan="2" align="right"><input type="button" name="removeritem" value="Remover" onclick="removeSelectedOptions(document.getElementById('agrupador'));"> <input type="button" name="restauraritens" value="Restaurar" onclick="restaurarItens();"></td>
	</tr>
	</table>
	<input type="hidden" name="hdn_agrup" id="hdn_agrup" value="0" />
	</div>
	</td>
</tr>
</table>
<p>
<input type="button" name="carregar" value="Carregar" onclick="carregarMapasPontos(1);">
<input type="button" name="limpar" value="Limpar pontos" onclick="deleteOverlays(pontoMarcadores['/imagens/icone_capacete_4.png']);">
<!-- <input type="button" value="Carregar Tela" onclick="carregarMapasPontos(2);"> -->
</p>
<?
}

function mapaTemas()
{
	global $db;
?>
<?php $arrParametros = array(
							array("codigo" => "dpiescolaestcomcobertura", "descricao" => "N� de escolas estaduais com quadra coberta"),
							array("codigo" => "dpiescolaestsemcobertura", "descricao" => "N� de escolas estaduais com quadra descoberta"),
							array("codigo" => "dpiescolaestsemquadra", "descricao" => "N� de escolas estaduais sem quadra"),
							array("codigo" => "dpiescolamuncomcobertura", "descricao" => "N� de escolas municipais com quadra coberta (ou em cobertura)"),
							array("codigo" => "dpiescolamunsemquadra", "descricao" => "N� de escolas municipais sem quadra"),
							array("codigo" => "dpiescolamunsemcobertura", "descricao" => "N� de escolas municipais com quadra descoberta"),
							array("codigo" => "dpipropostaquadracobertamun", "descricao" => "Proposta de constru��o de quadras cobertas municipais PAC 2"),
							array("codigo" => "dpipropostacoberturamun", "descricao" => "Proposta de cobertura de quadras municipais PAC 2"),
							array("codigo" => "dpipropostaquadracobertaest", "descricao" => "Proposta de constru��o de quadras cobertas estaduais PAC 2"),
							array("codigo" => "dpipropostacoberturaest", "descricao" => "Proposta de cobertura de quadras estaduais PAC 2"),
							array("codigo" => "dpiescolamunsemquadra_mais_dpiescolaestsemquadra", "descricao" => "Total de escolas sem quadra"),
							array("codigo" => "dpiescolaestsemcobertura_mais_dpiescolamunsemcobertura", "descricao" => "Total de escolas com quadra descoberta"),
							array("codigo" => "(dpipropostaquadracobertaest_mais_dpipropostaquadracobertamun)", "descricao" => "Proposta de constru��o de quadras cobertas PAC 2"),
							array("codigo" => "dpipropostacoberturaest_mais_dpipropostacoberturamun", "descricao" => "Proposta de cobertura de quadras PAC 2")
						  ); ?>
<table class="tabela" >
	<tr>
		<td class="SubtituloDireita" width="25%" >Tema:</td>
		<td><?php $db->monta_combo("combo_parametro",$arrParametros,"S","Selecione...","","","","200px","") ?></td>
	</tr>
</table>
<br /><br />
<b><span id="span_parametro" ></span></b><br /><br />
<div id="div_parametro" ></div>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<div id="grafico_parametro" >
	<div id="grafico_mapa" ></div>
</div>
<?php } ?>
<script type="text/javascript">

function retornaPosicaoMunicipio(deficit,parametro)
{
		
	deficit = deficit*1;
	if(deficit <= 0){
	  		posicao = 1;
	  	}
	  	
	jQuery.each(arrIncio[parametro],function(key,value) { 
	  	if(deficit >= arrIncio[parametro][key] && deficit <= arrFim[parametro][key]){
	  		posicao =  key + 2;
	  	}
	});	
	
	return posicao;
	
	
}


function retornaPosicaoMunicipio(deficit,parametro)
{
		
	var posicao = '';
	deficit = deficit*1;
	if(deficit <= 0){
	  		posicao = 1;
	  	}
	  	
	jQuery.each(arrIncio[parametro],function(key,value) { 
	  	if(deficit >= arrIncio[parametro][key] && deficit <= arrFim[parametro][key]){
	  		posicao =  key + 1;
	  	}
	});	
	
	if(!posicao)
	{
		return false;
	}else
	{
		return posicao;
	}
}

function carregarParametroGrafico(parametro)
{
	var arrUF = new Array();
	var i = 0;
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    arrUF[i] = jQuery(this).children().attr("id");
			i++;
		}
	});
	var parametroCor = new Array();
	var i = 1
	jQuery.each(jQuery("[id^='div_range']"),function(index,item){
	    var cor = jQuery(this).attr("class");
	    cor = cor.replace("#","");
	    var id = jQuery(this).attr("id");
	    id = id.replace("div_range_","");
	    parametroCor[i] = cor;
	    i++;
	});
	
	var params = ";parametro=" + parametro + ";estuf=" + arrUF + ';cor=' + parametroCor;
	
	swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_mapa", "320", "240", "9.0.0", "expressInstall.swf", {"data-file":"geraGrafico.php?tipoMapasAntigo=mapas" + params,"loading":"Carregando gr�fico..."} );
}

function callBack()
{
	var arrUF = new Array();
	var i = 0;
	jQuery('#estuf').find('option').remove();
	jQuery.each(jQuery("[id=linha_uf]").children(),function(index,item){
	    if(jQuery(this).attr("style"))
		{
		    var id = jQuery(this).children().attr("id");
		    arrUF[i] = id;
		    jQuery('#estuf').append(new Option(jQuery(this).children().attr("title"),id.replace("estuf",""), true, true));
		}
	});
	if(arrUF.length > 0){
		carregarMapasPontos(1,true);
	}else{
		removeTodosPontos();
	}
}
</script>
<?php
include APPRAIZ . 'includes/maps/maps.inc';
if($_REQUEST['quadra'] == "cobertura"){?>
	<script>
	jQuery(document).ready(function() {
  		jQuery("[id^='chk_']").attr("checked","");
  		jQuery("#chk_5").attr("checked","checked");
  		carregarMapasPontosGoogle(1);
	});
	</script>
<?php }
if($_REQUEST['quadra'] == "construcao"){?>
	<script>
	jQuery(document).ready(function() {
  		jQuery("[id^='chk_']").attr("checked","");
  		jQuery("#chk_5").attr("checked","checked");
  		carregarMapasPontosGoogle(1);
	});
	</script>
<?php }
if($_REQUEST['quadra'] == "todos"){?>
	<script>
	jQuery(document).ready(function() {
  		carregarMapasPontosGoogle(1);
	});
	</script>
<?php }