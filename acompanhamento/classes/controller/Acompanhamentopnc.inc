<?php

class Acompanhamento_Controller_Acompanhamentopnc
{
    public function salvar($dados, $urlModulo = NULL)
    {
        $url = '?modulo=principal/monitorar-pnc&acao=A';

        try {
            $mAcompanhamento = new Acompanhamento_Model_Acompanhamentopnc($dados['acpid']);
            $mAcompanhamento->popularDadosObjeto($dados);

            $mAcompanhamento->acoquantidade = $mAcompanhamento->acoquantidade ? desformata_valor($mAcompanhamento->acoquantidade) : NULL;
            $mAcompanhamento->acodata = $mAcompanhamento->acodata ? formata_data_sql($mAcompanhamento->acodata) : NULL;

            $mAcompanhamento->prsano = $mAcompanhamento->prsano ? $mAcompanhamento->prsano : $_SESSION['exercicio'];

            $mAcompanhamento->salvar(NULL, NULL, [
                'acoanalise',
                'acoprovidencias',
                'acojustificativa',
                'claid',
                'unocod',
                'suocod',
                'ipncod'
            ]);

            $mAcompanhamento->associarAnexo($dados);
            $this->_vincularMedidas($mAcompanhamento->acpid, $dados);
            $this->_gravarHistorico($mAcompanhamento->acpid);

            $mAcompanhamento->commit();
            simec_redirecionar($url. '&acpid='. $mAcompanhamento->acpid, 'success');
        } catch (Exception $e){
            $mAcompanhamento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
    public static function salvarIndicadorPnc($dados)
    {
//ver($dados, d);
        try {
            if($dados['idpid']){
                $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Indicadorpnc($dados['idpid']);
            } else {
                $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Indicadorpnc();
            }
            $acompanhamentoIndicadorPnc->popularDadosObjeto($dados);

            $acompanhamentoIndicadorPnc->idpquantidade = $acompanhamentoIndicadorPnc->idpquantidade? desformata_valor($acompanhamentoIndicadorPnc->idpquantidade): NULL;
            $acompanhamentoIndicadorPnc->idpdata = $acompanhamentoIndicadorPnc->idpdata? formata_data_sql($acompanhamentoIndicadorPnc->idpdata): NULL;

//            $mAcompanhamento->prsano = $mAcompanhamento->prsano? $mAcompanhamento->prsano: $_SESSION['exercicio'];

            $acompanhamentoIndicadorPnc->salvar(NULL, NULL, ['idpjustificativa']);

//            $this->_gravarHistorico($mAcompanhamento->acpid);

            $acompanhamentoIndicadorPnc->commit();
//ver($mAcompanhamento->acpid, d);
            return array(
                'success' => utf8_encode('Operação realizada com sucesso!'),
                'acpid' => $dados['acpid']
            );
        } catch (Exception $e){
            $acompanhamentoIndicadorPnc->rollback();
            return array(
                'error' => utf8_encode('Ocorreu um erro ao executar operação!'),
                'acpid' => $dados['acpid']
            );
        }
    }

    /**
     * Lista os indicadores PNC do ano atual e anos anteriores
     * 
     * @param int $janid
     * @param int $suoid
     * @param int $mpnid
     * @param int $exercicio
     */
    public function listarIndicadoresPNC($acpid = NULL){
        include APPRAIZ. 'acompanhamento/modulos/principal/listar-indicadores-pnc.inc';
    }
    
    public static function buscarAcompanhamentoJanelaSubunidadeMeta($suoid = NULL, $janid = NULL, $mpnid = NULL){
        $modelAcompanhamento = new Acompanhamento_Model_Acompanhamentopnc();
        $modelAcompanhamento->suoid = $suoid;
        $modelAcompanhamento->janid = $janid;
        $modelAcompanhamento->mpnid = $mpnid;
        $acompanhamento = $modelAcompanhamento->buscarPorJanelaSubunidadeMeta();
        return $acompanhamento;
    }
    
    public function excluir($acpid)
    {
        $url = '?modulo=principal/monitorar-ppa&acao=A';

        try {
            $mAcompanhamento = new Acompanhamento_Model_Acompanhamentopnc($acpid);
            
            if($mAcompanhamento->existeVinculo()){
                $mensagem = 'O registro não pode ser excluído pois possui vínculo com algum PI.';
                $url = '?modulo=apoio/modalidade-pactuacao-form&acao=A&capid=' . $mAcompanhamento->acpid;
                simec_redirecionar($url, 'error', $mensagem);
            }            
            
            $mAcompanhamento->acostatus = 'I';
            $mAcompanhamento->salvar();
            $mAcompanhamento->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mAcompanhamento->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    private function _gravarHistorico($acpid)
    {
        $mHistorico = new Acompanhamento_Model_Historico();

        $mHistorico->acpid = $acpid;
        $mHistorico->usucpf = $_SESSION['usucpforigem'];
        $mHistorico->hisdata = date('Y-m-d H:i:s');
        
        $mHistorico->salvar();
    }

    public function recuperarPiPorMetaESubunidade($mppid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorMetaESubunidade($mppid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }

    public function recuperarPiPorIndicadorESubunidade($ipnid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorIndicadorESubunidade($ipnid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }
    
    public function recuperarPiPorMetaPncESubunidade($mpnid, $suoid)
    {
        $mPi = new Pi_PlanoInterno();
        $aPis = $mPi->recuperarPiPorMetaPncESubunidade($mpnid, $suoid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/detalhe-pi.inc';
    }

    public function recuperarHistorico($acpid)
    {
        $aHistorico = (new Acompanhamento_Model_Historico())->recuperarHistorico($acpid);

        include_once APPRAIZ . 'acompanhamento/modulos/principal/historico.inc';
    }

    public function listarMetasPnc($suoid, $janid = NULL)
    {
        $mAcompanhamentopnc = new Acompanhamento_Model_Acompanhamentopnc();

        $mJanela = new Acompanhamento_Model_Janela();
        $janelaAtiva = $mJanela->recuperarJanelaAtiva(Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC);
        $aJanelas = $mJanela->recuperarTodos('*', ["prsano = '{$_SESSION['exercicio']}'", "janstatus = 'A'", "tipid = " . Acompanhamento_Model_Tipo::K_TIPO_INDICADOR_PNC], 'janinicio desc');

        include_once APPRAIZ . 'acompanhamento/modulos/principal/listar-metas-pnc.inc';
    }

}