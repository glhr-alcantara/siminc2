<?php

class Public_Controller_Idoc
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/idoc_form&acao=A';

        try {
            $mdIdoc = new Public_Model_Idoc($dados['idoid']);
            $mdIdoc->popularDadosObjeto($dados);
            $mdIdoc->salvar(null,null);
            $mdIdoc->commit();
            simec_redirecionar($url . '&idoid=' . $mdIdoc->idoid, 'success');
        } catch (Exception $e){
            $mdIdoc->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function excluir($idoid)
    {
        $url = '?modulo=apoio/idoc&acao=A';

        try {
            $mdIdoc = new Public_Model_Idoc($idoid);
            $mdIdoc->idostatus = 'I';
            $mdIdoc->salvar();
            $mdIdoc->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdIdoc->rollback();
            simec_redirecionar($url, 'error');
        }
    }

    public function validarIdoc()
    {
        $resposta = array();
        $mdIdoc = new Public_Model_Idoc();
        $mdIdoc->idocod = $_REQUEST['idocod'];
        $mdIdoc->idoid = $_REQUEST['idoid'];
        $verifica = $mdIdoc->carregarPorCodigo();
        if (!empty($verifica)) {
            $resposta['title'] = NULL;
            $resposta['text'] = 'O código do Idoc ' . $mdIdoc->idocod . ' já está sendo usado!';
            $resposta['type'] = 'error';
        } else {
            $resposta['type'] = 'success';
        }
        return simec_json_encode($resposta);
    }
}
