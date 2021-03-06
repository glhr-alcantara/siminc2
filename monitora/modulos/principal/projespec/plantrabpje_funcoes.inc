<?php
/**
 * Esta fun��o verifica se o usu�rio informado j� foi cadastro no projeto.
 * Se n�o faz a inclus�o, se sim atualiza o mesmo.
 * 
 * @param $pje
 * @param $usu
 * @param $influ
 * @param $inter
 * @return void
 */

function projetoaberto()
 {
 	 global $db;
	 
 	// verifica se o projeto esta aberto para ser acompanhado,	//
 	// ou seja, se ele nao esta concluido, cancelado etc.		//
 	
	/*
	1;"Atrasado";"A";"red            "
2;"Cancelado";"A";"red            "
3;"Conclu�do";"A";"blue           "
4;"Em dia";"A";"green          "
5;"N�o iniciado";"A";"yellow         "
6;"Paralisado";"I";"red            "
7;"Suspenso";"A";"black          "
8;"Sem andamento";"A";"black          "
9;"Iniciado";"A";"blue           "
10;"Fase de planejamento";"A";"yellow         "
11;"Fase de Execu��o";"A";"black          "
12;"Fase de Encerramento";"A";"black          "

	*/
	
	$sql = ' SELECT ' .
				'tpscod' .
			' FROM ' .
				'monitora.projetoespecial' .
			' WHERE ' .
				'pjeid' . ' = ' . $_SESSION['pjeid'];
	$sit=$db->pegaUm($sql);
	if ($sit=='11' or $sit=='10')
	{
		// se o projeto estiver Fase de planejamento ou Fase de Execu��o ent�o pode acompanhar
		return true;
	}
//	else
	{
		return false;
	}
}

function atualiza_ator($pje,$usu,$influ,$inter)
{
	global $db;
	// verifica se o projeto tem visibilidade aberta a todos //
	if ($usu <>'')
	{
		$sql =	' SELECT ' .
					'usucpf' .
				' FROM ' .
					'monitora.projeto_ator' .
				' WHERE ' . 
					'usucpf' . ' = ' . '\'' . $usu . '\'' . 
				' AND ' .
					'pjeid' . ' = ' . $pje .
				'';
		if ($db->pegaUm($sql))
		{
			$sql =	' UPDATE ' .
						'monitora.projeto_ator' .
					' SET ' . 
						'peainfluencia' . ' = ' . '\'' . $influ . '\'' .
						',' .
						'peainteresse' . ' = ' . '\'' . $inter . '\'' . 
						',' . 
						'peastatus' . ' = ' . '\'' . 'A' . '\'' . 
					' WHERE ' . 
						'usucpf' . ' = ' . '\'' . $usu . '\'' . 
					' AND ' . 
						'pjeid' . ' = ' . $pje . 
					'';
		}
		else 
		{
			$sql =	' INSERT INTO '.
						'monitora.projeto_ator' .
						' ( ' .
							'pjeid' . 
							',' . 
							'usucpf' . 
							',' . 
							'peainfluencia' . 
							',' . 
							'peainteresse' . 
						' ) ' . 
					' VALUES ' . 
						' ( ' . 
							$pje . 
							',' . 
							'\'' . $usu . '\'' . 
							',' . 
							'\'' . $influ . '\'' . 
							',' . 
							'\'' . $inter . '\'' . 
						' ) ' .
					'';
		}
		$saida = $db->executar($sql);
	}
}

/**
 * Aprova um projeto
 * 
 * @param $pto
 * @return void
 */

function atualiza_aprov($pto)
{
	global $db;
	
	$sql = " UPDATE 
				monitora.planotrabalho  
			 SET  
				ptosnaprovado= 't' 
			WHERE  
				ptostatus='A' and ptoid= $pto";
    $saida = $db->executar($sql);
}

/**
 * Checa se o projeto esta em aberto
 * @return boolean
 */


/**
 * 
 * @param $ptoid
 * @param $i
 * @return unknown
 */
function verifica_macroetapa( $ptoid , $contador )
{
	global $db;
	
	// esta funcao tem por objetivo numerar os filhos da macro-etapa //
	$sql = "select p.ptoid as ptoidfilho,ptotipo, p.ptoordem,p.ptoordem2
		from monitora.planotrabalho p where p.ptostatus='A' and p.ptoid_pai=$ptoid and pjeid=".$_SESSION['pjeid']."
		order by p.ptodata_ini,p.ptotipo,p.ptodata_fim,p.ptodsc";
	$rs = $db->carregar( $sql );
	if ($rs!=false) {
		foreach ( $rs as $registro ){
			//dbg($registro);
			extract($registro);
			$sql = "select ptoid as ptoidfilho, ptocod, ptotipo
					from monitora.planotrabalho
					where ptoid=$ptoidfilho and pjeid=".$_SESSION['pjeid'];
			$filho = $db->pegaLinha($sql);
			$contador++;
			//dbg($sql);
			$sql = "update monitora.planotrabalho set ptoordem2 = $contador where ptoid = $ptoidfilho";
			$db->executar($sql);
			if ($filho['ptotipo']=='M' ) {
				$contador=verifica_macroetapa($ptoidfilho,$contador);
			}
		}
	}
	return $contador;
}

/**
 * 
 */
function pega_nivel( $ptoid ){
	global $db;
	$sql = sprintf( "SELECT ptoid_pai FROM monitora.planotrabalho WHERE ptoid = '%s'", $ptoid );
	$ptoid_pai = $db->pegaUm( $sql );
	if ( $ptoid_pai ) {
		return pega_nivel( $ptoid_pai ) + 1;
	} else {
		return 1;
	}
}

/**
 * esta funcao tem por objetivo ajustar a ordem das atividades de modo a fazer com que
 * toda a lista fique coerente com sua forma de apresentacao
 * 
 * @return void
 */
function novo_ajusta_filhos($id,$ordempai)
{
	global $db;
		$pjeid=$_SESSION['pjeid'];
		$sql = "select ptonivel from monitora.planotrabalho where pjeid=$pjeid and ptostatus='A' order by ptonivel desc limit 1";
	$maiornivel=$db->pegaum($sql);
	$sql = "select p.ptoid from monitora.planotrabalho p where p.ptostatus='A' and ptoid_pai=$id  order by ptocod::integer * 10 ^($maiornivel-ptonivel+1)";
	//and p.ptoid in (select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid'].") 
	
	$ordensfilho = $db->carregar($sql);
	if (is_array ($ordensfilho))
	{
		foreach ($ordensfilho as $ordemfilho)
		{	
			$ordempai++;
			$sql = "update monitora.planotrabalho set ptoordem=$ordempai where ptoid=".$ordemfilho['ptoid'];
			$db->executar($sql);
	        $ordempai = novo_ajusta_filhos($ordemfilho['ptoid'],$ordempai);        	
		}
	}
	return $ordempai;
}

function novo_ajusta_ordem()
{
	global $db;
	$pjeid=$_SESSION['pjeid'];
	$sql = "select ptonivel from monitora.planotrabalho where pjeid=$pjeid and ptostatus='A' order by ptonivel desc limit 1";
	$maiornivel=$db->pegaum($sql);

	/*$sql = "select p.ptoid from monitora.planotrabalho p where p.ptonivel=1 and p.ptostatus='A' 
	and p.ptoid in (select ptoid from monitora.plantrabpje 
where pjeid=".$_SESSION['pjeid'].")  order by ptocod::integer * 10 ^($maiornivel-ptonivel+1)";
*/
	$sql = "select p.ptoid from monitora.planotrabalho p where p.ptonivel=1 and p.ptostatus='A' order by ptocod::integer * 10 ^($maiornivel-ptonivel+1)";
	$ordens = $db->carregar($sql);
	$minhaordem=1;
		if (is_array($ordens)) {
	foreach ($ordens as $ordem)
	{
		$sql = "update monitora.planotrabalho set ptoordem=$minhaordem where ptoid=".$ordem['ptoid'];
		$db->executar($sql);
        $minhaordem = novo_ajusta_filhos($ordem['ptoid'],$minhaordem)+1;
	}	
	}
	
	$db->commit();

}


function ordena_tarefas()
{

	// esta fun��o tem por finalidade reorganizar os c�digos quando da altera��o de um c�digo pai
	// vamos supor que algu�m exclua uma atividade n qdo existem n+1 atividades.
	// esta fun��o ir� fazer com que a atividade n+1 assuma o lugar de n
	// ou ainda, se algu�m move a atividade para uma outra posi��o. esta fun��o ir� reorganizar a �rvore.
	global $db;
	$pjeid=$_SESSION['pjeid'];
	// do primeiro ao �ltimo n�vel
	$sql = "select ptoid, ptocod, ptonivel, ptoordemprov, ptoid_pai from monitora.planotrabalho where pjeid=$pjeid and ptostatus='A' and ptonivel=1 order by ptoordemprov";
	$registros = $db->carregar($sql);
	$j=0;
	if (is_array($registros)) {
	foreach ($registros as $registro)
	{
		$j++;
		// para cada registro devo atualizar o ptoordemprov e o ptocod
		$sql = "update monitora.planotrabalho set ptocod='$j',ptoordemprov=$j where ptoid=".$registro['ptoid'];
		$db->executar($sql);
		ordena_tarefas_filho($registro['ptoid'],$j);
	}
	}

}

function ordena_tarefas_filho($ptoid,$cod)
{

	// esta fun��o tem por finalidade reorganizar os c�digos quando da altera��o de um c�digo pai
	// vamos supor que algu�m exclua uma atividade n qdo existem n+1 atividades.
	// esta fun��o ir� fazer com que a atividade n+1 assuma o lugar de n
	// ou ainda, se algu�m move a atividade para uma outra posi��o. esta fun��o ir� reorganizar a �rvore.
	global $db;
    $jj=0;
	$pjeid=$_SESSION['pjeid'];
	// verificar se possui filho
	$sql = "select ptoid,ptocod, ptonivel, ptoordemprov, ptoid_pai from monitora.planotrabalho where pjeid=$pjeid and ptostatus='A' and ptoid_pai=$ptoid order by ptoordemprov";
	// carrega todos os filhos
	$registros_filho = $db->carregar($sql);
	if (is_array($registros_filho))
	{
		foreach ($registros_filho as $registro_filho)
		{
			$jj++;
			// para cada registro devo atualizar o ptoordemprov e o ptocod
			$sql = "update monitora.planotrabalho set ptocod='$cod"."$jj',ptoordemprov=$jj where ptoid=".$registro_filho['ptoid'];
			$db->executar($sql);
			$njj= $cod . $jj;
			ordena_tarefas_filho($registro_filho['ptoid'], $njj );
		}
	} else return false;
}

function ajusta_ordem( $ptotipo , $ptoordem , $ptoordem2  )
{
	global $db;
	$sql = "update monitora.planotrabalho set ptoordem2=null where pjeid=".$_SESSION['pjeid'];
	//dbg( $sql );
	$db->executar($sql);
	$sql = "select p.ptoid,ptotipo, p.ptoordem  from monitora.planotrabalho p where p.ptostatus='A' 
       and p.ptoid in (select ptoid from monitora.plantrabpje where pjeid=".$_SESSION['pjeid']." and ptoid_pai is null) 
          order by p.ptodata_ini,p.ptotipo,p.ptodata_fim,p.ptodsc";

	$sql = "select p.ptoid,ptotipo, p.ptoordem  from monitora.planotrabalho p where p.ptostatus='A'                  order by p.ptodata_ini,p.ptotipo,p.ptodata_fim,p.ptodsc";
	// ordeno pelas datas de inicio e fim e pelo tipo	//
	$rs = @$db->carregar( $sql );
	if (  $rs && count($rs) > 0 )
	{
		$contador = 1;
		$i=0;
		foreach ( $rs as $linha )
		{
			foreach($linha as $k=>$v) ${$k}=$v;
			$sql = "select ptocod, ptotipo from monitora.planotrabalho where ptoid=$ptoid and pjeid=".$_SESSION['pjeid'];
			$registro=$db->pegaLinha($sql);
			extract($registro);
			
			$sql = "update monitora.planotrabalho set ptoordem2 = $contador where ptoid = $ptoid";
			$db->executar($sql);
			
			if ($ptotipo=='M' ){
				$contador=verifica_macroetapa($ptoid,$contador)+1;
			} else {
				$contador++;
			}
		}
//		dbg(1,1);
	}
	$sql = "update monitora.planotrabalho set ptoordem=ptoordem2 where pjeid=". $_SESSION['pjeid'];
	$db->executar($sql);
	$db->commit();
}

/**
 * Uma etapa n�o pode ser predecessora, diretamente ou n�o, de si mesma.	
 * 
 * @param $ptoid
 * @return void
 */
function elimina_antecessor_recurssivo( $ptoid , $ptoid2=null, $ptoordem=null, $antec=null)
{
	global $db;
	$sql = "select ptoordem,ptoid_antecessor as antec from monitora.planotrabalho where ptoid=$ptoid";
	$res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

	if ($antec)
	{
		$sql = "select ptoid as ptoid2 from monitora.planotrabalho where ptoid_antecessor=$ptoordem and ptoordem=$antec and pjeid=".$_SESSION['pjeid'];

		$ptoid2=$db->pegaum($sql);
		if ($ptoid2)
		{
			// entao aconteceu a recursividade
			$sql = "update monitora.planotrabalho set ptoid_antecessor =null where ptoid=$ptoid2";
			$db->executar($sql);
			$db->commit();

		}
	}
}


/**
 * Verifica se existe necessidade de altera��o nas datas de in�cio e/ou fim do Projeto Especial
 * caso positivo, verifica se as datas do projeto n�o est�o congeladas.
 * As datas passadas na chamada da fun��o s�o as datas de �ncio e t�rmino da atividade
 * 
 * @param integer pjeid
 * @param string dt_ini
 * @param string dt_fim
 * @return boolean
*/
function verifica_data_projetoespecial( $pjeid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select to_char(pjedataini,'dd/mm/yyyy') as pjedataini, to_char(pjedatafim,'dd/mm/yyyy') as pjedatafim, pjesndatafechada from monitora.projetoespecial where pjeid=".$pjeid;
	$resultado = $db->pegaLinha( $sql );
	if( is_array( $resultado ) )
	{
		$mudaInicio = verifica_data_maior( $resultado[ 'pjedataini' ], $dt_ini  ) ? TRUE : FALSE;
		$mudaFim = verifica_data_maior( $dt_fim, $resultado[ 'pjedatafim' ] ) ? TRUE : FALSE;
		//Se as datas do projeto especial estiverem congeladas retorna falso (erro )
		if( $resultado[ 'pjesndatafechada' ] == 't' && ( $mudaInicio || $mudaFim ) )
		{
			return FALSE;
		}
		if( $resultado[ 'pjesndatafechada' ] == 'f' && ( $mudaInicio || $mudaFim ) )
		{
			$dataini = $mudaInicio ? date( 'Y-m-d', gera_timestamp( $dt_ini ) ) : FALSE;
			$datafim = $mudaFim ? date( 'Y-m-d',gera_timestamp( $dt_fim ) ) : FALSE;
			$sql = "update monitora.projetoespecial set ";
			$sql .= $dataini ? "pjedataini = '". $dataini ."'" : '';
			$sql .= $dataini && $datafim ? ', ' : '';
			$sql .= $datafim ? "pjedatafim = '". $datafim ."'" : '';
			$sql .= " where pjeid=".$pjeid;
			return (boolean) $db->executar( $sql );
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

/**
 * Verifica se a data1 � maior que a data2
 * As datas devem estar no formato dd/mm/yyyy
 * 
 * @param string data1
 * @param string data2
 * @return boolean
 */
function verifica_data_maior( $data1, $data2 )
{
	$arrAux = explode( '/', $data1 );
	$diaData1 = $arrAux[ 0 ];
	$mesData1 = $arrAux[ 1 ];
	$anoData1 = $arrAux[ 2 ];
	$arrAux = explode( '/', $data2 );
	$diaData2 = $arrAux[ 0 ];
	$mesData2 = $arrAux[ 1 ];
	$anoData2 = $arrAux[ 2 ];
	$tsData1 = mktime( 0, 0, 0, $mesData1, $diaData1, $anoData1 );
	$tsData2 = mktime( 0, 0, 0, $mesData2, $diaData2, $anoData2 );
	if( $tsData1 > $tsData2 )
	return TRUE;

	return FALSE;
}
/**
 * Retorna um timestamp.
 * Data no formato dd/mm/yyyy
 * 
 * @param string data
 * @return integer
 */
function gera_timestamp( $data )
{
	$arrAux = explode( '/', $data );

	return mktime( 0, 0, 0, $arrAux[ 1 ], $arrAux[ 0 ], $arrAux[ 2 ] );
}

/**
 * Verifica se a atividade possui uma atividade antecessora. 
 * Caso positivo, verifica se as datas da atividade podem ser 
 * alteradas de acordo com a data de t�rmino da antecessora.
 * 
 * @param integer ptoid
 * @param string dt_ini
 * @param string dt_fim
 */
function verifica_antecessor( $ptoid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select ptoid_antecessor from monitora.planotrabalho where ptoid=".$ptoid;
	$ptoid_antecessor = $db->pegaUm( $sql );

	if( $ptoid_antecessor )
	{
		$sql = "select to_char(ptodata_ini, 'dd/mm/yyyy') as dataini, to_char(ptodata_fim, 'dd/mm/yyyy') as datafim from monitora.planotrabalho where ptostatus='A' and ptoordem=".$ptoid_antecessor." and pjeid=".$_SESSION[ "pjeid" ];
		$resultado = $db->pegaLinha( $sql );
		if( $resultado )
		{
			if( verifica_data_maior( $resultado[ 'datafim' ], $dt_ini  ) )
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**
 * Verifica se a atividade possui uma atividade pai.
 * Caso positivo, analisa a necessidade de se alterar as datas da atividade pai.
 * Realiza todo o processo de altera��o de data para a atividade pai recursivamente.
 * 
 * @param integer pto_id
 * @param string dt_ini
 * @param string dt_fim
 * @return boolean
 */
function verifica_pai( $ptoid, $dt_ini, $dt_fim )
{
	global $db;
	$sql = "select ptoid_pai from monitora.planotrabalho where ptoid=".$ptoid." and pjeid=".$_SESSION[ 'pjeid' ];
	$ptoid_pai = $db->pegaUm( $sql );
	if( $ptoid_pai )
	{
		$sql = "select to_char(ptodata_ini, 'dd/mm/yyyy') as dataini, to_char(ptodata_fim, 'dd/mm/yyyy') as datafim, ptosndatafechada from monitora.planotrabalho where ptoid=".$ptoid_pai;
		$resultado = $db->pegaLinha( $sql );
		$mudaDataIni = verifica_data_maior( $resultado[ 'dataini' ], $dt_ini ) ? TRUE : FALSE;
		$mudaDataFim = verifica_data_maior( $dt_fim, $resultado[ 'datafim' ] ) ? TRUE : FALSE;
		if( ( $mudaDataFim || $mudaDataIni ) && $resultado[ 'ptosndatafechada' ] == 't' )
		{
			return FALSE;
		}
		if( ( $mudaDataFim || $mudaDataIni ) && $resultado[ 'ptosndatafechada' ] == 'f' )
		{
			$dataIni = $mudaDataIni ? $dt_ini : $resultado[ 'dataini' ];
			$dataFim = $mudaDataFim ? $dt_fim : $resultado[ 'datafim' ];
			if( !verifica_antecessor( $ptoid_pai, $dataIni, $dataFim ) || !verifica_pai( $ptoid_pai, $dataIni, $dataFim ) || !verifica_sucessor( $ptoid_pai, $dataIni, $dataFim ) )
			{
				return FALSE;
			}

			$dataIni = $mudaDataIni ? date( 'Y-m-d', gera_timestamp( $dt_ini ) ) : FALSE;
			$dataFim = $mudaDataFim ? date( 'Y-m-d',gera_timestamp( $dt_fim ) ) : FALSE;
			$sql = "update monitora.planotrabalho set ";
			$sql .= $dataIni ? "ptodata_ini = '". $dataIni ."'" : '';
			$sql .= $dataIni && $dataFim ? ', ' : '';
			$sql .= $dataFim ? "ptodata_fim = '". $dataFim ."'" : '';
			$sql .= " where ptoid=".$ptoid_pai;

			return (boolean) $db->executar( $sql );
		}
	}

	return TRUE;
}

function chamaCabecalho()
{
	global $db;
	
	$sql = ' SELECT ' .
				'pe.pjesnvisivel' . ' AS ' . 'visivel' . 
				',' . 
				'pe.pjesndatafechada' . ' AS ' . 'projfechado' . 
				',' .
				'pe.pjecod' . 
				',' . 
				'pe.pjedsc' . 
				',' . 
				'to_char(pe.pjedataini,\'dd/mm/YYYY\')' . ' AS ' . 'pjedataini' . 
				' , ' . 
				'to_char(pe.pjedatafim,\'dd/mm/YYYY\')' . ' AS ' . 'pjedatafim' . 
				' , ' . 
				'p.prodsc' . 
				',' .
				'pe.pjeprevistoano,pe.pjevlrano' .
			' FROM ' . 
				'monitora.projetoespecial pe' .
			' INNER JOIN ' .
				'produto p' .
			' ON ' .
				'p.procod' . ' = ' . 'pe.procod' .
			' WHERE ' .
				'pjeid' . ' = ' . $_SESSION['pjeid'];
				
	$res=$db->pegalinha($sql);
	if(is_array($res)) foreach($res as $k=>$v)
	{
		global ${$k};
		${$k}=$v;
	}
	$pjeinimt=0;
	$pjefimmt=0;
}
function encontra_antecessor($antecessores,$ptoid)
{
	global $db;
   $antecessores= explode(',',$_REQUEST['coantecessoras']);
   $id= null;
   $datamaior = '';
   if (is_array($antecessores))
   {
    	foreach ($antecessores as $antecessor)
    	{
    		
    		$valor = intval(strval($antecessor));
    		if (intval(strval($antecessor)))
    		{
    			$sql = "select ptoid,to_char(ptodata_fim,'YYYY/mm/dd') as datafim from monitora.planotrabalho where pjeid=".$_SESSION['pjeid']." and ptoordem=$valor and ptoid <> $ptoid";
    			//dbg($sql);
    			$dados=$db->pegaLinha($sql);
    			if ($dados['datafim']> $datamaior)
    			{
    				$datamaior = $dados['datafim'];
    				$id = $dados['ptoid'];
    			}
    		}
    	}
   }
   return $id;
	
}
/**
 * Verifica se a atividade possui atividades filhas. 
 * Caso positivo, analisa a necessidade de se alterar as datas de cada atividade filha.
 * Realiza todo o processo de altera��o de data para as atividades filhas recursiva
 * */