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
                'success' => 'Operação realizada com sucesso!',
                'acpid' => $dados['acpid']
            );
        } catch (Exception $e){
            $acompanhamentoIndicadorPnc->rollback();
            return array(
                'error' => 'Ocorreu um erro ao executar operação!',
                'acpid' => $dados['acpid']
            );
        }
    }
    
    public static function enviar($dados)
    {
        try {
            $acompanhamentoPnc = new Acompanhamento_Model_Acompanhamentopnc((int)$dados['acpid']);
            $acompanhamentoPnc->acpenviado = TRUE;
            $acompanhamentoPnc->salvar();
            $acompanhamentoPnc->commit();

            return array(
                'success' => 'Operação realizada com sucesso!',
                'acpid' => $dados['acpid']
            );
        } catch (Exception $e){
            $acompanhamentoPnc->rollback();
            return array(
                'error' => 'Ocorreu um erro ao executar operação!',
                'acpid' => $dados['acpid']
            );
        }
    }
    
    public static function salvarAtividadePnc($dados)
    {
        try {
            if($dados['atvid']){
                $acompanhamentoAtividadePnc = new Acompanhamento_Model_Atividadepnc($dados['atvid']);
            } else {
                $acompanhamentoAtividadePnc = new Acompanhamento_Model_Atividadepnc();
            }
            $acompanhamentoAtividadePnc->popularDadosObjeto($dados);
            $acompanhamentoAtividadePnc->atvprodutoqtd = $acompanhamentoAtividadePnc->atvprodutoqtd? desformata_valor($acompanhamentoAtividadePnc->atvprodutoqtd): NULL;
            $acompanhamentoAtividadePnc->salvar(NULL, NULL, ['atvproduto', 'atvprodutoqtd', 'muncod']);
            $acompanhamentoAtividadePnc->commit();

            return array(
                'success' => 'Operação realizada com sucesso!',
                'acpid' => $dados['acpid']
            );
        } catch (Exception $e){
            $acompanhamentoAtividadePnc->rollback();
            return array(
                'error' => 'Ocorreu um erro ao executar operação!',
                'acpid' => $dados['acpid']
            );
        }
    }
    
    public static function excluirAtividadePnc($dados)
    {
        try {
            $acompanhamentoAtividadePnc = new Acompanhamento_Model_Atividadepnc($dados['atvid']);
            $acompanhamentoAtividadePnc->atvstatus = 'I';
            $acompanhamentoAtividadePnc->salvar(NULL, NULL, ['atvproduto', 'atvprodutoqtd', 'muncod']);
            $acompanhamentoAtividadePnc->commit();

            return array(
                'success' => 'Operação realizada com sucesso!',
                'acpid' => $acompanhamentoAtividadePnc->acpid
            );
        } catch (Exception $e){
            $acompanhamentoAtividadePnc->rollback();
            return array(
                'error' => 'Ocorreu um erro ao executar operação!',
                'acpid' => $dados['acpid']
            );
        }
    }
    
    public static function excluirAnexoPnc($dados)
    {
        try {
            $arquivo = new Public_Model_Arquivo($dados['arqid']);
            $arquivo->arqstatus = 'I';
            $arquivo->salvar(NULL, NULL, [
                'arqdescricao',
                'arqextensao',
                'arqtamanho',
                'arqdata',
                'arqhora',
                'arqstatus',
                'usucpf',
                'sisid']);
            $arquivo->commit();

            return array(
                'success' => 'Operação realizada com sucesso!',
                'acpid' => $dados['acpid']
            );
        } catch (Exception $e){
            $arquivo->rollback();
            return array(
                'error' => 'Ocorreu um erro ao executar operação!',
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