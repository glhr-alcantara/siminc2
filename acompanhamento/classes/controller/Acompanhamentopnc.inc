<?php

class Acompanhamento_Controller_Acompanhamentopnc
{
    public static function salvarIndicadorPnc($dados)
    {
        try {
            if($dados['idpid']){
                $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Indicadorpnc($dados['idpid']);
            } else {
                $acompanhamentoIndicadorPnc = new Acompanhamento_Model_Indicadorpnc();
            }
            $acompanhamentoIndicadorPnc->popularDadosObjeto($dados);

            $acompanhamentoIndicadorPnc->idpquantidade = $acompanhamentoIndicadorPnc->idpquantidade? desformata_valor($acompanhamentoIndicadorPnc->idpquantidade): NULL;
            $acompanhamentoIndicadorPnc->idpdata = $acompanhamentoIndicadorPnc->idpdata? formata_data_sql($acompanhamentoIndicadorPnc->idpdata): NULL;
            $acompanhamentoIndicadorPnc->salvar(NULL, NULL, ['idpjustificativa']);
            $acompanhamentoIndicadorPnc->commit();

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
    
    public static function buscarAcompanhamentoJanelaSubunidadeMeta($suoid = NULL, $janid = NULL, $mpnid = NULL){
        $modelAcompanhamento = new Acompanhamento_Model_Acompanhamentopnc();
        $modelAcompanhamento->suoid = $suoid;
        $modelAcompanhamento->janid = $janid;
        $modelAcompanhamento->mpnid = $mpnid;
        $acompanhamento = $modelAcompanhamento->buscarPorJanelaSubunidadeMeta();
        return $acompanhamento;
    }
}