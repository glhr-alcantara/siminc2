<?php

class Public_Controller_Iduso
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/iduso_form&acao=A';

        try {
            $mdIduso = new Public_Model_IdentifUso($dados['iduid']);
            $mdIduso->popularDadosObjeto($dados);
            $mdIduso->salvar();
            $mdIduso->commit();
            simec_redirecionar($url . '&iduid=' . $mdIduso->iduid, 'success');
        } catch (Exception $e){
            $mdIduso->rollback();
            simec_redirecionar($url, 'error');
        }

    }
   
    public function excluir($iduid)
    {
        $url = '?modulo=apoio/iduso&acao=A';
        try {
            $mdIduso = new Public_Model_IdentifUso($iduid);
            $mdIduso->idustatus = 'I';
            $mdIduso->salvar();
            $mdIduso->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdIduso->rollback();
            simec_redirecionar($url, 'error');
        }

    }

    public function validarIduso()
    {
        $resposta = array();
        $mdIduso = new Public_Model_IdentifUso();
        $mdIduso->iducod = $_REQUEST['iducod'];
        $mdIduso->iduid = $_REQUEST['iduid'];
        $verifica = $mdIduso->carregarPorCodigo();
        if (!empty($verifica)) {
            $resposta['title'] = NULL;
            $resposta['text'] = 'O código do Iduso ' . $mdIduso->iducod . ' já está sendo usado!';
            $resposta['type'] = 'error';
        } else {
            $resposta['type'] = 'success';
        }
        return simec_json_encode($resposta);
    }

}
