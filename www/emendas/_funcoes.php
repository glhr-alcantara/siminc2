<?php

/**
 * Verifica todas as condi��es para o sistema permitir a cria��o de PI e 
 * exportar para o m�dulo Planejamento.
 * 
 * @global cls_banco $db
 * @param integer $benid
 * @return boolean
 */
function condicaoGerarPi($benid){
    global $db;
    $resultado = FALSE;
    $sql = "
        SELECT
            b.benid
        FROM emendas.beneficiario b
            JOIN emendas.emenda e ON b.emeid = e.emeid
        WHERE
            benid = ". (int)$benid. "
            AND COALESCE(plititulo, NULL) IS NOT NULL
            AND COALESCE(plidsc, NULL) IS NOT NULL
            AND e.unoid IS NOT NULL
            AND b.suoid IS NOT NULL
            AND b.esfid IS NOT NULL
            AND e.ptrid IS NOT NULL
            AND b.pprid IS NOT NULL
            AND b.pumid IS NOT NULL
            AND b.picquantidade IS NOT NULL
            AND b.mdeid IS NOT NULL
            AND b.neeid IS NOT NULL
            AND b.neeid IS NOT NULL
            AND b.capid IS NOT NULL
            AND b.bented IS NOT NULL
    ";
    $beneficiario = $db->pegaUm($sql);
    if($beneficiario){
        $resultado = TRUE;
    }
    
    return $resultado;
}

/**
 * Busca os dados da emenda e cria um novo PI no m�dulo Planejamento com os 
 * dados da emenda.
 * 
 * @global cls_banco $db
 * @param integer $benid
 * @return boolean
 */
function posAcaoGerarPi($benid){
    global $db;
    $resultado = FALSE;
    $sqlBeneficiario = montarSqlDadosBeneficiario($benid);
    $beneficiario = $db->pegaLinha($sqlBeneficiario);
    if($beneficiario){
        # Salva associa��o da funcional com a Subunidade com o valor de custeio e capital
        associarFuncionalSubunidade((object)$beneficiario);
        # Adapta informa��es de Emenda e Beneficiario para Salvar o PI.
        $pi = adaptarBeneficioPi((object)$beneficiario);
//ver($pi, d);
        # Salva os dados de PI
        $pliid = salvarPI((array)$pi, TRUE);
        # Salva associa��o entre o Beneficiario e o PI salvo.
        $modelBeneficiario = new Emendas_Model_Beneficiario($benid);
        $modelBeneficiario->pliid = $pliid;
        $modelBeneficiario->salvar();
//ver($pliid, WF_TPDID_PLANEJAMENTO_PI, AED_ENVIAR_APROVACAO, d);
        $tipoFluxoFNC = verificarPiFnc($pliid);
//var_dump($tipoFluxoFNC); die;
        if($tipoFluxoFNC){
            # Gera n�mero de documento pra workflow
            $docid = pegarDocidPi($pliid, WF_TPDID_PLANEJAMENTO_PI_FNC);
            # Altera a situa��o do PI pra Em An�lise
            $resultado = wf_alterarEstado($docid, AED_SELECIONAR_PROJETO_EMENDAS_FNC, 'PI Gerado pelo Emendas', array('pliid' => $pliid));
//ver($pliid, $docid, AED_SELECIONAR_PROJETO_EMENDAS_FNC, $resultado, d);
        } else {
            # Gera n�mero de documento pra workflow
            $docid = pegarDocidPi($pliid, WF_TPDID_PLANEJAMENTO_PI);
            # Altera a situa��o do PI pra Aguardando Aprova��o
            $resultado = wf_alterarEstado($docid, AED_ENVIAR_APROVACAO, 'PI Gerado pelo Emendas', array('pliid' => $pliid));
//ver(WF_TPDID_PLANEJAMENTO_PI, AED_ENVIAR_APROVACAO, $pliid, $resultado, d);
        }
    }
//ver($resultado, $pliid, d);
    return $resultado;
}

/**
 * Salva associa��o da funcional com a Subunidade com o valor de custeio e capital
 * 
 * @param stdClass $beneficiario
 */
function associarFuncionalSubunidade(stdClass $beneficiario){
    $funcionalCusteio = buscarValorTotalEmenda($beneficiario->emeid, Emendas_Model_EmendaDetalhe::GND_COD_CUSTEIO_DESPESAS. ','. Emendas_Model_EmendaDetalhe::GND_COD_CUSTEIO_JUROS. ','. Emendas_Model_EmendaDetalhe::GND_COD_CUSTEIO_PESSOAL);
    $funcionalCapital = buscarValorTotalEmenda($beneficiario->emeid, Emendas_Model_EmendaDetalhe::GND_COD_CAPITAL_INVESTIMENTO. ','. Emendas_Model_EmendaDetalhe::GND_COD_CAPITAL_INVERSOES. ','. Emendas_Model_EmendaDetalhe::GND_COD_CAPITAL_AMORTIZACAO);
    
    $listaPtresSubunidade = (new Spo_Model_PtresSubunidade())->recuperarTodos('psuid', ['ptrid = ' . (int)$beneficiario->ptrid, 'suoid = ' . (int)$beneficiario->suoid]);
    $ptresSubunidade = current($listaPtresSubunidade);
//ver($listaPtresSubunidade, d);
    
    $modelPtresSubunidade = new Spo_Model_PtresSubunidade($ptresSubunidade['psuid']);
    $modelPtresSubunidade->ptrid = $beneficiario->ptrid;
    $modelPtresSubunidade->suoid = $beneficiario->suoid;
    $modelPtresSubunidade->ptrdotacaocusteio = $funcionalCusteio;
    $modelPtresSubunidade->ptrdotacaocapital = $funcionalCapital;
    $modelPtresSubunidade->salvar();
}

/**
 * Adpata o formato dos dados do beneficiarios para o formato de PI para serem 
 * salvos dados.
 * 
 * @param stdClass $beneficiario
 * @return stdClass $pi Dados do pi para serem salvos.
 */
function adaptarBeneficioPi(stdClass $beneficiario){
    require_once APPRAIZ .'emendas/classes/model/Siconv.inc';

    $emenda = new Emendas_Model_Emenda($beneficiario->emeid);
    
//ver($beneficiario);
    # Buscando enquadramento 2018 do tipo emenda.
    $eqdid = buscarEnquadramentoEmenda($beneficiario->prsano);
    $unicod = buscarUnidadeOrcamentariaEmenda($beneficiario->unoid);
    $ungcod = buscarSubUnidadeOrcamentariaEmenda($beneficiario->suoid);
    $listaSniic = buscarListaSniic($beneficiario->benid);
    $listaPronac = buscarListaPronac($beneficiario->benid);
    $listaAnexos = buscarListaAnexos($beneficiario->benid);
    $listaDelegacao = buscarListaDelegacao($beneficiario->benid);
    $piCusteio = buscarValorTotalBeneficiario($beneficiario->benid, Emendas_Model_EmendaDetalhe::GND_COD_CUSTEIO_DESPESAS. ','. Emendas_Model_EmendaDetalhe::GND_COD_CUSTEIO_JUROS. ','. Emendas_Model_EmendaDetalhe::GND_COD_CUSTEIO_PESSOAL);
    $piCapital = buscarValorTotalBeneficiario($beneficiario->benid, Emendas_Model_EmendaDetalhe::GND_COD_CAPITAL_INVESTIMENTO. ','. Emendas_Model_EmendaDetalhe::GND_COD_CAPITAL_INVERSOES. ','. Emendas_Model_EmendaDetalhe::GND_COD_CAPITAL_AMORTIZACAO);
    # Buscando numero do conv�nio do beneficiario
    $listaDadosSiconv = (new Emendas_Model_Siconv)->recuperarListagem($benid);

    # PI - Basico
    $pi = new stdClass();
    $pi->pliid = $beneficiario->pliid;
    $pi->mdeid = $beneficiario->mdeid;
    $pi->eqdid = $eqdid;
    $pi->neeid = $beneficiario->neeid;
    $pi->capid = $beneficiario->capid;
    $pi->plititulo = $beneficiario->plititulo. ($emenda->emenumero? ' - '. $emenda->emenumero: NULL);
    $pi->plidsc = $beneficiario->plidsc;
    # Ptres
    $pi->ptrid = $beneficiario->ptrid;
    $pi->usucpf = $_SESSION['usucpf'];
    $pi->unicod = $unicod;
    $pi->ungcod = $ungcod;
    $pi->pliano = $beneficiario->prsano;
    # PI Complemento
    $pi->picedital = 'f';
    $pi->oppid = $beneficiario->oppid;
    $pi->mppid = $beneficiario->mppid;
    $pi->ippid = $beneficiario->ippid;
    $pi->mpnid = $beneficiario->mpnid;
    $pi->ipnid = $beneficiario->ipnid;
    $pi->pprid = $beneficiario->pprid;
    $pi->pumid = $beneficiario->pumid;
    $pi->picquantidade = $beneficiario->picquantidade;
    $pi->picted = $beneficiario->bented;
    $pi->picedital = $beneficiario->bented;
    $pi->picvalorcusteio = $piCusteio? number_format($piCusteio, 2, ',', '.'): NULL;
    $pi->picvalorcapital = $piCapital? number_format($piCapital, 2, ',', '.'): NULL;
    # PI Associac�es
    if($listaDadosSiconv[0]['numeroconvenio']){
        $pi->lista_convenio = array(0 => str_replace(array('/', '.', ',','-'), '', $listaDadosSiconv[0]['numeroconvenio']));
    }
    $pi->lista_sniic = $listaSniic;
    if($beneficiario->bennumeroprocesso){
        $pi->lista_sei = array(0 => str_replace(array('/', '.', ',','-'), '', $beneficiario->bennumeroprocesso));
    }
    $pi->lista_pronac = $listaPronac;
    $pi->esfid = $beneficiario->esfid;
    
    switch($beneficiario->esfid){
        case Territorios_Model_Esfera::K_ESTADUAL:
            $pi->listaLocalizacaoEstadual = buscarListaLocalizacao($beneficiario->benid, $beneficiario->esfid);
        break;
        case Territorios_Model_Esfera::K_MUNICIPAL:
            $pi->listaLocalizacao = buscarListaLocalizacao($beneficiario->benid, $beneficiario->esfid);
        break;
        case Territorios_Model_Esfera::K_EXTERIOR:
            $pi->listaLocalizacaoExterior = buscarListaLocalizacao($beneficiario->benid, $beneficiario->esfid);
        break;
    }
    
    $pi->listaAnexos = $listaAnexos;
    $pi->delegacao = $listaDelegacao;
    
    return $pi;
}

/**
 * 
 * 
 * @global cls_banco $db
 * @param integer $benid
 * @param string $gnd
 * @return float
 */
function buscarValorTotalBeneficiario($benid, $gnd){
    global $db;
    $sql = "
        SELECT
            SUM(COALESCE(bedvalor, 0)) as valor
        FROM emendas.beneficiariodetalhe bed
            JOIN emendas.emendadetalhe emd on emd.emdid = bed.emdid
        WHERE
            gndcod IN(". $gnd. ")
            AND bed.benid = ". (int)$benid;
    $resultado = $db->pegaUm($sql);
    $valor = $resultado? $resultado: NULL;
    
    return $valor;
}

/**
 * 
 * 
 * @global cls_banco $db
 * @param integer $emeid
 * @param string $gnd
 * @return float
 */
function buscarValorTotalEmenda($emeid, $gnd){
    global $db;
    $sql = "
        SELECT
            SUM(COALESCE(emdvalor, 0)) AS valor
        FROM emendas.emendadetalhe
        WHERE
            gndcod IN(". $gnd. ")
            AND emeid = ". (int)$emeid;
    $resultado = $db->pegaUm($sql);
    $valor = $resultado? $resultado: NULL;
    
    return $valor;
}

/**
 * 
 * 
 * @global cls_banco $db
 * @param integer $benid
 * @return array
 */
function buscarListaDelegacao($benid){
    global $db;
    $listaDelegada = array();
    $sql = "
        SELECT
            suo.suoid
        FROM public.vw_subunidadeorcamentaria suo
            JOIN emendas.delegacao d ON(suo.suoid = d.suoid)
        WHERE
            suo.suostatus = 'A'
            AND benid = ". (int)$benid;
    $resultado = $db->carregar($sql);
    if($resultado){
        foreach($resultado as $arquivo){
            $listaDelegada[] = $arquivo['suoid'];
        }
    }
    
    return $listaDelegada;
}

/**
 * 
 * 
 * @global cls_banco $db
 * @param integer $benid
 * @return array
 */
function buscarListaAnexos($benid){
    global $db;
    $listaAnexos = array();
    $sql = "
        SELECT
            a.arqid
        FROM emendas.beneficiario_anexo ba
            JOIN public.arquivo a ON(ba.arqid = a.arqid)
        WHERE
            ba.beastatus = 'A'
            AND ba.benid = ". (int)$benid;
    $resultado = $db->carregar($sql);
    if($resultado){
        foreach($resultado as $arquivo){
            # Copia o arquivo fisico da pasta do m�dulo emendas para a pasta equivalente do m�dulo planejamento.
            $origem = APPRAIZ. "arquivos/emendas". '/'. floor($arquivo['arqid']/1000). '/'. $arquivo['arqid'];
            $destino = APPRAIZ. "arquivos/planacomorc". '/'. floor($arquivo['arqid']/1000). '/'. $arquivo['arqid'];
            copy($origem, $destino);
            # Insere o c�digo do arquivo na lista pra vincular ao PI.
            $listaAnexos[] = $arquivo['arqid'];
        }
    }
    
    return $listaAnexos;
}

/**
 * 
 * 
 * @global cls_banco $db
 * @param integer $benid C�digo do beneficiario
 * @param integer $esfera C�digo da esfera
 * @return array
 */
function buscarListaLocalizacao($benid, $esfera){
    global $db;
    $listaLocalizacao = array();
    
    switch($esfera){
        case Territorios_Model_Esfera::K_ESTADUAL:
            $coluna = 'estuf';
        break;
        case Territorios_Model_Esfera::K_MUNICIPAL:
            $coluna = 'muncod';
        break;
        case Territorios_Model_Esfera::K_EXTERIOR:
            $coluna = 'paiid';
        break;
    }
    $sql = "
        SELECT
            $coluna
        FROM emendas.beneficiariolocalizacao
        WHERE
            benid = ". (int)$benid;

    $resultado = $db->carregar($sql);
    if($resultado){
        foreach($resultado as $localizacao){
            $listaLocalizacao[] = $localizacao[$coluna];
        }
    }
    
    return $listaLocalizacao;
}

/**
 * 
 * 
 * @global cls_banco $db
 * @param integer $benid
 * @return array
 */
function buscarListaPronac($benid){
    global $db;
    $listaPronac = array();
    $sql = "
        SELECT
            pronumero
        FROM emendas.pronac
        WHERE
            benid = ". (int)$benid;
    $resultado = $db->carregar($sql);
    if($resultado){
        foreach($resultado as $pronac){
            $listaPronac[] = $pronac['pronumero'];
        }
    }
    
    return $listaPronac;
}

/**
 * 
 * 
 * @global cls_banco $db
 * @param integer $benid
 * @return array
 */
function buscarListaSniic($benid){
    global $db;
    $listaSniic = array();
    $sql = "
        SELECT
            sninumero
        FROM emendas.sniic
        WHERE
            benid = ". (int)$benid;
    $resultado = $db->carregar($sql);
    if($resultado){
        foreach($resultado as $sniic){
            $listaSniic[] = $sniic['sninumero'];
        }
    }
    
    return $listaSniic;
}

function buscarUnidadeOrcamentariaEmenda($unoid){
    global $db;
    $sql = "
        SELECT
            unocod
        FROM public.vw_subunidadeorcamentaria
        WHERE
            suostatus = 'A'
            AND unoid = ". (int)$unoid;
    $codigo = $db->pegaUm($sql);
    return $codigo;
}

function buscarSubUnidadeOrcamentariaEmenda($suoid){
    global $db;
    $sql = "
        SELECT
            suocod
        FROM public.vw_subunidadeorcamentaria
        WHERE
            suostatus = 'A'
            AND suoid = ". (int)$suoid;
    $codigo = $db->pegaUm($sql);
    return $codigo;
}

/**
 * Busca o enquadramento de acordo com o ano.
 * 
 * @global cls_banco $db
 * @param integer $ano
 * @return integer
 */
function buscarEnquadramentoEmenda($ano){
    global $db;
    $sql = "
        SELECT
            eqdid
        FROM monitora.pi_enquadramentodespesa
        WHERE
            eqdcod = 'E'
            AND eqdano = '". (int)$ano. "'";
    $codigo = $db->pegaUm($sql);
    return $codigo;
}

/**
 * Monta consulta aos dados de beneficiarios para exportar os dados.
 * 
 * @param integer $benid
 * @return string
 */
function montarSqlDadosBeneficiario($benid){
    $sql = "
        SELECT
            e.emeid,
            e.emenumero,
            e.unoid,
            e.ptrid,
            e.emejustificativa,
            e.emeimpositiva,
            e.autid,
            e.prsano,
            e.emestatus,
            b.benid,
            b.suoid,
            b.proid,
            b.esfid,
            b.benformaexecucao,
            b.bennumeroprocesso,
            b.beninicio,
            b.benprogramatitulo,
            b.benprogramaobjeto,
            b.benstatus,
            b.benpprogramanumero,
            b.benpropostatitulo,
            b.benpropostaobjeto,
            b.benpropostanumero,
            b.benempenhado,
            b.benconveniado,
            b.benpago,
            b.benmotivoimpedimento,
            b.sicid,
            b.impid,
            b.mdeid,
            b.neeid,
            b.capid,
            b.bented,
            b.benparecertecnico,
            b.benparecerjuridico,
            b.docid,
            b.plititulo,
            b.plidsc,
            b.oppid,
            b.mppid,
            b.ippid,
            b.mpnid,
            b.ipnid,
            b.pumid,
            b.pprid,
            b.picquantidade,
            pi.pliid
        FROM
            emendas.beneficiario b
            JOIN emendas.emenda e ON b.emeid = e.emeid
            LEFT JOIN monitora.pi_planointerno pi ON(
                b.pliid = pi.pliid
                AND plistatus = 'A'
            )
        WHERE
            benid = ". (int)$benid. "
    ";
    return $sql;
}
